/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringReplace.c
 * Author: niedermeierr
 * Created: May 02, 2016
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

/* Ersetzt eine Zeichenfolge in einem Text durch eine andere */
unsigned long BrbWcStringReplace(plcwstring* pText, plcwstring* pFind, plcwstring* pReplace)
{
	UDINT nResult = 0;
	UDINT nTextLen = brwcslen(pText);
	UDINT nFindLen = brwcslen(pFind);
	UDINT nFindByteLen = nFindLen * 2;
	UDINT nReplaceLen = brwcslen(pReplace);
	UDINT nReplaceByteLen = nReplaceLen * 2;
	DINT nDiff = (DINT)nReplaceLen - (DINT)nFindLen;
	UDINT nCharIndex = 0;
	// Text von vorne nach hinten durchsuchen
	while(1==1)
	{
		// Textstelle vergleichen
		if(memcmp((pText + nCharIndex), pFind, nFindByteLen) == 0)
		{
			// Text gefunden
			if(nDiff > 0)
			{
				// Replace ist länger
				UDINT nRestByteLen = (nTextLen - nCharIndex + 1) * 2;
				memmove((pText + nCharIndex + nDiff), (pText + nCharIndex), nRestByteLen);
				memcpy((pText + nCharIndex), pReplace, nReplaceByteLen);
				nTextLen += nDiff;
				nCharIndex += nReplaceLen;
				nResult += 1;
			}
			else if(nDiff < 0)
			{
				// Replace ist kürzer
				UDINT nRestByteLen = (nTextLen - nCharIndex + nDiff + 1) * 2;
				memmove((pText + nCharIndex), (pText + nCharIndex - nDiff), nRestByteLen);
				memcpy((pText + nCharIndex), pReplace, nReplaceByteLen);
				nTextLen += nDiff;
				nCharIndex += nReplaceLen;
				nResult += 1;
			}
			else
			{
				// Zeichenfolgen sind gleich lang
				memcpy((pText + nCharIndex), pReplace, nReplaceByteLen);
				nCharIndex += nReplaceLen;
				nResult += 1;
			}
		}
		else
		{
			nCharIndex += 1;
		}
		if(nCharIndex>=nTextLen)
		{
			break;
		}
	}
	return nResult;
}
