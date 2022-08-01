/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringFormatFractionDigits.c
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

/* Formatiert einen Real-String-Wert auf Nachkommastellen */
unsigned short  BrbStringFormatFractionDigits(plcstring* pValue, unsigned long nValueSize, unsigned short nFractionsDigits)
{
	STRING sTemp[nValueSize+1];
	BrbStringConvertRealFromExp(pValue, sTemp, sizeof(sTemp));
	brsstrcpy((UDINT)pValue, (UDINT)&sTemp);
	UDINT nValueLen = strlen(pValue);
	STRING* pDot = BrbStringGetAdrOf(pValue, ".", nValueLen);
	if(pDot > pValue)
	{
		// Punkt gefunden
		UDINT nLenAfterDot = (pValue + nValueLen) - pDot - 1;
		if(nLenAfterDot > nFractionsDigits)
		{
			if(nFractionsDigits > 0)
			{
				*(pDot + nFractionsDigits + 1) = 0;
			}
			else
			{
				*pDot = 0;
			}
		}
		else if(nLenAfterDot < nFractionsDigits)
		{
			UDINT nMissingDigits = nFractionsDigits - nLenAfterDot;
			UDINT nMissingChars = 0;
			for(nMissingChars=0; nMissingChars<nMissingDigits; nMissingChars++)
			{
				strcat(pValue, "0");
			}
		}
	}
	else
	{
		// Punkt nicht gefunden
		if(nFractionsDigits > 0)
		{
			strcat(pValue, ".");
			UDINT nMissingChars = 0;
			for(nMissingChars=0; nMissingChars<nFractionsDigits; nMissingChars++)
			{
				strcat(pValue, "0");
			}
		}
	}
	return eBRB_ERR_OK;
}
