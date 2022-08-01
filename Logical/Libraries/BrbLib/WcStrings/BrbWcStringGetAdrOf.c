/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringGetAdrOf.c
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

/* Gibt die erste Adresse eines zu suchenden Textes zurück */
unsigned long BrbWcStringGetAdrOf(plcwstring* pText, plcwstring* pFind, unsigned long nTextLen)
{
	UDINT nResult = 0;
	// Wenn die Länge bekannt ist, kann sie übergeben werden
	// Damit entfällt das Aufrufen von strlen, welches bei langen Texten Zeit braucht
	UDINT nLenText = nTextLen;
	if(nTextLen == 0)
	{
		nLenText = brwcslen(pText);
	}
	UDINT nByteLenFind = brwcslen(pFind) * 2;
	UDINT nCharIndex;
	// Text von vorne nach hinten durchsuchen
	for(nCharIndex=0; nCharIndex<nLenText; nCharIndex++)
	{
		// Textstelle vergleichen
		if(memcmp((pText + nCharIndex), pFind, nByteLenFind) == 0)
		{
			// Text gefunden
			nResult = (UDINT)(pText + nCharIndex);
			break;
		}
	}
	return nResult;
}
