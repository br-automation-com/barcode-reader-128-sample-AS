/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbMemListIn.c
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

/* Fügt einen Eintrag an bestimmter Stelle in der Liste ein */
signed long BrbMemListIn(struct BrbMemListManagement_Typ* pListManagement, unsigned long nIndex, unsigned long pNewEntry)
{
	DINT nResult = -1;
	if(pListManagement != 0)
	{
		if(pListManagement->pList > 0)
		{
			if(pNewEntry > 0)
			{
				// Prüfen, ob noch ein Platz frei ist
				if((pListManagement->nEntryCount == 0) || ((pListManagement->nEntryCount-1) < pListManagement->nIndexMax))
				{
					// Prüfen, ob Index gültig
					if(nIndex <= pListManagement->nEntryCount)
					{
						// Alle Einträge ab nIndex um 1 nach unten schieben
						UDINT pListSource = pListManagement->pList + (nIndex * pListManagement->nEntryLength);
						UDINT pListDestination = pListManagement->pList + ((nIndex+1) * pListManagement->nEntryLength);
						UDINT nLen = pListManagement->nEntryLength * (pListManagement->nIndexMax - nIndex);
						memmove((USINT*)pListDestination, (USINT*)pListSource, nLen);
						// Übergebenen Eintrag in die Liste kopieren
						pListDestination = pListManagement->pList + (nIndex * pListManagement->nEntryLength);
						memcpy((USINT*)pListDestination, (USINT*)pNewEntry, pListManagement->nEntryLength);
						// Anzahl inkrementieren
						pListManagement->nEntryCount += 1;
						nResult = nIndex;
					}
				}
			}
		}
	}
	return nResult;
}
