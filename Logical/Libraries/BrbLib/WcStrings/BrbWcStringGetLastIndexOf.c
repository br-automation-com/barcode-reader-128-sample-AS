/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringGetLastIndexOf.c
 * Author: niedermeierr
 * Created: April 29, 2016
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

/* Gibt den letzten Index eines zu suchenden Textes zurück */
signed long BrbWcStringGetLastIndexOf(plcwstring* pText, plcwstring* pFind, unsigned long nTextLen)
{
	DINT nResult = -1;
	// Wenn die Länge bekannt ist, kann sie übergeben werden
	// Damit entfällt das Aufrufen von strlen, welches bei langen Texten Zeit braucht
	UDINT nLenText = nTextLen;
	if(nTextLen == 0)
	{
		nLenText = brwcslen(pText);
	}
	UDINT nByteLenFind = brwcslen(pFind) * 2;
	DINT nCharIndex;
	// Text von hinten nach vorne durchsuchen
	for(nCharIndex=nLenText-1; nCharIndex>=0; nCharIndex--)
	{
		// Textstelle vergleichen
		if(memcmp((pText + nCharIndex), pFind, nByteLenFind) == 0)
		{
			// Text gefunden
			nResult = nCharIndex;
			break;
		}
	}
	return nResult;
}
