/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringCutFromLastSeparator.c
 * Author: niedermeierr
 * Created: October 21, 2014
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

/* Schneidet die Zeichenfolge ab dem letzten Trennzeichen in einem Text ab */
unsigned long BrbStringCutFromLastSeparator(plcstring* pText, plcstring* pSeparator, plcstring* pCut)
{
	UDINT nResult = 0;
	UDINT nTextLen = strlen(pText);
	STRING* pSeparatorStart = BrbStringGetLastAdrOf(pText, pSeparator, nTextLen);
	if(pSeparatorStart != 0)
	{
		UDINT nCutIndex = pSeparatorStart - pText;
		UDINT nCutLen = nTextLen - nCutIndex;
		BrbStringCut(pText, pSeparatorStart-pText, nCutLen, pCut);
		nResult = nCutLen;
	}
	return nResult;
}
