/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringSwap.c
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

/* Dreht die Reihenfolge der Zeichen in einem Text */
plcbit BrbStringSwap(plcstring* pText, plcstring* pSwapped, unsigned long nSwappedSize)
{
	BOOL bResult = 0;
	memset(pSwapped, 0, nSwappedSize);
	UDINT nIndex;
	USINT* pCharText;
	USINT* pCharSwapped;
	UDINT nTextLen = strlen(pText);
	if(nTextLen <= nSwappedSize)
	{
		for(nIndex=0; nIndex<nTextLen; nIndex++)
		{
			pCharText = (USINT*)(pText + nIndex);
			pCharSwapped = (USINT*)(pSwapped + nTextLen - nIndex - 1);
			*pCharSwapped = *pCharText;
		}
		bResult = 1;
	}
	return bResult;
}
