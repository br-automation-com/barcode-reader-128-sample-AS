/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringStartsWith.c
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

/* Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge beginnt */
plcbit BrbStringStartsWith(plcstring* pText, plcstring* pFind)
{
	BOOL bResult = 0;
	UDINT nLenFind = strlen(pFind);
	if(memcmp(pText, pFind, nLenFind) == 0)
	{
		bResult = 1;
	}
	return bResult;
}
