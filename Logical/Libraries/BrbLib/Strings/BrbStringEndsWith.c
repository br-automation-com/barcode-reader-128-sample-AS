/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringEndsWith.c
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

/* Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge endet */
plcbit BrbStringEndsWith(plcstring* pText, plcstring* pFind)
{
	BOOL bResult = 0;
	UDINT nLenText = strlen(pText);
	UDINT nLenFind = strlen(pFind);
	if(nLenText >= nLenFind)
	{
		if(memcmp(pText + nLenText - nLenFind, pFind, nLenFind) == 0)
		{
			bResult = 1;
		}
	}
	return bResult;
}
