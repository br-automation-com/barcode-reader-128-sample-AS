/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringGetLastAdrOf.c
 * Author: niedermeierr
 * Created: October 21, 2014
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

/* Gibt die letzte Adresse eines zu suchenden Textes zur�ck */
plcstring* BrbStringGetLastAdrOf(plcstring* pText, plcstring* pFind, unsigned long nTextLen)
{
	STRING* pResult = 0;
	// Wenn die L�nge bekannt ist, kann sie �bergeben werden
	// Damit entf�llt das Aufrufen von strlen, welches bei langen Texten Zeit braucht
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
			pResult = pText + nCharIndex;
			break;
		}
	}
	return pResult;
}
