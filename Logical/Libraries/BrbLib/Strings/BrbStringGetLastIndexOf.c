/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringGetLastIndexOf.c
 * Author: niedermeierr
 * Created: December 01, 2014
 *******************************************************************/

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
signed long BrbStringGetLastIndexOf(plcstring* pText, plcstring* pFind, unsigned long nTextLen)
{
	DINT nResult = -1;
	// Wenn die Länge bekannt ist, kann sie übergeben werden
	// Damit entfällt das Aufrufen von strlen, welches bei langen Texten Zeit braucht
	UDINT nLenText = nTextLen;
	if(nTextLen == 0)
	{
		nLenText = strlen(pText);
	}
	UDINT nLenFind = strlen(pFind);
	DINT nCharIndex;
	// Text von hinten nach vorne durchsuchen
	for(nCharIndex=nLenText-1; nCharIndex>=0; nCharIndex--)
	{
		// Textstelle vergleichen
		if(memcmp((pText + nCharIndex), pFind, nLenFind) == 0)
		{
			// Text gefunden
			nResult = nCharIndex;
			break;
		}
	}
	return nResult;
}
