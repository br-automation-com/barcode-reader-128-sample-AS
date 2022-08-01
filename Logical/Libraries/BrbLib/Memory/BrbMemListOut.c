/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbMemListOut.c
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

/* Holt einen bestimmten Eintrag und löscht ihn aus der Liste */
signed long BrbMemListOut(struct BrbMemListManagement_Typ* pListManagement, unsigned long nIndex, unsigned long pListEntry)
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
				UDINT pListDestination = 0;
				if(pListEntry > 0)
				{
					// Adresse des Eintrags berechnen
					pListSource = pListManagement->pList + (nIndex * pListManagement->nEntryLength);
					// Eintrag kopieren
					memcpy((USINT*)pListEntry, (USINT*)pListSource, pListManagement->nEntryLength);
				}
				// Anzahl dekrementieren
				pListManagement->nEntryCount -= 1;
				// Alle folgenden Einträge um 1 nach oben schieben
				pListDestination = pListManagement->pList + (nIndex * pListManagement->nEntryLength);
				pListSource = pListManagement->pList + ((nIndex+1) * pListManagement->nEntryLength);
				UDINT nLen = pListManagement->nEntryLength * (pListManagement->nIndexMax - nIndex);
				memmove((USINT*)pListDestination, (USINT*)pListSource, nLen);
				// Letzten Eintrag löschen
				pListDestination = pListManagement->pList + (pListManagement->nIndexMax * pListManagement->nEntryLength);
				memset((USINT*)pListDestination, 0, pListManagement->nEntryLength);
				nResult = nIndex;
			}
		}
	}
	return nResult;
}
