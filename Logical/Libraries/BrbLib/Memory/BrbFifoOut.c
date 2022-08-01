/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbFifoOut.c
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

#ifdef __cplusplus
	};
#endif

/* Gibt den Eintrag an erster Stelle zurück und löscht ihn aus der Liste */
signed long BrbFifoOut(struct BrbMemListManagement_Typ* pListManagement, unsigned long pListEntry)
{
	DINT nResult = -1;
	if(pListManagement != 0)
	{
		if(pListManagement->nEntryCount > 0)
		{
			nResult = BrbMemListOut(pListManagement, 0, pListEntry);
		}
	}
	return nResult;
}
