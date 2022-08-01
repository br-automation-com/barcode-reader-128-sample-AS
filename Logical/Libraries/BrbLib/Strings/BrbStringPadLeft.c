/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringPadLeft.c
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

/* Füllt einen String auf der linken Seite mit einem Füllzeichen auf eine bestimme Länge */
plcbit BrbStringPadLeft(plcstring* pText, plcstring* pFillChar, unsigned long nLen)
{
	BOOL bResult = 0;
	UDINT nTextLen = strlen(pText);
	DINT nFillLen = nLen - nTextLen;
	USINT* pChar = (USINT*)pText;
	if(nFillLen > 0)
	{
		// Text nach rechts schieben
		memmove(pText+nFillLen, pText, nTextLen+1);
		// Füllzeichen einsetzen
		while(pChar < (USINT*)(pText+nFillLen))
		{
			*pChar = *pFillChar;
			pChar++;
		}
		bResult = 1;
	}
	return bResult;
}
