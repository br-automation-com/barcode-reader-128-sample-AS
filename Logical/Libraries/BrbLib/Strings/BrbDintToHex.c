/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDintToHex.c
 * Author: niedermeierr
 * Created: January 17, 2017
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

/* Wandelt einen DINT in einen Hex-String */
unsigned short BrbDintToHex(signed long nValue, plcstring* pHex, unsigned long nHexSize, plcbit bWithPraefix)
{
	// Die Bitmuster stimmen auch bei negativen Zahlen
	UDINT nValueUdint = (UDINT)nValue;
	BrbUdintToHex(nValueUdint, pHex, nHexSize, bWithPraefix);
	return 0;
}
