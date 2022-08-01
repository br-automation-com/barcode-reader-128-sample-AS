/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbMemListClear.c
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

/* Löscht die gesamte Liste */
signed long BrbMemListClear(struct BrbMemListManagement_Typ* pListManagement)
{
	DINT nResult = -1;
	if(pListManagement != 0)
	{
		if(pListManagement->pList > 0)
		{
			// Länge der Liste berechnen
			UDINT nLen = (pListManagement->nIndexMax+1) * pListManagement->nEntryLength;
			// Komplette Liste löschen
			memset((USINT*)pListManagement->pList, 0, nLen);
			// Anzahl auf 0 setzen
			pListManagement->nEntryCount = 0;
			nResult = 0;
		}
	}
	return nResult;
}
