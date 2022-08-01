/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbMemListGetEntry.c
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

/* Holt einen bestimmten Eintrag aus der Liste */
signed long BrbMemListGetEntry(struct BrbMemListManagement_Typ* pListManagement, unsigned long nIndex, unsigned long pListEntry)
{
	DINT nResult = -1;
	if(pListManagement != 0)
	{
		if(pListManagement->pList > 0)
		{
			// Prüfen, ob ein Eintrag vorhanden ist
			if(pListManagement->nEntryCount > nIndex)
			{
				UDINT pListSource = 0;
				if(pListEntry > 0)
				{
					// Adresse des Eintrags berechnen
					pListSource = pListManagement->pList + (nIndex * pListManagement->nEntryLength);
					// Eintrag kopieren
					memcpy((USINT*)pListEntry, (USINT*)pListSource, pListManagement->nEntryLength);
				}
				nResult = nIndex;
			}
		}
	}
	return nResult;
}
