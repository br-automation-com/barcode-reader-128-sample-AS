/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringGetAdrOf.c
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

/* Gibt die erste Adresse eines zu suchenden Textes zurück */
plcstring* BrbStringGetAdrOf(plcstring* pText, plcstring* pFind, unsigned long nTextLen)
{
	STRING* pResult = 0;
	// Wenn die Länge bekannt ist, kann sie übergeben werden
	// Damit entfällt das Aufrufen von strlen, welches bei langen Texten Zeit braucht
	UDINT nLenText = nTextLen;
	if(nTextLen == 0)
	{
		nLenText = strlen(pText);
	}
	UDINT nLenFind = strlen(pFind);
	UDINT nCharIndex;
	// Text von vorne nach hinten durchsuchen
	for(nCharIndex=0; nCharIndex<nLenText; nCharIndex++)
	{
		// Textstelle vergleichen
		if(memcmp((pText + nCharIndex), pFind, nLenFind) == 0)
		{
			// Text gefunden
			pResult = pText + nCharIndex;
			break;
		}
	}
	return pResult;
}
