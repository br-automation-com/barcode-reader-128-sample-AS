/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringPadRight.c
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

/* Füllt einen String auf der rechten Seite mit einem Füllzeichen auf eine bestimme Länge */
plcbit BrbStringPadRight(plcstring* pText, plcstring* pFillChar, unsigned long nLen)
{
	BOOL bResult = 0;
	UDINT nTextLen = strlen(pText);
	DINT nFillLen = nLen - nTextLen;
	USINT* pChar = (USINT*)pText + nTextLen;
	if(nFillLen > 0)
	{
		// Füllzeichen einsetzen
		while(pChar < (USINT*)(pText+nLen))
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
