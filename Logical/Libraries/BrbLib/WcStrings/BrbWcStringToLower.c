/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringToLower.c
 * Author: niedermeierr
 * Created: May 04, 2016
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

/* Konvertiert einen WcString zu Kleinbuchstaben */
plcbit BrbWcStringToLower(plcwstring* pText)
{
	BOOL bResult = 0;
	UDINT nTextLen = brwcslen(pText);
	UDINT nIndex;
	UINT* pChar;
	for(nIndex=0; nIndex<nTextLen; nIndex++)
	{
		pChar = (UINT*)(pText + nIndex);
		if(*pChar >= 65 && *pChar <= 90)
		{
			*pChar = *pChar + 32;
			bResult = 1;
		}
	}
	return bResult;
}
