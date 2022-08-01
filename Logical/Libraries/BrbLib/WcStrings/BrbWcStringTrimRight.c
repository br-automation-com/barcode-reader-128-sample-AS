/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringTrimRight.c
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

/* Schneidet eine Zeichenfolge auf der rechten Seite eines Textes heraus */
plcbit BrbWcStringTrimRight(plcwstring* pText, plcwstring* pTrim)
{
	BOOL bResult = 0;
	UDINT nTrimLen = brwcslen(pTrim);
	BOOL bLeave = 0;
	do
	{
		if(BrbWcStringEndsWith(pText, pTrim) == 1)
		{
			UDINT nTextLen = brwcslen(pText);
			BrbWcStringCut(pText, nTextLen-nTrimLen, nTrimLen, 0);
			bResult = 1;
		}
		else
		{
			bLeave = 1;
		}
	} while(bLeave == 0);
	return bResult;
}
