
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

/* Erzeugt einen Zufallstext mit bestimmter Länge aus einer Zeichenauswahl */
unsigned long BrbGetRandomStringExt(plcstring* pText, unsigned long nTextSize, unsigned long nTextLength, plcstring* pSelection)
{
	memset(pText, 0, nTextSize);
	UDINT nSelectionLength = strlen(pSelection);
	if(nSelectionLength > 0)
	{
		if(nTextLength >= nTextSize)
		{
			nTextLength = nTextSize-1;
		}
		UDINT nCharIndex = 0;
		for(nCharIndex=0; nCharIndex<nTextLength; nCharIndex++)
		{
			UDINT nSelectionCharIndex = BrbGetRandomUdint(0,nSelectionLength-1);
			USINT* pChar = (USINT*)pText + nCharIndex;
			*pChar = *((USINT*)pSelection + nSelectionCharIndex);
		}
	}
	else
	{
		nTextLength = 0;
	}
	return nTextLength;
}
