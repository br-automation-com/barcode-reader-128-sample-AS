/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringGetSubTextByAdr.c
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

#ifdef __cplusplus
	};
#endif

/* Gibt eine Zeichenfolge innerhalb eines Textes zurück */
plcstring* BrbStringGetSubTextByAdr(plcstring* pStart, plcstring* pEnd, plcstring* pSubText)
{
	STRING* pResult = 0;
	if(pEnd > pStart)
	{
		UDINT nLen = pEnd - pStart;
		pResult = BrbStringGetSubTextByLen(pStart, nLen, pSubText);
	}
	return pResult;
}
