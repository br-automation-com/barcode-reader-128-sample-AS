/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringCountText.c
 * Author: niedermeierr
 * Created: April 13, 2016
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

/* Zählt das Vorkommen einer Zeichenfolge innerhalb eines Texts */
unsigned long BrbStringCountText(plcstring* pText, plcstring* pFind, unsigned long nTextLen)
{
	UDINT nResult = 0;
	// Wenn die Länge bekannt ist, kann sie übergeben werden
	// Damit entfällt das Aufrufen von strlen, welches bei langen Texten Zeit braucht
	UDINT nLenText = nTextLen;
	if(nTextLen == 0)
	{
		nLenText = strlen(pText);
	}
	UDINT nLenFind = strlen(pFind);
	UDINT nCharIndex = 0;
	// Text von vorne nach hinten durchsuchen
	for(nCharIndex=0; nCharIndex<nLenText; nCharIndex++)
	{
		// Textstelle vergleichen
		if(memcmp((pText + nCharIndex), pFind, nLenFind) == 0)
		{
			// Text gefunden
			nResult += 1;
			nCharIndex += nLenFind-1;
		}
	}
	return nResult;
}
