/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbUdintToHex.c
 * Author: niedermeierr
 * Created: July 14, 2021
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

/* Wandelt einen Usint in einen Hex-String */
unsigned short BrbUsintToHex(unsigned char nValue, plcstring* pHex, unsigned long nHexSize, plcbit bWithPraefix)
{
	memset(pHex, 0, nHexSize);
	if(bWithPraefix == 1)
	{
		strcat(pHex, "0x");
	}
	DINT nDigit = 0;
	for(nDigit=1; nDigit>=0; nDigit--)
	{
		UDINT nHelp1 = 0xF << nDigit * 4;
		UDINT nHelp2 = nValue & nHelp1;
		UDINT nHelp3 = nHelp2 >> nDigit * 4;
		
		if(nHelp3 <= 9)
		{
			STRING sChar[2];
			sChar[0] = 48 + nHelp3;
			sChar[1] = 0;
			strcat(pHex, sChar);
		}
		else
		{
			STRING sChar[2];
			sChar[0] = 65 + nHelp3 - 10;
			sChar[1] = 0;
			strcat(pHex, sChar);
		}
	}
	return eBRB_ERR_OK;
}
