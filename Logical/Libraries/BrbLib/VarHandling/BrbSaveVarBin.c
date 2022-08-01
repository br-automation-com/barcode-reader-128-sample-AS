/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSaveVarBin.c
 * Author: niedermeierr
 * Created: June 11, 2013
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

#ifdef __cplusplus
	};
#endif

// Eigene Enums und Typen
enum Steps_ENUM
{
	eSTEP_INIT,
	eSTEP_GET_PV_INFO,
	eSTEP_DELETE_FILE,
	eSTEP_CREATE_FILE,
	eSTEP_WRITE_FILE,
	eSTEP_CLOSE_FILE,
};

/* Speichert eine Variable mit allen Elementen in einer Binär-Datei */
void BrbSaveVarBin(struct BrbSaveVarBin* inst)
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
	if(inst->pVarName == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}

	UINT	nFuncStatus;
	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = 65535;
			inst->nStatusIntern = 0;
			memset(&inst->PvInfo, 0, sizeof(inst->PvInfo));
			memset(&inst->fbFileDelete, 0, sizeof(inst->fbFileDelete));
			memset(&inst->fbFileCreate, 0, sizeof(inst->fbFileCreate));
			memset(&inst->fbFileWrite, 0, sizeof(inst->fbFileWrite));
			memset(&inst->fbFileClose, 0, sizeof(inst->fbFileClose));
			inst->eStep = eSTEP_GET_PV_INFO;
			break;

		case eSTEP_GET_PV_INFO:
			// Informationen der Variablen holen
			inst->nStatus = 65535;
			// Adresse der Variable holen
			nFuncStatus = PV_xgetadr((STRING*)inst->pVarName, &inst->PvInfo.nAdr, &inst->PvInfo.nLen);
			if(nFuncStatus == 0)
			{
				// Größe der Variable holen
				nFuncStatus = PV_ninfo((STRING*)inst->pVarName, &inst->PvInfo.nDataType, &inst->PvInfo.nLen, &inst->PvInfo.nDimension);
				if(nFuncStatus == 0)
				{
					inst->eStep = eSTEP_DELETE_FILE;
				}
				else
				{
					inst->nStatus = nFuncStatus;
					inst->eStep = eSTEP_INIT;
				}
			}
			else
			{
				inst->nStatus = nFuncStatus;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_DELETE_FILE:
			// Evtl. vorhandene Datei löschen
			inst->nStatus = 65535;
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
			// Datei neu erzeugen
			inst->nStatus = 65535;
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
				inst->nStatus = inst->fbFileCreate.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_WRITE_FILE:
			// Speicher der Variable in die Datei schreiben
			inst->nStatus = 65535;
			inst->fbFileWrite.enable = 1;
			inst->fbFileWrite.ident = inst->fbFileCreate.ident;
			inst->fbFileWrite.offset = 0;
			inst->fbFileWrite.pSrc = (UDINT)inst->PvInfo.nAdr;
			inst->fbFileWrite.len = inst->PvInfo.nLen;
			FileWrite(&inst->fbFileWrite);
			if(inst->fbFileWrite.status == 0)
			{
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			else if(inst->fbFileWrite.status != 65535)
			{
				// Fehler merken
				inst->nStatusIntern = inst->fbFileWrite.status;
				// Datei muß auf jeden Fall geschlossen werden
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			break;

		case eSTEP_CLOSE_FILE:
			// Datei schliessen
			inst->nStatus = 65535;
			inst->fbFileClose.enable = 1;
			inst->fbFileClose.ident = inst->fbFileCreate.ident;
			FileClose(&inst->fbFileClose);
			if(inst->fbFileClose.status == 0)
			{
				// Prüfen, ob vorher schon ein Fehler aufgetreten ist
				if(inst->nStatusIntern > 0)
				{
					inst->nStatus = inst->nStatusIntern;
					inst->eStep = eSTEP_INIT;
				}
				else
				{
					inst->nStatus = 0;
					inst->eStep = eSTEP_INIT;
				}
			}
			else if(inst->fbFileClose.status != 65535)
			{
				// Prüfen, ob vorher schon ein Fehler aufgetreten ist
				if(inst->nStatusIntern > 0)
				{
					inst->nStatus = inst->nStatusIntern;
					inst->eStep = eSTEP_INIT;
				}
				else
				{
					inst->nStatus = inst->fbFileClose.status;
					inst->eStep = eSTEP_INIT;
				}
			}
			break;

	}
}
