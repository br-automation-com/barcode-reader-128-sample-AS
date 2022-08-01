/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringStartsWith.c
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

/* Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge beginnt */
plcbit BrbWcStringStartsWith(plcwstring* pText, plcwstring* pFind)
{
	BOOL bResult = 0;
	UDINT nByteLenFind = brwcslen(pFind) * 2;
	if(memcmp(pText, pFind, nByteLenFind) == 0)
	{
		bResult = 1;
	}
	return bResult;
}
