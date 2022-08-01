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
	BOOL bFirstDigitFound; // Erste g�ltige Stelle erkannt
	bFirstDigitFound = 0;
	// Die Schleife beginnt mit der gr��ten Stelle und geht runter bis zur zweitletzten Stelle (Wertigkeit 10-90)
	for(nDiff = 1000000000; nDiff > 1; )
	{
		nHelpVal = nValue/nDiff;		// Ganzzahldivision bringt die Wertigkeit der gew�nschten Stelle
		// F�hrende Nullen werden unterdr�ckt
		if(nHelpVal > 0 || bFirstDigitFound == 1)
		{
			bFirstDigitFound = 1;	// Ab der ersten Stelle, deren Wertigkeit gr��er 0 ist, m�ssen alle Werte �bernommen werden
			*pText = '0'+nHelpVal;	// Den Ascii-Wert der Wertigkeit in den String einf�gen
			pText++;				// String-Pointer ein Zeichen weiterstellen

		}
		nValue = nValue % nDiff;		// Rest der Ganzzahldivision brint den Wert ohne die schon behandelte Stelle (f�r n�chsten Durchlauf)
		nDiff = nDiff/10;				// Teiler um eine Stelle verringern (f�r n�chsten Durchlauf)
	}
	// Die letzte Stelle wird immer eingef�gt, f�r den Fall, da� der ganze Wert 0 ist.
	nHelpVal = nValue/nDiff;			// Ganzzahldivision bringt die Wertigkeit der letzten Stelle
	*pText = '0'+nHelpVal;			// Den Ascii-Wert der Wertigkeit in den String einf�gen
	pText++;						// String-Pointer ein Zeichen weiterstellen
	// String-Terminierung
	*pText = 0;
	return 0;
}
