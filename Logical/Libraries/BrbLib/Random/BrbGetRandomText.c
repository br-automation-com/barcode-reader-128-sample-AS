
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

/* Erzeugt einen Zufallstext mit bestimmter Länge (nur Zahlen, Groß- und Kleinbuchstaben) */
unsigned long BrbGetRandomText(plcstring* pText, unsigned long nTextSize, unsigned long nTextLength)
{
	memset(pText, 0, nTextSize);
	if(nTextLength >= nTextSize)
	{
		nTextLength = nTextSize-1;
	}
	UDINT nCharIndex = 0;
	for(nCharIndex=0; nCharIndex<nTextLength; nCharIndex++)
	{
		USINT nChar = 0;
		DINT nBlockIndex = BrbGetRandomDint(0,2);
		if (nBlockIndex == 0)
		{
			// Zahl
			nChar = (USINT)BrbGetRandomUdint(48,57);
		}
		else if (nBlockIndex == 1)
		{
			// Großbuchstabe
			nChar = (USINT)BrbGetRandomUdint(65,90);
		}
		else
		{
			// KLeinbuchstabe
			nChar = (USINT)BrbGetRandomUdint(97,122);
		}
		USINT* pChar = (USINT*)pText + nCharIndex;
		*pChar = nChar;
	}
	return nTextLength;
}
