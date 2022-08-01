
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
	eSTEP_OPEN_FILE,
	eSTEP_CHECK_SIZE,
	eSTEP_READ_FILE,
	eSTEP_CLOSE_FILE,
	eSTEP_ERR_CLOSE_FILE,
};

/* Lädt eine Datei in eine Feld-Variable */
void BrbLoadFileBin(struct BrbLoadFileBin* inst)
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
	if(inst->pVar == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}

	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = inst->nStatusIntern = 65535;
			memset((USINT*)inst->pVar, 0, inst->nVarSize);
			inst->nValidBytes = 0;
			memset(&inst->fbFileOpen, 0, sizeof(inst->fbFileOpen));
			memset(&inst->fbFileReadEx, 0, sizeof(inst->fbFileReadEx));
			memset(&inst->fbFileClose, 0, sizeof(inst->fbFileClose));
			inst->eStep = eSTEP_OPEN_FILE;
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
				inst->eStep = eSTEP_CHECK_SIZE;
			}
			else if(inst->fbFileOpen.status != 65535)
			{
				inst->nStatus = inst->nStatusIntern = inst->fbFileOpen.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_CHECK_SIZE:
			// Datei- und Variablen-Größe prüfen
			if(inst->nVarSize >= inst->fbFileOpen.filelen)
			{
				inst->eStep = eSTEP_READ_FILE;
			}
			else
			{
				inst->nStatusIntern = fiERR_DATA_SIZE; // = 20701
				inst->eStep = eSTEP_ERR_CLOSE_FILE;
			}
			break;

		case eSTEP_READ_FILE:
			// Datei in das Datenpbjekt lesen
			inst->nStatus = inst->nStatusIntern = 65535;
			inst->fbFileReadEx.enable = 1;
			inst->fbFileReadEx.ident = inst->fbFileOpen.ident;
			inst->fbFileReadEx.offset = 0;
			inst->fbFileReadEx.pDest = (UDINT)inst->pVar;
			inst->fbFileReadEx.len = inst->fbFileOpen.filelen;
			FileReadEx(&inst->fbFileReadEx);
			if(inst->fbFileReadEx.status == 0)
			{
				inst->nValidBytes = inst->fbFileReadEx.bytesread;
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			else if(inst->fbFileReadEx.status != 65535)
			{
				inst->nStatusIntern = inst->fbFileReadEx.status;
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
				inst->eStep = eSTEP_INIT;
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
				inst->nStatus = inst->nStatusIntern;
				inst->eStep = eSTEP_INIT;
			}
			break;

	}
}
