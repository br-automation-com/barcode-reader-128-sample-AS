/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDeleteFiles.c
 * Author: niedermeierr
 * Created: January 26, 2015
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
	eSTEP_DELETE_FILE,
};

/* Löscht Dateien, die über Filter festgelegt sind */
void BrbDeleteFiles(struct BrbDeleteFiles* inst)
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

	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = 65535;
			inst->nDeletedFileCount = 0;
			inst->nKeptFileCount = 0;
			memset(&inst->FilterListMan, 0, sizeof(inst->FilterListMan));
			memset(&inst->fbDirRead, 0, sizeof(inst->fbDirRead));
			memset(&inst->FileInfo, 0, sizeof(inst->FileInfo));
			inst->eStep = eSTEP_GET_FILTER;
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
			inst->fbDirRead.option = fiFILE;
			inst->fbDirRead.pData = (UDINT)&inst->FileInfo;
			inst->fbDirRead.data_len = sizeof(inst->FileInfo);
			DirRead(&inst->fbDirRead);
			if(inst->fbDirRead.status == 0)
			{
				BOOL bInFilter = 0;
				// Datumsprüfung
				if(inst->FileInfo.Date >= inst->dtDateStart && inst->FileInfo.Date <= inst->dtDateEnd)
				{
					// Filterprüfung
					if(inst->FilterListMan.nEntryCount > 0)
					{
						UINT nFilterListIndex = 0;
						for(nFilterListIndex=0; nFilterListIndex<inst->FilterListMan.nEntryCount; nFilterListIndex++)
						{
							if(BrbStringEndsWith((STRING*)&inst->FileInfo.Filename, inst->FilterList[nFilterListIndex]) == 1)
							{
								// Datei
								bInFilter = 1;
								break;
							}
						}
					}
					else
					{
						bInFilter = 1;
					}
				}
				if(bInFilter == 1)
				{
					// Datei löschen
					memset(&inst->fbFileDelete, 0, sizeof(inst->fbFileDelete));
					memset(&inst->sFileWithPath, 0, sizeof(inst->sFileWithPath));
					if(inst->pPath != 0)
					{
						BrbCombinePath((STRING*)inst->pPath, (STRING*)&inst->FileInfo.Filename, (STRING*)&inst->sFileWithPath);
					}
					else
					{
						strcat((STRING*)&inst->sFileWithPath, (STRING*)&inst->FileInfo.Filename);
					}
					inst->nDeletedFileCount += 1;
					inst->eStep = eSTEP_DELETE_FILE;
				}
				else
				{
					inst->nKeptFileCount += 1;
					inst->fbDirRead.entry += 1;
				}
				memset(&inst->FileInfo, 0, sizeof(inst->FileInfo));
			}
			else if(inst->fbDirRead.status == fiERR_NO_MORE_ENTRIES)
			{
				// Alle Dateien gelesen -> Fertig
				inst->nStatus = 0;
				inst->eStep = eSTEP_INIT;
			}
			else if(inst->fbDirRead.status != 65535)
			{
				inst->nStatus = inst->fbDirRead.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_DELETE_FILE:
			inst->nStatus = 65535;
			inst->fbFileDelete.enable = 1;
			inst->fbFileDelete.pDevice = (UDINT)inst->pDevice;
			inst->fbFileDelete.pName = (UDINT)&inst->sFileWithPath;
			FileDelete(&inst->fbFileDelete);
			if(inst->fbFileDelete.status == 0)
			{
				inst->eStep = eSTEP_READ_DIR;
			}
			else if(inst->fbFileDelete.status != 65535)
			{
				inst->nStatus = inst->fbFileDelete.status;
				inst->eStep = eSTEP_INIT;
			}
			break;

	}
}
