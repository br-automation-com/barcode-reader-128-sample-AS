/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringTrimLeft.c
 * Author: niedermeierr
 * Created: December 03, 2013
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

/* Schneidet eine Zeichenfolge auf der linken Seite eines Textes heraus */
plcbit BrbStringTrimLeft(plcstring* pText, plcstring* pTrim)
{
	BOOL bResult = 0;
	UDINT nTrimLen = strlen(pTrim);
	BOOL bLeave = 0;
	do
	{
		if(BrbStringStartsWith(pText, pTrim) == 1)
		{
			BrbStringCut(pText, 0, nTrimLen, 0);
			bResult = 1;
		}
		else
		{
			bLeave = 1;
		}
	} while(bLeave == 0);
	return bResult;
}
