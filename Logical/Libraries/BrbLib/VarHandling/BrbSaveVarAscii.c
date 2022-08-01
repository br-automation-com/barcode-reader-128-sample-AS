/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSaveVarAscii.c
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
#include <AnsiCFunc.h>

#ifdef __cplusplus
	};
#endif

// Eigene Enums und Typen
enum Steps_ENUM
{
	eSTEP_INIT,
	eSTEP_DELETE_FILE,
	eSTEP_CREATE_FILE,
	eSTEP_GET_HEADER,
	eSTEP_SET_ROOT_PV,
	eSTEP_GET_LINES,
	eSTEP_WRITE_LINES,
	eSTEP_CLOSE_FILE
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
void GetValue(BrbPvInfo_TYP* pPvInfo, STRING* pValue);

/* Speichert eine Variable mit allen Elementen in einer Ascii-Datei */
void BrbSaveVarAscii(struct BrbSaveVarAscii* inst)
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
	STRING sValue[1000];
	STRING sName[40];
	STRING sIndex[20];

	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = 65535;
			inst->nStatusIntern = 0;
			memset(&inst->fbFileDelete, 0, sizeof(inst->fbFileDelete));
			memset(&inst->fbFileCreate, 0, sizeof(inst->fbFileCreate));
			inst->nOffset = 0;
			inst->nLineCountWrite = 0;
			inst->nCharCountMaxPerWrite = 0;
			memset(&inst->fbFileWrite, 0, sizeof(inst->fbFileWrite));
			memset(&inst->fbFileClose, 0, sizeof(inst->fbFileClose));
			if(inst->nLinesToWriteAtOneStep == 0)
			{
				// Wenn keine Anzahl angegeben ist, einen Defaultwert verwenden
				inst->nLinesToWriteAtOneStep = 10;
			}
			inst->bVarFinished = 0;
			inst->eStep = eSTEP_DELETE_FILE;
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
				inst->eStep = eSTEP_GET_HEADER;
			}
			else if(inst->fbFileCreate.status != 65535)
			{
				inst->nStatus = inst->fbFileCreate.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_GET_HEADER:
			// Header erzeugen
			inst->nStatus = 65535;
			memset(&inst->sLines, 0, sizeof(inst->sLines));
			strcpy(inst->sLines, "// Parameterfile '");
			strcat(inst->sLines, (STRING*)inst->pFile);
			strcat(inst->sLines, "'\r\n");
			inst->nLineCountWrite += 1;
			strcat(inst->sLines, "// Variable: '");
			strcat(inst->sLines, (STRING*)inst->pVarName);
			strcat(inst->sLines, "'\r\n");
			inst->nLineCountWrite += 1;
			BrbGetCurrentTimeText(inst->sHelp, sizeof(inst->sHelp), "yyyy.mm.dd hh:MM:ss");
			strcat(inst->sLines, "// Date: ");
			strcat(inst->sLines, (STRING*)inst->sHelp);
			strcat(inst->sLines, "\r\n");
			inst->nLineCountWrite += 1;
			inst->eStep = eSTEP_SET_ROOT_PV;
			break;

		case eSTEP_SET_ROOT_PV:
			// Informationen der Basis-Pv holen
			inst->nStatus = 65535;
			memset(&inst->PvInfo, 0, sizeof(inst->PvInfo));
			inst->nPvLevel = 0;
			strcpy(inst->PvInfo[inst->nPvLevel].sName, (STRING*)inst->pVarName);
			inst->eStep = eSTEP_GET_LINES;
			break;

		case eSTEP_GET_LINES:
			// Die Zeilen der Elemente erzeugen
			inst->nStatus = 65535;
			do
			{
				// Prüfen, ob die Information des aktuellen Elements schon geholt wurden
				nFuncStatus = 0;
				if(inst->PvInfo[inst->nPvLevel].nLen == 0)
				{
					// Information des aktuellen Elements holen
					nFuncStatus = PV_ninfo((STRING*)&inst->PvInfo[inst->nPvLevel].sName, &inst->PvInfo[inst->nPvLevel].nDataType, &inst->PvInfo[inst->nPvLevel].nLen, &inst->PvInfo[inst->nPvLevel].nDimension);
				}
				if(nFuncStatus != 0)
				{
					inst->nStatusIntern = nFuncStatus;
					inst->bVarFinished = 1;
				}
				else
				{
					// Datentyp und Feldgröße prüfen
					if(inst->PvInfo[inst->nPvLevel].nDataType == eDATATYPE_STRUCT_ARRAY)
					{
						// Struktur-Array
						if(inst->PvInfo[inst->nPvLevel].nItemIndex < inst->PvInfo[inst->nPvLevel].nDimension)
						{
							// Nächstes Level
							inst->nPvLevel += 1;
							memset(&inst->PvInfo[inst->nPvLevel], 0, sizeof(inst->PvInfo[inst->nPvLevel]));
							// Namen zusammensetzen
							// "Root.StructArray.StructArray[x]"
							strcat(inst->PvInfo[inst->nPvLevel].sName, inst->PvInfo[inst->nPvLevel-1].sName);
							strcat(inst->PvInfo[inst->nPvLevel].sName, "[");
							brsitoa((DINT)inst->PvInfo[inst->nPvLevel-1].nItemIndex, (UDINT)&sIndex);
							strcat(inst->PvInfo[inst->nPvLevel].sName, sIndex);
							strcat(inst->PvInfo[inst->nPvLevel].sName, "]");
						}
						else
						{
							// Array-Dimension erreicht
							if(inst->nPvLevel > 0)
							{
								// Vorheriges Level und nächstes Element
								inst->nPvLevel -= 1;
								inst->PvInfo[inst->nPvLevel].nItemIndex += 1;
							}
							else
							{
								// Alle Elemente der Basis-Pv durchlaufen
								inst->bVarFinished = 1;
							}					
						}
					}
					else if(inst->PvInfo[inst->nPvLevel].nDataType == eDATATYPE_STRUCT)
					{
						// Einfache Struktur -> Element-Informationen holen
						nFuncStatus = PV_item(inst->PvInfo[inst->nPvLevel].sName, inst->PvInfo[inst->nPvLevel].nItemIndex, sName);
						if(nFuncStatus == 0)
						{
							// Element gültig -> Nächstes Level
							inst->nPvLevel += 1;
							memset(&inst->PvInfo[inst->nPvLevel], 0, sizeof(inst->PvInfo[inst->nPvLevel]));
							// Namen zusammensetzen
							// "Root.Element"
							strcat(inst->PvInfo[inst->nPvLevel].sName, inst->PvInfo[inst->nPvLevel-1].sName);
							strcat(inst->PvInfo[inst->nPvLevel].sName, ".");
							strcat(inst->PvInfo[inst->nPvLevel].sName, sName);
						}
						else
						{
							// Keine weiteren Elemente
							if(inst->nPvLevel > 0)
							{
								// Vorheriges Level und nächstes Element
								memset(&inst->PvInfo[inst->nPvLevel], 0, sizeof(inst->PvInfo[inst->nPvLevel]));
								inst->nPvLevel -= 1;
								inst->PvInfo[inst->nPvLevel].nItemIndex += 1;
							}
							else
							{
								// Alle Elemente der Basis-Pv durchlaufen
								inst->bVarFinished = 1;
							}
						}
					}
					else
					{
						// Einfacher Datentyp
						if(inst->PvInfo[inst->nPvLevel].nDimension > 1)
						{
							// Einfaches Array
							// Prüfen, ob schon alle Felder durchlaufen sind
							if(inst->PvInfo[inst->nPvLevel].nItemIndex < inst->PvInfo[inst->nPvLevel].nDimension)
							{
								// Noch Felder übrig -> Nächstes Level
								inst->nPvLevel += 1;
								memset(&inst->PvInfo[inst->nPvLevel], 0, sizeof(inst->PvInfo[inst->nPvLevel]));
								// Namen zusammensetzen
								// "Root.Array.Array[x]"							
								strcat(inst->PvInfo[inst->nPvLevel].sName, inst->PvInfo[inst->nPvLevel-1].sName);
								strcat(inst->PvInfo[inst->nPvLevel].sName, "[");
								brsitoa((DINT)inst->PvInfo[inst->nPvLevel-1].nItemIndex, (UDINT)&sIndex);
								strcat(inst->PvInfo[inst->nPvLevel].sName, sIndex);
								strcat(inst->PvInfo[inst->nPvLevel].sName, "]");
							}
							else
							{
								// Kein Feld übrig
								if(inst->nPvLevel > 0)
								{
									// Vorheriges Level und nächstes Element
									memset(&inst->PvInfo[inst->nPvLevel], 0, sizeof(inst->PvInfo[inst->nPvLevel]));
									inst->nPvLevel -= 1;
									inst->PvInfo[inst->nPvLevel].nItemIndex += 1;
								}
								else
								{
									// Alle Elemente der Basis-Pv durchlaufen
									inst->bVarFinished = 1;
								}					
							}
						}
						else
						{
							// Einfacher Datentyp -> Zeile erzeugen
							// "PvName=Wert"
							strcat(inst->sLines, inst->PvInfo[inst->nPvLevel].sName);
							strcat(inst->sLines, "=");
							memset(&sValue, 0, sizeof(sValue));
							GetValue(&inst->PvInfo[inst->nPvLevel], sValue);
							strcat(inst->sLines, sValue);
							strcat(inst->sLines, "\r\n");
							inst->nLineCountWrite += 1;
							if(inst->nPvLevel > 0)
							{
								// Vorheriges Level und nächstes Element
								memset(&inst->PvInfo[inst->nPvLevel], 0, sizeof(inst->PvInfo[inst->nPvLevel]));
								inst->nPvLevel -= 1;
								inst->PvInfo[inst->nPvLevel].nItemIndex += 1;
							}
							else
							{
								// Nächstes Element
								inst->PvInfo[inst->nPvLevel].nItemIndex += 1;
							}
						}
					}
				}
			} while((inst->bVarFinished == 0) && (inst->nLineCountWrite < inst->nLinesToWriteAtOneStep));
			inst->eStep = eSTEP_WRITE_LINES;
			break;

		case eSTEP_WRITE_LINES:
			// Bisher erzeugte Zeilen schreiben
			inst->nStatus = 65535;
			inst->fbFileWrite.enable = 1;
			inst->fbFileWrite.ident = inst->fbFileCreate.ident;
			inst->fbFileWrite.offset = inst->nOffset;
			inst->fbFileWrite.pSrc = (UDINT)&inst->sLines;
			inst->fbFileWrite.len = strlen((STRING*)&inst->sLines);
			FileWrite(&inst->fbFileWrite);
			if(inst->fbFileWrite.status == 0)
			{
				inst->nOffset += inst->fbFileWrite.len;
				memset(&inst->sLines, 0, sizeof(inst->sLines));
				inst->nLineCountWrite = 0;
				// Prüfen, ob noch Zeilen zu erzeugen sind
				if(inst->bVarFinished == 0)
				{
					inst->eStep = eSTEP_GET_LINES;
				}
				else
				{
					inst->eStep = eSTEP_CLOSE_FILE;
				}
			}
			else if(inst->fbFileWrite.status != 65535)
			{
				// Fehler merken
				if(inst->nStatusIntern == 0)
				{
					inst->nStatusIntern = inst->fbFileWrite.status;
				}
				// Datei muß auf jeden Fall geschlossen werden
				inst->eStep = eSTEP_CLOSE_FILE;
			}
			// Den größten Wert für die Zeichen pro Zeile ermitteln
			if(inst->nCharCountMaxPerWrite < inst->fbFileWrite.len)
			{
				inst->nCharCountMaxPerWrite = inst->fbFileWrite.len;
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

// Gibt den Wert einer Pv als String zurück.
void GetValue(BrbPvInfo_TYP* pPvInfo, STRING* pValue)
{
	UINT	nFuncStatus;
	STRING sItoA[20];
	memset(sItoA, 0, sizeof(sItoA));
	// Adresse der Variable holen
	nFuncStatus = PV_xgetadr(pPvInfo->sName, &pPvInfo->nAdr, &pPvInfo->nLen);
	if(nFuncStatus == 0)
	{
		// Wert aufgrund des Datentyps an der Adresse holen und in String wandeln
		switch(pPvInfo->nDataType)
		{
			case eDATATYPE_BOOL:
				{
					BOOL bValue;
					bValue = *(BOOL*)pPvInfo->nAdr;
					brsitoa((DINT)bValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_SINT:
				{
					SINT nValue;
					nValue = *(SINT*)pPvInfo->nAdr;
					brsitoa((DINT)nValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_INT:
				{
					INT nValue;
					nValue = *(INT*)pPvInfo->nAdr;
					brsitoa((DINT)nValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_DINT:
				{
					DINT nValue;
					nValue = *(DINT*)pPvInfo->nAdr;
					brsitoa((DINT)nValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_USINT:
				{
					USINT nValue;
					nValue = *(USINT*)pPvInfo->nAdr;
					brsitoa((DINT)nValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_UINT:
				{
					UINT nValue;
					nValue = *(UINT*)pPvInfo->nAdr;
					brsitoa((DINT)nValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_UDINT:
				{
					UDINT nValue;
					nValue = *(UDINT*)pPvInfo->nAdr;
					BrbUdintToAscii(nValue, sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_REAL:
				{
					REAL rValue;
					rValue = *(REAL*)pPvInfo->nAdr;
					brsftoa(rValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_STRING:
				{
					strcpy(pValue, (STRING*)pPvInfo->nAdr);
				}
				break;

			case eDATATYPE_DATE_AND_TIME:
				{
					UDINT nValue;
					nValue = *(UDINT*)pPvInfo->nAdr;
					BrbUdintToAscii(nValue, sItoA);
					strcpy(pValue, sItoA);
				}
				break;

			case eDATATYPE_TIME:
				{
					DINT nValue;
					nValue = *(DINT*)pPvInfo->nAdr;
					brsitoa((DINT)nValue, (UDINT)&sItoA);
					strcpy(pValue, sItoA);
				}
				break;

		}
	}
}
