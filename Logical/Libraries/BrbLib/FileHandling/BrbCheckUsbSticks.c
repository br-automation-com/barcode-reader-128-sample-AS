/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbCheckUsbSticks.c
 * Author: niedermeierr
 * Created: October 10, 2014
 ********************************************************************
 * Implementation of library BrbLib
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"
#include <string.h>
#include <AnsiCFunc.h>

#ifdef __cplusplus
	};
#endif

// Eigene Enums und Typen
enum Steps_ENUM
{
	eSTEP_INIT,
	eSTEP_CHECK_DEVICES,
	eSTEP_CHECK_UNLINK_DEVICES,
	eSTEP_REMOVE_DEVICE,
	eSTEP_UNLINK_DEVICE,
	eSTEP_CHECK_LINK_DEVICES,
	eSTEP_ADD_DEVICE,
	eSTEP_LINK_DEVICE,
};

/* Pr�ft, ob ein Usb-Medium gesteckt wurde und linkt es */
void BrbCheckUsbSticks(struct BrbCheckUsbSticks* inst)
{
	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			memset(&inst->fbUsbNodeGet, 0, sizeof(inst->fbUsbNodeGet));
			memset(&inst->UsbNodeInfo, 0, sizeof(inst->UsbNodeInfo));
			memset(&inst->DeviceListMan, 0, sizeof(inst->DeviceListMan));
			inst->DeviceListMan.pList = (UDINT)&inst->UsbDeviceList;
			inst->DeviceListMan.nEntryLength = sizeof(BrbUsbDeviceListEntry_TYP);
			inst->DeviceListMan.nIndexMax = nBRB_USB_DEVICE_LIST_INDEX_MAX;
			BrbMemListClear(&inst->DeviceListMan);
			inst->eStep = eSTEP_CHECK_DEVICES;
			break;
		
		case eSTEP_CHECK_DEVICES:
			inst->nStatus = 0;
			// �nderung pr�fen
			inst->fbUsbNodeListGet.enable = 1;
			inst->fbUsbNodeListGet.pBuffer = (UDINT)&inst->UsbNodeList;
			inst->fbUsbNodeListGet.bufferSize = sizeof(inst->UsbNodeList);
			inst->fbUsbNodeListGet.filterInterfaceClass = asusb_CLASS_MASS_STORAGE;
			inst->fbUsbNodeListGet.filterInterfaceSubClass = asusb_SUBCLASS_SCSI_COMMAND_SET;
			UsbNodeListGet(&inst->fbUsbNodeListGet);
			if(inst->fbUsbNodeListGet.status == 0 || inst->fbUsbNodeListGet.status == 32900)
			{
				if(inst->fbUsbNodeListGet.attachDetachCount != inst->nAttachDetachCountOld)
				{
					// �nderung erkannt
					inst->nStatus = eBRB_ERR_BUSY;
					inst->nUsbDeviceIndex = 0;
					inst->eStep = eSTEP_CHECK_UNLINK_DEVICES;
				}
				inst->nAttachDetachCountOld = inst->fbUsbNodeListGet.attachDetachCount;
			}
			else if(inst->fbUsbNodeListGet.status != 65535)
			{
				// Fehler
				inst->nStatus = inst->fbUsbNodeListGet.status;
			}
			inst->bUsbDeviceCountChanged = 0;
			break;

		case eSTEP_CHECK_UNLINK_DEVICES:
			// Pr�fen, ob ein Device entfernt werden muss -> Liste der Devices durchtickern
			if(inst->nUsbDeviceIndex < inst->DeviceListMan.nEntryCount)
			{
				if(inst->UsbDeviceList[inst->nUsbDeviceIndex].nNode > 0)
				{
					// Pr�fen, ob es den Node noch gibt
					BOOL bNodeFound = 0;
					for(inst->nUsbNodeIndex=0; inst->nUsbNodeIndex<=nBRB_USB_DEVICE_LIST_INDEX_MAX; inst->nUsbNodeIndex++)
					{
						if(inst->UsbDeviceList[inst->nUsbDeviceIndex].nNode == inst->UsbNodeList[inst->nUsbNodeIndex])
						{
							bNodeFound = 1;
							break;
						}
					}
					if(bNodeFound == 0)
					{
						// Node ist nicht mehr vorhanden -> Device l�schen
						inst->eStep = eSTEP_REMOVE_DEVICE;
					}
					else
					{
						// N�chtes Device
						inst->nUsbDeviceIndex += 1;
					}
				}
			}
			else
			{
				// Device-Liste ist zu Ende
				inst->nUsbNodeIndex = 0;
				inst->eStep = eSTEP_CHECK_LINK_DEVICES;
			}
			break;

		case eSTEP_REMOVE_DEVICE:
			BrbMemListOut(&inst->DeviceListMan, inst->nUsbDeviceIndex, (UDINT)&inst->UsbDeviceListEntry);
			if(inst->UsbDeviceListEntry.nHandle > 0)
			{
				// Device muss ungelinkt werden
				inst->eStep = eSTEP_UNLINK_DEVICE;
			}
			else
			{
				// N�chtes Device
				inst->nUsbDeviceIndex += 1;
				inst->eStep = eSTEP_CHECK_UNLINK_DEVICES;
			}
			break;
		
		case eSTEP_UNLINK_DEVICE:
			inst->fbDevUnlink.enable = 1;
			inst->fbDevUnlink.handle = inst->UsbDeviceListEntry.nHandle;
			DevUnlink(&inst->fbDevUnlink);
			if(inst->fbDevUnlink.status != 65535)
			{
				// N�chtes Device
				inst->nUsbDeviceIndex += 1;
				inst->eStep = eSTEP_CHECK_UNLINK_DEVICES;
			}
			break;

		case eSTEP_CHECK_LINK_DEVICES:
			// Pr�fen, ob ein Device hinzukommt -> Liste der Nodes durchtickern
			if(inst->nUsbNodeIndex <= nBRB_USB_DEVICE_LIST_INDEX_MAX)
			{
				if(inst->UsbNodeList[inst->nUsbNodeIndex] > 0)
				{
					// Pr�fen, ob es das Device schon gibt
					BOOL bDeviceFound = 0;
					for(inst->nUsbDeviceIndex=0; inst->nUsbDeviceIndex<inst->DeviceListMan.nEntryCount; inst->nUsbDeviceIndex++)
					{
						if(inst->UsbDeviceList[inst->nUsbDeviceIndex].nNode == inst->UsbNodeList[inst->nUsbNodeIndex])
						{
							bDeviceFound = 1;
							break;
						}
					}
					if(bDeviceFound == 0)
					{
						// Device ist noch nicht vorhanden -> Device hinzuf�gen
						inst->eStep = eSTEP_ADD_DEVICE;
					}
					else
					{
						// Device ist schon vorhanden ->N�chster Node
						inst->nUsbNodeIndex += 1;
					}
				}
				else
				{
					// N�chster Node
					inst->nUsbNodeIndex += 1;
				}
			}
			else
			{
				// Liste ist zu Ende
				inst->nUsbDeviceCount = inst->DeviceListMan.nEntryCount;
				inst->bUsbDeviceCountChanged = 1;
				inst->eStep = eSTEP_CHECK_DEVICES;
			}
			break;
		
		case eSTEP_ADD_DEVICE:
			// Informationen holen
			inst->fbUsbNodeGet.enable = 1;
			inst->fbUsbNodeGet.nodeId = inst->UsbNodeList[inst->nUsbNodeIndex];
			inst->fbUsbNodeGet.pBuffer = (UDINT)&inst->UsbNodeInfo;
			inst->fbUsbNodeGet.bufferSize = sizeof(inst->UsbNodeInfo);
			UsbNodeGet(&inst->fbUsbNodeGet);
			if(inst->fbUsbNodeGet.status == 0)
			{
				// Neues Device hinzuf�gen
				memset(&inst->UsbDeviceListEntry, 0, sizeof(inst->UsbDeviceListEntry));
				inst->UsbDeviceListEntry.nNode = inst->UsbNodeList[inst->nUsbNodeIndex];
				strncpy((STRING*)&inst->UsbDeviceListEntry.sInterfaceName, (STRING*)&inst->UsbNodeInfo.ifName, sizeof(inst->UsbDeviceListEntry.sInterfaceName));
				strcpy((STRING*)&inst->UsbDeviceListEntry.sDeviceName, "BrbUsb_" );
				strcat((STRING*)&inst->UsbDeviceListEntry.sDeviceName, (STRING*)&inst->UsbDeviceListEntry.sInterfaceName);
				inst->nUsbDeviceIndex = BrbMemListIn(&inst->DeviceListMan, inst->DeviceListMan.nEntryCount, (UDINT)&inst->UsbDeviceListEntry);
				if(inst->bAutoLink == 1)
				{
					// Device muss gelinkt werden
					memset(&inst->sLinkParameter, 0, sizeof(inst->sLinkParameter));
					strcpy((STRING*)&inst->sLinkParameter, "/DEVICE=");
					strcat((STRING*)&inst->sLinkParameter, (STRING*)&inst->UsbDeviceList[inst->nUsbNodeIndex].sInterfaceName);
					inst->eStep = eSTEP_LINK_DEVICE;
				}
				else
				{
					// N�chster Node
					inst->nUsbNodeIndex += 1;
					inst->eStep = eSTEP_CHECK_LINK_DEVICES;
				}
			}
			else if(inst->fbUsbNodeGet.status != 65535)
			{
				// Fehler (sollte nicht auftreten)
				// N�chster Node
				inst->nUsbNodeIndex += 1;
				inst->eStep = eSTEP_CHECK_LINK_DEVICES;
			}
			break;
		
		case eSTEP_LINK_DEVICE:
			inst->fbDevLink.enable = 1;
			inst->fbDevLink.pDevice = (UDINT)&inst->UsbDeviceListEntry.sDeviceName;
			inst->fbDevLink.pParam = (UDINT)&inst->sLinkParameter;
			DevLink(&inst->fbDevLink);
			if(inst->fbDevLink.status == 0)
			{
				inst->UsbDeviceList[inst->nUsbDeviceIndex].nHandle = inst->fbDevLink.handle;
				// N�chster Node
				inst->nUsbNodeIndex += 1;
				inst->eStep = eSTEP_CHECK_LINK_DEVICES;
			}
			else if(inst->fbDevLink.status == fiERR_DEVICE_ALREADY_EXIST)
			{
				inst->UsbDeviceList[inst->nUsbNodeIndex].nHandle = inst->fbDevLink.handle;
				// N�chster Node
				inst->nUsbNodeIndex += 1;
				inst->eStep = eSTEP_CHECK_LINK_DEVICES;
			}
			else if(inst->fbDevLink.status != 65535)
			{
				inst->UsbDeviceList[inst->nUsbNodeIndex].nHandle = 0;
				// N�chster Node
				inst->nUsbNodeIndex += 1;
				inst->eStep = eSTEP_CHECK_LINK_DEVICES;
			}
			break;

	}
}
