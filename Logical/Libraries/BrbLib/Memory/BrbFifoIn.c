/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbFifoIn.c
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

/* Fügt einen neuen Eintrag an der letzten Stelle hinzu */
signed long BrbFifoIn(struct BrbMemListManagement_Typ* pListManagement, unsigned long pNewEntry)
{
	if(pListManagement != 0)
	{
		return BrbMemListIn(pListManagement, pListManagement->nEntryCount, pNewEntry);
	}
	else
	{
		return -1;
	}
}
