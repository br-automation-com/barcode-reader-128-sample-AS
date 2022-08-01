/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringIsHex.c
 * Author: niedermeierr
 * Created: January 17, 2017
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

/* Gibt zurück, ob ein Text eine hexadezimale Nummer enthält */
plcbit BrbStringIsHex(plcstring* pText, plcbit bLowCaseAllowed)
{
	BOOL bResult = 1;
	UDINT nTextLen = strlen(pText);
	if(nTextLen == 0)
	{
		bResult = 0;
	}
	else
	{
		UDINT nIndex;
		USINT* pChar;
		for(nIndex=0; nIndex<nTextLen; nIndex++)
		{
			pChar = (USINT*)(pText + nIndex);
			if((*pChar >= 48 && *pChar <= 57)) // 0..9
			{
			}
			else if((*pChar >= 65 && *pChar <= 70)) // A..F
			{
			}
			else if((*pChar >= 97 && *pChar <= 102) && bLowCaseAllowed == 1) // a..f
			{
			}
			else
			{
				bResult = 0;
				break;
			}
		}
	}
	return bResult;
}
