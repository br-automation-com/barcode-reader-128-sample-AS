/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringPadLeft.c
 * Author: niedermeierr
 * Created: May 03, 2016
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
plcbit BrbWcStringPadLeft(plcwstring* pText, plcwstring* pFillChar, unsigned long nLen)
{
	BOOL bResult = 0;
	UDINT nTextLen = brwcslen(pText);
	UDINT nTextByteLen = nTextLen * 2;
	DINT nFillLen = nLen - nTextLen;
	UINT* pChar = (UINT*)pText;
	if(nFillLen > 0)
	{
		// Text nach rechts schieben
		memmove(pText+nFillLen, pText, nTextByteLen+2);
		// Füllzeichen einsetzen
		while(pChar < (UINT*)(pText+nFillLen))
		{
			*pChar = *pFillChar;
			pChar++;
		}
		bResult = 1;
	}
	return bResult;
}
