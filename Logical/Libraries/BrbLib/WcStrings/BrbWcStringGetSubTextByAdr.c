/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringGetSubTextByAdr.c
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

/* Gibt eine Zeichenfolge innerhalb eines Textes zurück */
unsigned long BrbWcStringGetSubTextByAdr(plcwstring* pStart, plcwstring* pEnd, plcwstring* pSubText)
{
	UDINT nResult = 0;
	if(pEnd > pStart)
	{
		UDINT nLen = (UDINT)(pEnd - pStart);
		nResult = BrbWcStringGetSubTextByLen(pStart, nLen, pSubText);
	}
	return nResult;
}
