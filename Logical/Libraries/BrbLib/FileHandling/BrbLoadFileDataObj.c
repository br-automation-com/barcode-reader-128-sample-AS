/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbLoadFileDataObj.c
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
	eSTEP_DEL_DATA_OBJ,
	eSTEP_OPEN_FILE,
	eSTEP_CREATE_DAT_OBJ,
	eSTEP_READ_FILE,
	eSTEP_CLOSE_FILE,
	eSTEP_ERR_CLOSE_FILE,
	eSTEP_ERR_DEL_DATA_OBJ,
};

/* Lädt eine Datei in ein Datenobjekt */
void BrbLoadFileDataObj(struct BrbLoadFileDataObj* inst)
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
			inst->nDataObjIdent = 0;
			inst->pDataObjMem = 0;
			inst->nDataObjLen = 0;
			memset(&inst->fbDatObjInfo, 0, sizeof(inst->fbDatObjInfo));
			memset(&inst->fbDatObjDelete, 0, sizeof(inst->fbDatObjDelete));
			memset(&inst->fbFileOpen, 0, sizeof(inst->fbFileOpen));
			memset(&inst->fbDatObjCreate, 0, sizeof(inst->fbDatObjCreate));
			memset(&inst->fbFileRead, 0, sizeof(inst->fbFileRead));
			memset(&inst->fbFileClose, 0, sizeof(inst->fbFileClose));
			inst->eStep = eSTEP_GET_DAT_OBJ;
			break;

		case eSTEP_GET_DAT_OBJ:
			// Evtl. unter diesem Namen schon vorhandenes Datenobjekt finden
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbDatObjInfo.enable = 1;
			inst->fbDatObjInfo.pName = (UDINT)inst->pDataObjName;
			DatObjInfo(&inst->fbDatObjInfo);
			if(inst->fbDatObjInfo.status == 0)
			{
				inst->eStep = eSTEP_DEL_DATA_OBJ;
			}
			else if(inst->fbDatObjInfo.status != 65535)
			{
				inst->eStep = eSTEP_OPEN_FILE;			
			}
			break;

		case eSTEP_DEL_DATA_OBJ:
			// Evtl. unter diesem Namen schon vorhandenes Datenobjekt löschen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbDatObjDelete.enable = 1;
			inst->fbDatObjDelete.ident = inst->fbDatObjInfo.ident;
			DatObjDelete(&inst->fbDatObjDelete);
			if(inst->fbDatObjDelete.status != 65535)
			{
				inst->eStep = eSTEP_OPEN_FILE;			
			}
			break;

		case eSTEP_OPEN_FILE:
			// Datei öffnen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileOpen.enable = 1;
			inst->fbFileOpen.pDevice = (UDINT)inst->pDevice;
			inst->fbFileOpen.pFile = (UDINT)inst->pFile;
			inst->fbFileOpen.mode = fiREAD_ONLY;
			FileOpen(&inst->fbFileOpen);
			if(inst->fbFileOpen.status == 0)
			{
				inst->eStep = eSTEP_CREATE_DAT_OBJ;
			}
			else if(inst->fbFileOpen.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern = inst->fbFileOpen.status;
				inst->nDataObjIdent = 0;
				inst->pDataObjMem = 0;
				inst->nDataObjLen = 0;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_CREATE_DAT_OBJ:
			// Datenobjekt von der Größe der Datei erzeugen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbDatObjCreate.enable = 1;
			inst->fbDatObjCreate.grp = 0;
			inst->fbDatObjCreate.pName = (UDINT)inst->pDataObjName;
			inst->fbDatObjCreate.len = inst->fbFileOpen.filelen;
			inst->fbDatObjCreate.MemType = inst->nDataObjMemType;
			inst->fbDatObjCreate.Option = inst->nDataObjOption;
			inst->fbDatObjCreate.pCpyData = 0;
			DatObjCreate(&inst->fbDatObjCreate);
			if(inst->fbDatObjCreate.status == 0)
			{
				memset((USINT*)inst->fbDatObjCreate.pDatObjMem, 0, inst->fbDatObjCreate.len);
				inst->nDataObjIdent = inst->fbDatObjCreate.ident;
				inst->pDataObjMem = inst->fbDatObjCreate.pDatObjMem;
				inst->nDataObjLen = inst->fbDatObjCreate.len;
				inst->eStep = eSTEP_READ_FILE;
			}
			else if(inst->fbDatObjCreate.status != 65535)
			{
				inst->nStatusIntern = inst->fbDatObjCreate.status;
				inst->eStep = eSTEP_ERR_CLOSE_FILE;
			}
			break;

		case eSTEP_READ_FILE:
			// Datei in das Datenpbjekt lesen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileRead.enable = 1;
			inst->fbFileRead.ident = inst->fbFileOpen.ident;
			inst->fbFileRead.offset = 0;
			inst->fbFileRead.pDest = inst->fbDatObjCreate.pDatObjMem;
			inst->fbFileRead.len = inst->fbDatObjCreate.len;
			FileRead(&inst->fbFileRead);
			if(inst->fbFileRead.status == 0)
			{
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			else if(inst->fbFileRead.status != 65535)
			{
				inst->nStatusIntern = inst->fbFileRead.status;
				inst->eStep = eSTEP_ERR_CLOSE_FILE;
			}
			break;

		case eSTEP_CLOSE_FILE:
			// Datei schliessen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileClose.enable = 1;
			inst->fbFileClose.ident = inst->fbFileOpen.ident;
			FileClose(&inst->fbFileClose);
			if(inst->fbFileClose.status == 0)
			{
				inst->nStatus = inst->nStatusIntern = 0;
				inst->eStep = eSTEP_INIT;
			}
			else if(inst->fbFileClose.status != 65535)
			{
				inst->nStatusIntern = inst->fbFileClose.status;
				inst->eStep = eSTEP_ERR_DEL_DATA_OBJ;
			}
			break;

		case eSTEP_ERR_CLOSE_FILE:
			// Datei im Fehlerfall schliessen
			inst->nStatus = 65535;
			inst->fbFileClose.enable = 1;
			inst->fbFileClose.ident = inst->fbFileOpen.ident;
			FileClose(&inst->fbFileClose);
			if(inst->fbFileClose.status != 65535)
			{
				if(inst->fbDatObjCreate.ident != 0)
				{
					inst->eStep = eSTEP_ERR_DEL_DATA_OBJ;
				}
				else
				{
					inst->nStatus = inst->nStatusIntern;
					inst->nDataObjIdent = 0;
					inst->pDataObjMem = 0;
					inst->nDataObjLen = 0;
					inst->eStep = eSTEP_INIT;
				}
			}
			break;

		case eSTEP_ERR_DEL_DATA_OBJ:
			// Datenobjekt im Fehlerfall löschen
			inst->nStatus = 65535;
			inst->fbDatObjDelete.enable = 1;
			inst->fbDatObjDelete.ident = inst->fbDatObjInfo.ident;
			DatObjDelete(&inst->fbDatObjDelete);
			if(inst->fbDatObjDelete.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern;
				inst->nDataObjIdent = 0;
				inst->pDataObjMem = 0;
				inst->nDataObjLen = 0;
				inst->eStep = eSTEP_INIT;
			}
			break;

	}
}
