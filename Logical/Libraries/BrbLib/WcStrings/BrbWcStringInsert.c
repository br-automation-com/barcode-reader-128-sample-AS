/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringInsert.c
 * Author: niedermeierr
 * Created: May 02, 2016
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

/* Fügt eine Zeichenfolge in einen Text ein */
unsigned long BrbWcStringInsert(plcwstring* pText, unsigned long nInsertIndex, plcwstring* pInsert)
{
	UDINT nResult = 0;
	UDINT nTextLen = brwcslen(pText);
	UDINT nInsertLen = brwcslen(pInsert);
	UDINT nInsertByteLen = nInsertLen * 2;
	UDINT nRestByteLen = (nTextLen - nInsertIndex + 1) * 2;
	// Zeichen am Index nach hinten schieben
	memmove((pText + nInsertIndex + nInsertLen), (pText + nInsertIndex), nRestByteLen);
	memcpy((pText + nInsertIndex), pInsert, nInsertByteLen);
	nResult = (UDINT)(pText + nInsertIndex + nInsertLen);
	return nResult;
}
