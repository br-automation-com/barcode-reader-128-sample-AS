/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbHexToUdint.c
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
#include <math.h>
#include <string.h>

#ifdef __cplusplus
	};
#endif

/* Wandelt einen Hex-String in einen Udint */
unsigned long BrbHexToUdint(plcstring* pHex)
{
	UDINT nResult = 0;
	UINT nBitDigit = 0;
	UINT nHexLen = strlen((STRING*)pHex);
	INT nCharIndex = 0;
	for(nCharIndex = nHexLen-1; nCharIndex >= 0; nCharIndex--)
	{
		USINT* pChar = (USINT*)pHex+nCharIndex;
		UDINT nDigitValue = 0;
		LREAL rDigitValency = 0;
		if(*pChar >= 48 && *pChar <= 57)
		{
			nDigitValue = (*pChar-48);
		}
		else if(*pChar >= 65 && *pChar <= 70)
		{
			nDigitValue = (*pChar-55);
		}
		else if(*pChar >= 97 && *pChar <= 102)
		{
			nDigitValue = (*pChar-87);
		}
		rDigitValency = pow(2, nBitDigit);
		LREAL rResult = (LREAL)nResult + ((LREAL)nDigitValue * rDigitValency);
		if(rResult > (LREAL)4294967295UL)
		{
			// Ergebnis zu groß für UDINT
			nResult = 4294967295UL; // Maximalen Wert zurückgeben
			break;
		}
		nResult = nResult + (nDigitValue * (UDINT)rDigitValency);
		nBitDigit += 4;
	}
	return nResult;
}
