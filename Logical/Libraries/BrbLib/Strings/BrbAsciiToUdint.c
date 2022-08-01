/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbAsciiToUdint.c
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

/* Wandelt einen String zu einem UDINT-Wert */
unsigned long BrbAsciiToUdint(plcstring* pText)
{
	UDINT nFuncRetVal;
	UDINT nCharCount;
	STRING* pChar;
	UDINT nDiff; // Wertigkeit
	UDINT nDigitVal; // Wert der Stelle als Einer-Wertigkeit
	nFuncRetVal = 0;
	nCharCount = strlen((STRING*)pText )-1;
	nDiff = 1;
	while(1 == 1)
	{
		// Zeichen von hinten nach vorne durchlaufen
		pChar = (STRING*)pText+nCharCount;
		if(*pChar >= '0' && *pChar <= '9')
		{
			// Zeichen ist eine Nummer
			nDigitVal = *pChar-'0'; // Wert als Einer-Stelle
			LREAL rResult = (LREAL)nFuncRetVal + ((LREAL)nDigitVal * (LREAL)nDiff);
			if(rResult > (LREAL)4294967295UL)
			{
				// Ergebnis zu groß für UDINT
				nFuncRetVal = 4294967295UL; // Maximalen Wert zurückgeben
				break;
			}
			nFuncRetVal += nDigitVal * nDiff; // Wert als tatsächliche Wertigkeit zum Udint addieren
			nDiff *= 10; // Nächste Wertigkeit (Einer->Zehner->Hunderter->usw.)
		}
		else
		{
			// Zeichen ist keine Nummer -> Abbrechen
			break;
		}
		if(nCharCount > 0)
		{
			// Vorheriges Zeichen
			nCharCount--;
		}
		else
		{
			// Alle Zeichen durchlaufen
			break;
		}
	}
	return nFuncRetVal;
}
