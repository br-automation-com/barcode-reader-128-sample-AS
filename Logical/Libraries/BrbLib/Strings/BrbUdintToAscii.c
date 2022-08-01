/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbUdintToAscii.c
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

#ifdef __cplusplus
	};
#endif

/* Wandelt einen UDINT-Wert zu einem String */
unsigned short BrbUdintToAscii(unsigned long nValue, plcstring* pText)
{
	UDINT nDiff; // Wertigkeit
	UDINT nHelpVal;
	BOOL bFirstDigitFound; // Erste gültige Stelle erkannt
	bFirstDigitFound = 0;
	// Die Schleife beginnt mit der größten Stelle und geht runter bis zur zweitletzten Stelle (Wertigkeit 10-90)
	for(nDiff = 1000000000; nDiff > 1; )
	{
		nHelpVal = nValue/nDiff;		// Ganzzahldivision bringt die Wertigkeit der gewünschten Stelle
		// Führende Nullen werden unterdrückt
		if(nHelpVal > 0 || bFirstDigitFound == 1)
		{
			bFirstDigitFound = 1;	// Ab der ersten Stelle, deren Wertigkeit größer 0 ist, müssen alle Werte übernommen werden
			*pText = '0'+nHelpVal;	// Den Ascii-Wert der Wertigkeit in den String einfügen
			pText++;				// String-Pointer ein Zeichen weiterstellen

		}
		nValue = nValue % nDiff;		// Rest der Ganzzahldivision brint den Wert ohne die schon behandelte Stelle (für nächsten Durchlauf)
		nDiff = nDiff/10;				// Teiler um eine Stelle verringern (für nächsten Durchlauf)
	}
	// Die letzte Stelle wird immer eingefügt, für den Fall, daß der ganze Wert 0 ist.
	nHelpVal = nValue/nDiff;			// Ganzzahldivision bringt die Wertigkeit der letzten Stelle
	*pText = '0'+nHelpVal;			// Den Ascii-Wert der Wertigkeit in den String einfügen
	pText++;						// String-Pointer ein Zeichen weiterstellen
	// String-Terminierung
	*pText = 0;
	return 0;
}
