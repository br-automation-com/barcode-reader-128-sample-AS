/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbLoadVarAscii.c
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
	eSTEP_GET_DATAOBJ_INFO,
	eSTEP_DELETE_DATAOBJ_START,
	eSTEP_OPEN_FILE,
	eSTEP_CREATE_DATAOBJ,
	eSTEP_READ_FILE,
	eSTEP_CLOSE_FILE,
	eSTEP_READ_LINES,
	eSTEP_DELETE_DATAOBJ_END,
};

enum Datatypes_ENUM
{
	eDATATYPE_STRUCT,
	eDATATYPE_BOOL,
	eDATATYPE_SINT,
	eDATATYPE_INT,
	eDATATYPE_DINT,
	eDATATYPE_USINT,
	eDATATYPE_UINT,
	eDATATYPE_UDINT,
	eDATATYPE_REAL,
	eDATATYPE_STRING,
	eDATATYPE_DATE_AND_TIME = 11,
	eDATATYPE_TIME,
	eDATATYPE_STRUCT_ARRAY = 15
};

// Prototypen
STRING* SetToNextLine(STRING* pText, STRING* pTextEnd);
STRING* ReadPvName(STRING* pText, STRING* pPvName, STRING* pTextEnd);
STRING* ReadPvValue(STRING* pText, STRING* pPvValue, STRING* pTextEnd);
void SetValue(BrbPvInfo_TYP* pPvInfo, STRING* pValue);

/* Lädt eine Ascii-Datei mit Variablen-Werten */
void BrbLoadVarAscii(struct BrbLoadVarAscii* inst)
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

	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = 65535;
			inst->nStatusIntern = 0;
			memset(&inst->fbDatObjInfo, 0, sizeof(inst->fbDatObjInfo));
			memset(&inst->fbDatObjDelete, 0, sizeof(inst->fbDatObjDelete));
			memset(&inst->fbFileOpen, 0, sizeof(inst->fbFileOpen));
			memset(&inst->fbDatObjCreate, 0, sizeof(inst->fbDatObjCreate));
			if(inst->nLinesToReadAtOneStep == 0)
			{
				// Wenn keine Anzahl angegeben ist, einen Defaultwert verwenden
				inst->nLinesToReadAtOneStep = 10;
			}
			inst->pTextStart = 0;
			inst->pTextEnd = 0;
			inst->pText = 0;
			inst->nLineCountRead = 0;
			memset(&inst->fbFileRead, 0, sizeof(inst->fbFileRead));
			memset(&inst->fbFileClose, 0, sizeof(inst->fbFileClose));
			inst->eStep = eSTEP_GET_DATAOBJ_INFO;
			break;

		case eSTEP_GET_DATAOBJ_INFO:
			// Prüfen, ob noch ein altes Datenobjekt vorhanden ist
			inst->nStatus = 65535;
			inst->fbDatObjInfo.enable = 1;
			inst->fbDatObjInfo.pName = (UDINT)&"LdPv123";
			DatObjInfo(&inst->fbDatObjInfo);
			if(inst->fbDatObjInfo.status == 0)
			{
				// Vorhanden -> löschen
				inst->eStep = eSTEP_DELETE_DATAOBJ_START;
			}
			else if(inst->fbDatObjInfo.status != 65535)
			{
				// Nicht vorhanden -> Datei öffnen
				inst->eStep = eSTEP_OPEN_FILE;
			}
			break;

		case eSTEP_DELETE_DATAOBJ_START:
			// Evtl. noch vorhandenes Datenobjekt löschen
			inst->nStatus = 65535;
			inst->fbDatObjDelete.enable = 1;
			inst->fbDatObjDelete.ident = inst->fbDatObjCreate.ident;
			DatObjDelete(&inst->fbDatObjDelete);
			if(inst->fbDatObjDelete.status == 0)
			{
				inst->eStep = eSTEP_OPEN_FILE;
			}
			else if(inst->fbDatObjDelete.status != 65535)
			{
				inst->nStatus = inst->fbFileOpen.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_OPEN_FILE:
			// Datei öffnen
			inst->nStatus = 65535;
			inst->fbFileOpen.enable = 1;
			inst->fbFileOpen.pDevice = (UDINT)inst->pDevice;
			inst->fbFileOpen.pFile = (UDINT)inst->pFile;
			FileOpen(&inst->fbFileOpen);
			if(inst->fbFileOpen.status == 0)
			{
				inst->eStep = eSTEP_CREATE_DATAOBJ;
			}
			else if(inst->fbFileOpen.status != 65535)
			{
				inst->nStatus = inst->fbFileOpen.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_CREATE_DATAOBJ:
			// Datenobjekt mit der Größe der Datei erzeugen
			inst->nStatus = 65535;
			inst->fbDatObjCreate.enable = 1;
			inst->fbDatObjCreate.grp = 0;
			inst->fbDatObjCreate.pName = (UDINT)&"LdPv123";
			inst->fbDatObjCreate.len = inst->fbFileOpen.filelen;
			inst->fbDatObjCreate.MemType = doTEMP;
			inst->fbDatObjCreate.Option = doNO_CS;
			inst->fbDatObjCreate.pCpyData = 0;
			DatObjCreate(&inst->fbDatObjCreate);
			if(inst->fbDatObjCreate.status == 0)
			{
				inst->pTextStart = (STRING*)inst->fbDatObjCreate.pDatObjMem;
				inst->pTextEnd = inst->pTextStart + inst->fbDatObjCreate.len;
				inst->pText = inst->pTextStart;
				inst->eStep = eSTEP_READ_FILE;
			}
			else if(inst->fbDatObjCreate.status != 65535)
			{
				// Fehler merken 
				inst->nStatusIntern = inst->fbDatObjCreate.status;
				// Datei muß auf jeden Fall geschlossen werden
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			break;

		case eSTEP_READ_FILE:
			// Datei in das Datenobjekt einlesen
			inst->nStatus = 65535;
			inst->fbFileRead.enable = 1;
			inst->fbFileRead.ident = inst->fbFileOpen.ident;
			inst->fbFileRead.offset = 0;
			inst->fbFileRead.pDest = inst->fbDatObjCreate.pDatObjMem;
			inst->fbFileRead.len = inst->fbFileOpen.filelen;
			FileRead(&inst->fbFileRead);
			if(inst->fbFileRead.status == 0)
			{
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			else if(inst->fbFileRead.status != 65535)
			{
				// Fehler merken 
				inst->nStatusIntern = inst->fbFileRead.status;
				// Datei muß auf jeden Fall geschlossen werden
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			break;

		case eSTEP_CLOSE_FILE:
			// Datei schliessen
			inst->nStatus = 65535;
			inst->fbFileClose.enable = 1;
			inst->fbFileClose.ident = inst->fbFileOpen.ident;
			FileClose(&inst->fbFileClose);
			if(inst->fbFileClose.status == 0)
			{
				// Prüfen, ob vorher schon ein Fehler aufgetreten ist
				if(inst->nStatusIntern > 0)
				{
					// Datenobjekt muß auf jeden Fall gelöscht werden
					inst->eStep = eSTEP_DELETE_DATAOBJ_END;
				}
				else
				{
					inst->eStep = eSTEP_READ_LINES;
				}
			}
			else if(inst->fbFileClose.status != 65535)
			{
				// Prüfen, ob vorher schon ein Fehler aufgetreten ist
				if(inst->nStatusIntern > 0)
				{
					// Datenobjekt muß auf jeden Fall gelöscht werden
					inst->eStep = eSTEP_DELETE_DATAOBJ_END;
				}
				else
				{
					// Fehler merken
					inst->nStatusIntern = inst->fbFileClose.status;
					// Datenobjekt muß auf jeden Fall gelöscht werden
					inst->eStep = eSTEP_DELETE_DATAOBJ_END;
				}
			}
			break;

		case eSTEP_READ_LINES:
			inst->nStatus = 65535;
			inst->nLineCountRead = 0;
			do
			{
				if(BrbStringStartsWith(inst->pText, "//"))
				{
					// Kommentare werden überlesen
				}
				else
				{
					// Variable lesen
					BrbPvInfo_TYP PvInfo;
					memset(&PvInfo, 0, sizeof(PvInfo));
					STRING sPvValue[nBRB_PVNAME_CHAR_MAX];
					memset(&sPvValue, 0, sizeof(sPvValue));
					// Namen lesen
					inst->pText = ReadPvName(inst->pText, PvInfo.sName, inst->pTextEnd);
					if(inst->pText != 0)
					{
						// Wert lesen
						inst->pText = ReadPvValue(inst->pText, sPvValue, inst->pTextEnd);
						if(inst->pText != 0)
						{
							SetValue(&PvInfo, sPvValue);
						}
					}
				}
				// Auf den Anfang der nächsten Zeile springen
				inst->pText = SetToNextLine(inst->pText, inst->pTextEnd);
				inst->nLineCountRead += 1;
				if((inst->pText >= inst->pTextEnd) || (inst->pText == 0))
				{
					// Ende der Datei erreicht -> Schleife verlassen
					inst->nLineCountRead = inst->nLinesToReadAtOneStep + 1;
					inst->eStep = eSTEP_DELETE_DATAOBJ_END;
					break;
				}
			}
			while(inst->nLineCountRead <= inst->nLinesToReadAtOneStep);
			break;

		case eSTEP_DELETE_DATAOBJ_END:
			// Datenobjekt löschen
			inst->nStatus = 65535;
			inst->fbDatObjDelete.enable = 1;
			inst->fbDatObjDelete.ident = inst->fbDatObjCreate.ident;
			DatObjDelete(&inst->fbDatObjDelete);
			if(inst->fbDatObjDelete.status != 65535)
			{
				if(inst->nStatusIntern > 0)
				{
					inst->nStatus = inst->nStatusIntern;
				}
				else
				{
					inst->nStatus = 0;
				}
				inst->eStep = eSTEP_INIT;
			}
			break;

	}
}

// Gibt die Adresse nach dem nächsten CrLf zurück.
// 0, wenn es nicht gefunden wird.
STRING* SetToNextLine(STRING* pText, STRING* pTextEnd)
{
	STRING* pResult = 0;
	if(pText != 0)
	{
		// Nach CrLf suchen
		pResult = BrbStringGetAdrOf(pText, "\r\n", pTextEnd - pText);
		if(pResult >= pText)
		{
			// Gefunden -> CrLf überspringen
			pResult += 2;
		}
	}
	return pResult;
}

// Liest den Text bis zum nächsten = und gibt die Adresse danach zurück.
// 0, wenn es nicht gefunden wird.
STRING* ReadPvName(STRING* pText, STRING* pPvName, STRING* pTextEnd)
{
	STRING* pResult = 0;
	// Nach = suchen
	pResult = BrbStringGetAdrOf(pText, "=", pTextEnd - pText);
	if(pResult >= pText)
	{
		// Gefunden -> Überspringen
		memcpy(pPvName, pText, pResult - pText);
		pResult += 1;
	}
	return pResult;
}

// Liest den Text bis zum nächsten CrLf und gibt die Adresse zurück.
// 0, wenn es nicht gefunden wird.
STRING* ReadPvValue(STRING* pText, STRING* pPvValue, STRING* pTextEnd)
{
	STRING* pResult = 0;
	// Nach CrLf suchen
	pResult = BrbStringGetAdrOf(pText, "\r\n", pTextEnd - pText);
	if(pResult >= pText)
	{
		
		// Gefunden -> Länge des Werts prüfen 
		UDINT nLenValue = pResult - pText;
		if(nLenValue >= nBRB_PVNAME_CHAR_MAX)
		{
			// Länge begrenzen
			nLenValue = nBRB_PVNAME_CHAR_MAX - 1;
		}
		// Wert kopieren
		memcpy(pPvValue, pText, nLenValue);
		// CrLf nicht überspringen (wird zum Erkennen der nächsten Zeile gebraucht)
	}
	return pResult;
}

// Setzt den Wert einer Pv als String.
void SetValue(BrbPvInfo_TYP* pPvInfo, STRING* pValue)
{
	UINT	nFuncStatus;
	UDINT nLen;
	STRING sItoA[20];
	memset(sItoA, 0, sizeof(sItoA));
	// Adresse der Variablen holen
	nFuncStatus = PV_xgetadr(pPvInfo->sName, &pPvInfo->nAdr, &nLen);
	if(nFuncStatus == 0)
	{
		// Datentyp der Variable holen
		nFuncStatus = PV_ninfo(pPvInfo->sName, &pPvInfo->nDataType, &pPvInfo->nLen, &pPvInfo->nDimension);
		if(nFuncStatus == 0)
		{
			// Wert in Datentyp wandeln und an der Adresse setzen
			switch(pPvInfo->nDataType)
			{
				case eDATATYPE_BOOL:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(BOOL*)pPvInfo->nAdr = (BOOL)nValue;
					}
					break;

				case eDATATYPE_SINT:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(SINT*)pPvInfo->nAdr = (SINT)nValue;
					}
					break;

				case eDATATYPE_INT:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(INT*)pPvInfo->nAdr = (INT)nValue;
					}
					break;

				case eDATATYPE_DINT:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(DINT*)pPvInfo->nAdr = (DINT)nValue;
					}
					break;

				case eDATATYPE_USINT:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(USINT*)pPvInfo->nAdr = (USINT)nValue;
					}
					break;

				case eDATATYPE_UINT:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(UINT*)pPvInfo->nAdr = (UINT)nValue;
					}
					break;

				case eDATATYPE_UDINT:
					{
						UDINT nValue = BrbAsciiToUdint(pValue);
						*(UDINT*)pPvInfo->nAdr = (UDINT)nValue;
					}
					break;

				case eDATATYPE_REAL:
					{
						REAL rValue = brsatof((UDINT)pValue);
						*(REAL*)pPvInfo->nAdr = (REAL)rValue;
					}
					break;

				case eDATATYPE_STRING:
					{
						strcpy((STRING*)pPvInfo->nAdr, pValue);
					}
					break;

				case eDATATYPE_DATE_AND_TIME:
					{
						UDINT nValue = BrbAsciiToUdint(pValue);
						*(UDINT*)pPvInfo->nAdr = (UDINT)nValue;
					}
					break;

				case eDATATYPE_TIME:
					{
						DINT nValue = brsatoi((UDINT)pValue);
						*(DINT*)pPvInfo->nAdr = (DINT)nValue;
					}
					break;

			}
		}
	}
}
