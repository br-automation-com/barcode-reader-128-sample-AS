/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringIsHex.c
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

#ifdef __cplusplus
	};
#endif

/* Gibt zurück, ob ein Text eine hexadezimale Nummer enthält */
plcbit BrbWcStringIsHex(plcwstring* pText, plcbit bLowCaseAllowed)
{
	BOOL bResult = 1;
	UDINT nTextLen = brwcslen(pText);
	if(nTextLen == 0)
	{
		bResult = 0;
	}
	else
	{
		UDINT nIndex;
		UINT* pChar;
		for(nIndex=0; nIndex<nTextLen; nIndex++)
		{
			pChar = (UINT*)(pText + nIndex);
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
