/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringPadRight.c
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

/* F�llt einen String auf der rechten Seite mit einem F�llzeichen auf eine bestimme L�nge */
plcbit BrbWcStringPadRight(plcwstring* pText, plcwstring* pFillChar, unsigned long nLen)
{
	BOOL bResult = 0;
	UDINT nTextLen = brwcslen(pText);
	DINT nFillLen = nLen - nTextLen;
	UINT* pChar = (UINT*)pText + nTextLen;
	if(nFillLen > 0)
	{
		// F�llzeichen einsetzen
		while(pChar < (UINT*)(pText+nLen))
		{
			*pChar = *pFillChar;
			pChar++;
		}
		bResult = 1;
	}
	// Terminierung
	*pChar = 0;
	return bResult;
}
