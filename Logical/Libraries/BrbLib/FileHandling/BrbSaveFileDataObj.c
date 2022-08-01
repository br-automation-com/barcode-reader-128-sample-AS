/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSaveFileDataObj.c
 * Author: niedermeierr
 * Created: June 25, 2013
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
	eSTEP_GET_DAT_OBJ,
	eSTEP_GET_FILE,
	eSTEP_DEL_FILE,
	eSTEP_CREATE_FILE,
	eSTEP_WRITE_FILE,
	eSTEP_CLOSE_FILE,
	eSTEP_ERR_CLOSE_FILE,
};

/* Speichert ein Datenobjekt in eine Datei */
void BrbSaveFileDataObj(struct BrbSaveFileDataObj* inst)
{
	// Prüfung der Parameter
	if(inst->pDevice == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}
	if(inst->pFile == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}
	if(inst->pDataObjName == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}

	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = inst->nStatusIntern = 65535;
			memset(&inst->fbDatObjInfo, 0, sizeof(inst->fbDatObjInfo));
			memset(&inst->fbFileInfo, 0, sizeof(inst->fbFileInfo));
			memset(&inst->fbFileDelete, 0, sizeof(inst->fbFileDelete));
			memset(&inst->fbFileCreate, 0, sizeof(inst->fbFileCreate));
			memset(&inst->fbFileWrite, 0, sizeof(inst->fbFileWrite));
			memset(&inst->fbFileClose, 0, sizeof(inst->fbFileClose));
			inst->eStep = eSTEP_GET_DAT_OBJ;
			break;

		case eSTEP_GET_DAT_OBJ:
			// Prüfen, ob es das angegebene Datenobjekt gibt
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbDatObjInfo.enable = 1;
			inst->fbDatObjInfo.pName = (UDINT)inst->pDataObjName;
			DatObjInfo(&inst->fbDatObjInfo);
			if(inst->fbDatObjInfo.status == 0)
			{
				inst->eStep = eSTEP_GET_FILE;
			}
			else if(inst->fbDatObjInfo.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern = inst->fbDatObjInfo.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_GET_FILE:
			// Prüfen, ob es die Datei schon gibt
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileInfo.enable = 1;
			inst->fbFileInfo.pDevice = (UDINT)inst->pDevice;
			inst->fbFileInfo.pName = (UDINT)inst->pFile;
			inst->fbFileInfo.pInfo = (UDINT)&inst->FileInfo;
			FileInfo(&inst->fbFileInfo);
			if(inst->fbFileInfo.status == 0)
			{
				inst->eStep = eSTEP_DEL_FILE;
			}
			else if(inst->fbFileInfo.status != 65535)
			{
				inst->eStep = eSTEP_CREATE_FILE;
			}
			break;

		case eSTEP_DEL_FILE:
			// Evtl. schon vorhandene Datei löschen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileDelete.enable = 1;
			inst->fbFileDelete.pDevice = (UDINT)inst->pDevice;
			inst->fbFileDelete.pName = (UDINT)inst->pFile;
			FileDelete(&inst->fbFileDelete);
			if(inst->fbFileDelete.status != 65535)
			{
				inst->eStep = eSTEP_CREATE_FILE;
			}
			break;

		case eSTEP_CREATE_FILE:
			// Datei erzeugen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileCreate.enable = 1;
			inst->fbFileCreate.pDevice = (UDINT)inst->pDevice;
			inst->fbFileCreate.pFile = (UDINT)inst->pFile;
			FileCreate(&inst->fbFileCreate);
			if(inst->fbFileCreate.status == 0)
			{
				inst->eStep = eSTEP_WRITE_FILE;
			}
			else if(inst->fbFileCreate.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern = inst->fbFileCreate.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_WRITE_FILE:
			// Datenobjekt in die Datei schreiben
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileWrite.enable = 1;
			inst->fbFileWrite.ident = inst->fbFileCreate.ident;
			inst->fbFileWrite.offset = 0;
			inst->fbFileWrite.pSrc = inst->fbDatObjInfo.pDatObjMem;
			inst->fbFileWrite.len = inst->fbDatObjInfo.len;
			FileWrite(&inst->fbFileWrite);
			if(inst->fbFileWrite.status == 0)
			{
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			else if(inst->fbFileWrite.status != 65535)
			{
				inst->nStatusIntern = inst->fbFileWrite.status;
				inst->eStep = eSTEP_ERR_CLOSE_FILE;
			}
			break;

		case eSTEP_CLOSE_FILE:
			// Datei schliessen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileClose.enable = 1;
			inst->fbFileClose.ident = inst->fbFileCreate.ident;
			FileClose(&inst->fbFileClose);
			if(inst->fbFileClose.status == 0)
			{
				inst->nStatus = inst->nStatusIntern = 0;
				inst->eStep = eSTEP_INIT;
			}
			else if(inst->fbFileClose.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern = inst->fbFileClose.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_ERR_CLOSE_FILE:
			// Datei im Fehlerfall schliessen
			inst->nStatus = 65535;
			inst->fbFileClose.enable = 1;
			inst->fbFileClose.ident = inst->fbFileCreate.ident;
			FileClose(&inst->fbFileClose);
			if(inst->fbFileClose.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern;
				inst->eStep = eSTEP_INIT;
			}
			break;

	}
}
