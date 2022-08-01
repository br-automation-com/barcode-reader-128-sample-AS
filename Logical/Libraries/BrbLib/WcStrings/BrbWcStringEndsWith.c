/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringEndsWith.c
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

/* Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge endet */
plcbit BrbWcStringEndsWith(plcwstring* pText, plcwstring* pFind)
{
	BOOL bResult = 0;
	UDINT nLenText = brwcslen(pText);
	UDINT nLenFind = brwcslen(pFind);
	UDINT nByteLenFind = nLenFind * 2;
	if(nLenText >= nLenFind)
	{
		if(memcmp(pText + nLenText - nLenFind, pFind, nByteLenFind) == 0)
		{
			bResult = 1;
		}
	}
	return bResult;
}
