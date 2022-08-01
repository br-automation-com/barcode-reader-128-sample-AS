/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbHexToDint.c
 * Author: niedermeierr
 * Created: January 19, 2017
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

/* Wandelt einen Hex-String in einen Dint */
signed long BrbHexToDint(plcstring* pHex)
{
	DINT nResult = 0;
	UINT nHexLen = strlen(pHex);
	UDINT nValueUdint = BrbHexToUdint(pHex);
	if(nHexLen < 8)
	{
		UDINT nMaxValuePositive = pow(2, nHexLen*4) - 1;
		UDINT nPolarityLimit = ((nMaxValuePositive+1) / 2) - 1;
		if(nValueUdint > nPolarityLimit)
		{
			// Zahl ist negativ
			nResult = nValueUdint - nMaxValuePositive - 1;
		}
		else
		{
			// Zahl ist positiv
			nResult = nValueUdint;
		}
	}
	else
	{
		// Sonderbehandlung, weil die Berechnungen sonst durch Überlauf falsch wären
		if(nValueUdint > 2147483647)
		{
			// Zahl ist negativ
			nResult = nValueUdint - 4294967295UL - 1;
		}
		else
		{
			// Zahl ist positiv
			nResult = nValueUdint;
		}
	}
	return nResult;
}
