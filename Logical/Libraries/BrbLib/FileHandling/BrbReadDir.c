/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbReadDir.c
 * Author: niedermeierr
 * Created: June 17, 2013
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
	eSTEP_GET_FILTER,
	eSTEP_READ_DIR,
	eSTEP_SORT,
};

// Prototypen
BOOL CallUserFilterFunction(struct BrbReadDir* inst, fiDIR_READ_DATA* pFileInfo);
void SortList(struct BrbReadDir* inst);
INT SortTwoEntries(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2, BrbFileSorting_ENUM eSorting, BOOL bCaseSensitive, UDINT pUserCompareFunction);
INT FileComparerNone(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerAlphUp(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerAlphUpCaseSensitive(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerAlphDown(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerAlphDownCaseSensitive(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerOldest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerYoungest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerBiggest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);
INT FileComparerSmallest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2);

/* Gibt eine Liste der Dateien und/oder der Verzeichnisse eines Laufwerks zurück */
void BrbReadDir(struct BrbReadDir* inst)
{
	// Prüfung der Parameter
	if(inst->pDevice == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}
	if(inst->pFileFilter == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}

	if(inst->pList == 0)
	{
		inst->nStatus = eBRB_ERR_NULL_POINTER;
		return;
	}

	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = 65535;
			inst->nDirCount = 0;
			inst->nFileCount = 0;
			inst->nTotalCount = 0;
			memset(&inst->FilterListMan, 0, sizeof(inst->FilterListMan));
			memset(&inst->fbDirRead, 0, sizeof(inst->fbDirRead));
			memset(&inst->FileInfo, 0, sizeof(inst->FileInfo));
			memset(&inst->FileListMan, 0, sizeof(inst->FileListMan));
			// Der Einfachkeit halber wird das File-Array in einer MemList verwaltet
			inst->FileListMan.pList = inst->pList;
			inst->FileListMan.nIndexMax = inst->nListIndexMax;
			inst->FileListMan.nEntryLength = sizeof(inst->ReadListEntry);
			BrbMemListClear(&inst->FileListMan);
			// Prüfen, ob bei benutzerdefinierter Filterung eine Funktion angegeben ist
			inst->eStep = eSTEP_GET_FILTER;
			if(inst->bUserFilter == 1)
			{
				if(inst->pUserFilterFunction == 0)
				{
					inst->nStatus = fiERR_PARAMETER;
					inst->eStep = eSTEP_INIT;
				}
			}
			// Prüfen, ob bei benutzerdefinierter Sortierung eine Vergleichsfunktion angegeben ist
			if(inst->eSorting == eBRB_FILE_SORTING_USER)
			{
				if(inst->pUserCompareFunction == 0)
				{
					inst->nStatus = fiERR_PARAMETER;
					inst->eStep = eSTEP_INIT;
				}
			}
			break;

		case eSTEP_GET_FILTER:
			// Filtertext in Liste umsetzen
			// Der Einfachkeit halber wird die Filter-Liste in einer MemList verwaltet
			inst->FilterListMan.pList = (UDINT)&inst->FilterList;
			inst->FilterListMan.nIndexMax = nBRB_FILTER_SINGLE_INDEX_MAX;
			inst->FilterListMan.nEntryLength = nBRB_FILTER_SINGLE_CHAR_MAX+1;
			BrbMemListClear(&inst->FilterListMan);
			if(strlen(inst->pFileFilter))
			{
				inst->FilterListMan.nEntryCount = BrbStringSplit(inst->pFileFilter, ";", inst->FilterListMan.pList, inst->FilterListMan.nIndexMax, inst->FilterListMan.nEntryLength);
			}
			inst->eStep = eSTEP_READ_DIR;
			break;

		case eSTEP_READ_DIR:
			// Laufwerk auslesen
			inst->nStatus = 65535;
			inst->fbDirRead.enable = 1;
			inst->fbDirRead.pDevice = (UDINT)inst->pDevice;
			inst->fbDirRead.pPath = (UDINT)inst->pPath;
			inst->fbDirRead.option = fiBOTH;
			inst->fbDirRead.pData = (UDINT)&inst->FileInfo;
			inst->fbDirRead.data_len = sizeof(inst->FileInfo);
			DirRead(&inst->fbDirRead);
			if(inst->fbDirRead.status == 0)
			{
				// Filterprüfung
				BOOL bInFilter = 0;
				// Interner Filter (Dateiendung)
				if((strcmp((STRING*)&inst->FileInfo.Filename, "..") == 0 && inst->bWithParentDir == 0) || strcmp((STRING*)&inst->FileInfo.Filename, ".") == 0)
				{
					// ".." bezieht sich auf das übergeordnete Verzeichnis und wird nur optional zurückgegeben
					// "." bezieht sich auf das eigene Verzeichnis und wird grundsätzlich nicht zurückgegeben
				}
				else if(strcmp((STRING*)&inst->FileInfo.Filename, "..") == 0 && inst->bWithParentDir == 1)
				{
					// ".." bezieht sich auf das übergeordnete Verzeichnis und wird nur optional zurückgegeben
					bInFilter = 1;
					inst->nDirCount += 1;
				}
				else if(inst->eFilter != eBRB_DIR_INFO_ONLY_FILES && inst->FileInfo.Filelength == 0)
				{
					// Verzeichnis
					BOOL bInUserFilter = CallUserFilterFunction(inst, &inst->FileInfo);
					if(bInUserFilter == 1)
					{
						bInFilter = 1;
						inst->nDirCount += 1;
					}
				}
				else if(inst->eFilter != eBRB_DIR_INFO_ONLY_DIRS && inst->FileInfo.Filelength > 0)
				{
					if(inst->FilterListMan.nEntryCount > 0)
					{
						UINT nFilterListIndex = 0;
						for(nFilterListIndex=0; nFilterListIndex<inst->FilterListMan.nEntryCount; nFilterListIndex++)
						{
							if(BrbStringEndsWith((STRING*)&inst->FileInfo.Filename, inst->FilterList[nFilterListIndex]) == 1)
							{
								// Datei
								BOOL bInUserFilter = CallUserFilterFunction(inst, &inst->FileInfo);
								if(bInUserFilter == 1)
								{
									bInFilter = 1;
									inst->nFileCount += 1;
								}
								break;
							}
						}
					}
					else
					{
						// Datei
						BOOL bInUserFilter = CallUserFilterFunction(inst, &inst->FileInfo);
						if(bInUserFilter == 1)
						{
							bInFilter = 1;
							inst->nFileCount += 1;
						}
					}
				}
				if(bInFilter == 1)
				{
					// Im Filter
					memset(&inst->ReadListEntry, 0, sizeof(inst->ReadListEntry));
					memcpy(&inst->ReadListEntry, &inst->FileInfo, sizeof(inst->ReadListEntry));
					if(inst->FileListMan.nEntryCount <= inst->FileListMan.nIndexMax)
					{
						// Liste ist nicht voll -> in Liste eintragen
						BrbMemListIn(&inst->FileListMan, inst->FileListMan.nEntryCount, (UDINT)&inst->ReadListEntry);
					}
					else
					{
						// Liste ist voll
						// Sortieren
						SortList(inst);
						// Den letzten Eintrag mit dem aktuellen vergleichen
						fiDIR_READ_DATA* pFileInfoLast = (fiDIR_READ_DATA*)(inst->FileListMan.pList + ((inst->FileListMan.nEntryCount-1) * inst->FileListMan.nEntryLength));
						SINT nSort = SortTwoEntries(pFileInfoLast, &inst->FileInfo, inst->eSorting, inst->bCaseSensitive, inst->pUserCompareFunction);
						if(nSort > 0)
						{
							// Neue Datei ist näher am Filter als der letzte Eintrag -> ersetzen
							inst->FileListMan.nEntryCount -= 1;
							BrbMemListIn(&inst->FileListMan, inst->FileListMan.nEntryCount, (UDINT)&inst->ReadListEntry);
						}
					}
				}
				memset(&inst->FileInfo, 0, sizeof(inst->FileInfo));
				inst->fbDirRead.entry += 1;
			}
			else if(inst->fbDirRead.status == fiERR_NO_MORE_ENTRIES)
			{
				// Alle Dateien gelesen
				inst->nTotalCount = inst->nDirCount + inst->nFileCount;
				inst->eStep = eSTEP_SORT;
			}
			else if(inst->fbDirRead.status != 65535)
			{
				inst->nStatus = inst->fbDirRead.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_SORT:
			inst->nStatus = 65535;
			SortList(inst);
			inst->nStatus = eBRB_ERR_OK;
			inst->eStep = eSTEP_INIT;
			break;

	}
}

BOOL CallUserFilterFunction(struct BrbReadDir* inst, fiDIR_READ_DATA* pFileInfo)
{
	BOOL bResult = 0;
	if(inst->bUserFilter == 1)
	{
		if(inst->pUserFilterFunction != 0)
		{
			// Funktion hinterlegt -> Funktionspointer casten
			BOOL (*pFpFunction) (BrbReadDirListEntry_TYP*) = (void*)inst->pUserFilterFunction;
			// Funktion mit Kopie des Eintrags aufrufen
			BrbReadDirListEntry_TYP Entry;
			memset(&Entry, 0, sizeof(Entry));
			memcpy(&Entry, pFileInfo, sizeof(Entry));
			bResult = (*pFpFunction)(&Entry);
		}
	}
	else
	{
		bResult = 1;
	}
	return bResult;
}

void SortList(struct BrbReadDir* inst)
{
	switch(inst->eSorting)
	{
		case eBRB_FILE_SORTING_NONE:
			qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerNone);
			break;

		case eBRB_FILE_SORTING_ALPH_UP:
			if(inst->bCaseSensitive == 1)
			{
				qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerAlphUpCaseSensitive);
			}
			else
			{
				qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerAlphUp);
			}
			break;

		case eBRB_FILE_SORTING_ALPH_DOWN:
			if(inst->bCaseSensitive == 1)
			{
				qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerAlphDownCaseSensitive);
			}
			else
			{
				qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerAlphDown);
			}
			break;

		case eBRB_FILE_SORTING_OLDEST:
			qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerOldest);
			break;
	
		case eBRB_FILE_SORTING_YOUNGEST:
			qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerYoungest);
			break;

		case eBRB_FILE_SORTING_BIGGEST:
			qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerBiggest);
			break;

		case eBRB_FILE_SORTING_SMALLEST:
			qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(fiDIR_READ_DATA), (void*)&FileComparerSmallest);
			break;

		case eBRB_FILE_SORTING_USER:
			if(inst->pUserCompareFunction != 0)
			{
				// Funktion hinterlegt -> Funktionspointer übergeben
				qsort((fiDIR_READ_DATA*)inst->pList, inst->nListIndexMax+1, sizeof(BrbReadDirListEntry_TYP), (void*)inst->pUserCompareFunction);
			}
			else
			{
				// Funktion nicht hinterlegt -> keine Sortierung
			}
			break;
	}
}

INT SortTwoEntries(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2, BrbFileSorting_ENUM eSorting, BOOL bCaseSensitive, UDINT pUserCompareFunction)
{
	INT nResult = 0;
	switch(eSorting)
	{
		case eBRB_FILE_SORTING_NONE:
			nResult = FileComparerNone(pEntry1, pEntry2);
			break;

		case eBRB_FILE_SORTING_ALPH_UP:
			if(bCaseSensitive == 1)
			{
				nResult = FileComparerAlphUpCaseSensitive(pEntry1, pEntry2);
			}
			else
			{
				nResult = FileComparerAlphUp(pEntry1, pEntry2);
			}
			break;

		case eBRB_FILE_SORTING_ALPH_DOWN:
			if(bCaseSensitive == 1)
			{
				nResult = FileComparerAlphDownCaseSensitive(pEntry1, pEntry2);
			}
			else
			{
				nResult = FileComparerAlphDown(pEntry1, pEntry2);
			}
			break;

		case eBRB_FILE_SORTING_OLDEST:
			nResult = FileComparerOldest(pEntry1, pEntry2);
			break;
	
		case eBRB_FILE_SORTING_YOUNGEST:
			nResult = FileComparerYoungest(pEntry1, pEntry2);
			break;

		case eBRB_FILE_SORTING_BIGGEST:
			nResult = FileComparerBiggest(pEntry1, pEntry2);
			break;

		case eBRB_FILE_SORTING_SMALLEST:
			nResult = FileComparerSmallest(pEntry1, pEntry2);
			break;
		
		case eBRB_FILE_SORTING_USER:
			if(pUserCompareFunction != 0)
			{
				// Funktion hinterlegt -> Funktionspointer casten
				INT (*pFpFunction) (BrbReadDirListEntry_TYP*, BrbReadDirListEntry_TYP*) = (void*)pUserCompareFunction;
					// Funktion aufrufen
					nResult = (*pFpFunction)((BrbReadDirListEntry_TYP*) pEntry1, (BrbReadDirListEntry_TYP*)pEntry2);
			}
			else
			{
				// Funktion nicht hinterlegt -> keine Sortierung
				return 0;
			}
			break;

	}
	return nResult;
}

INT FileComparerNone(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	// Die Verzeichnisse müssen oben sein

	// Anpassung für Compiler-Version V6.3.0: Das Setzen von nResult und Zurückgeben am Ende der Funktion bewirkte, dass die leeren Elemente
	// ganz oben und die gefüllten Elemente ganz unten waren. Jetzt wird der Rückgabewert direkt im If-Block zurückgegeben.
	//INT nResult = 0;

	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);		
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

INT FileComparerAlphUp(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		STRING sFilename1[nBRB_FILE_NAME_CHAR_MAX];
		strcpy(sFilename1, (STRING*)&pEntry1->Filename);
		BrbStringToUpper(sFilename1);
		STRING sFilename2[nBRB_FILE_NAME_CHAR_MAX];
		strcpy(sFilename2, (STRING*)&pEntry2->Filename);
		BrbStringToUpper(sFilename2);
		return (INT)(strcmp((STRING*)&sFilename1, (STRING*)&sFilename2));
	}
}

INT FileComparerAlphUpCaseSensitive(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		return (INT)(strcmp((STRING*)&pEntry1->Filename, (STRING*)&pEntry2->Filename));
	}
}

INT FileComparerAlphDown(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		STRING sFilename1[nBRB_FILE_NAME_CHAR_MAX];
		strcpy(sFilename1, (STRING*)&pEntry1->Filename);
		BrbStringToUpper(sFilename1);
		STRING sFilename2[nBRB_FILE_NAME_CHAR_MAX];
		strcpy(sFilename2, (STRING*)&pEntry2->Filename);
		BrbStringToUpper(sFilename2);
		return (INT)(strcmp((STRING*)&sFilename2, (STRING*)&sFilename1));
	}
}

INT FileComparerAlphDownCaseSensitive(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		return (INT)(strcmp((STRING*)&pEntry2->Filename, (STRING*)&pEntry1->Filename));
	}
}

INT FileComparerOldest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		if(pEntry1->Date < pEntry2->Date)
		{
			return (-1);
		}
		else if(pEntry1->Date > pEntry2->Date)
		{
			return (1);
		}
		else
		{
			return (FileComparerAlphUp(pEntry1, pEntry2));
		}
	}
}

INT FileComparerYoungest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		if(pEntry1->Date > pEntry2->Date)
		{
			return (-1);
		}
		else if(pEntry1->Date < pEntry2->Date)
		{
			return (1);
		}
		else
		{
			return (FileComparerAlphUp(pEntry1, pEntry2));
		}
	}
}

INT FileComparerBiggest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		if(pEntry1->Filelength > pEntry2->Filelength)
		{
			return (-1);
		}
		else if(pEntry1->Filelength < pEntry2->Filelength)
		{
			return (1);
		}
		else
		{
			return (FileComparerAlphUp(pEntry1, pEntry2));
		}
	}
}

INT FileComparerSmallest(fiDIR_READ_DATA* pEntry1, fiDIR_READ_DATA* pEntry2)
{
	UDINT nNameLen1 = strlen((STRING*)&pEntry1->Filename);
	UDINT nNameLen2 = strlen((STRING*)&pEntry2->Filename);
	if(nNameLen1 == 0 && nNameLen2 == 0)
	{
		return (0);
	}
	else if(nNameLen1 == 0)
	{
		return (1);
	}
	else if(nNameLen2 == 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength == 0 && pEntry2->Filelength > 0)
	{
		return (-1);
	}
	else if(pEntry1->Filelength > 0 && pEntry2->Filelength == 0)
	{
		return (1);
	}
	else if(strcmp((STRING*)&pEntry1->Filename, "..") == 0)
	{
		return (-1);
	}
	else if(strcmp((STRING*)&pEntry2->Filename, "..") == 0)
	{
		return (1);
	}
	else
	{
		if(pEntry1->Filelength < pEntry2->Filelength)
		{
			return (-1);
		}
		else if(pEntry1->Filelength > pEntry2->Filelength)
		{
			return (1);
		}
		else
		{
			return (FileComparerAlphUp(pEntry1, pEntry2));
		}
	}
}

