/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringToLower.c
 * Author: niedermeierr
 * Created: March 04, 2015
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

/* Konvertiert einen String zu Kleinbuchstaben */
plcbit BrbStringToLower(plcstring* pText)
{
	BOOL bResult = 0;
	UDINT nTextLen = strlen(pText);
	UDINT nIndex;
	USINT* pChar;
	for(nIndex=0; nIndex<nTextLen; nIndex++)
	{
		pChar = (USINT*)(pText + nIndex);
		if(*pChar >= 65 && *pChar <= 90)
		{
			*pChar = *pChar + 32;
			bResult = 1;
		}
	}
	return bResult;
}
