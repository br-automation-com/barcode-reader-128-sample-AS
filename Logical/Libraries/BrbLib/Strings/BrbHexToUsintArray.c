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
#include <string.h>

#ifdef __cplusplus
	};
#endif

/* Wandelt einen Hex-String in ein Usint-Array */
unsigned short BrbHexToUsintArray(plcstring* pHex, unsigned char* pArray, signed long nArrayLength, plcbit bDescending)
{
	UINT nStatus = eBRB_ERR_OK;
	if(pHex != 0 && pArray != 0)
	{
		if(nArrayLength > 0)
		{
			memset(pArray, 0, nArrayLength);
			UDINT nLengthHex = brsstrlen((UDINT)pHex);
			if(bDescending == 0)
			{
				BOOL bExpandFirstHex = 0;
				if(nLengthHex % 2 != 0)
				{
					// Ungerade Anzahl von Hex-Zeichen -> dem ersten Hex-Wert eine 0 voranstellen
					bExpandFirstHex = 1;
				}
				BOOL bLeave = 0;
				UDINT nCharIndex = 0;
				DINT nByteIndex = 0;
				STRING sValueHexUsint[4];
				UDINT nValueUdint = 0;
				do
				{
					if(nCharIndex >= nLengthHex || nByteIndex >= nArrayLength)
					{
						bLeave = 1;
					}
					else
					{
						// Das erste Byte könnte aus nur einem Hex-Zeichen bestehen
						if(nCharIndex == 0 && bExpandFirstHex == 1)
						{
							// Dem ersten Hex-Wert eine 0 voranstellen
							BrbStringGetSubText(pHex, nCharIndex, 1, sValueHexUsint);
							BrbStringInsert(sValueHexUsint, 0, "0");
							nValueUdint = BrbHexToUdint(sValueHexUsint);
							*(pArray + nByteIndex) = (USINT)nValueUdint;
							nCharIndex += 1;
						}
						else
						{
							BrbStringGetSubText(pHex, nCharIndex, 2, sValueHexUsint);
							nValueUdint = BrbHexToUdint(sValueHexUsint);
							*(pArray + nByteIndex) = (USINT)nValueUdint;
							nCharIndex += 2;
						}
						nByteIndex += 1;
					}
				} while(bLeave == 0);
			}
			else
			{
				BOOL bLeave = 0;
				DINT nCharIndex = nLengthHex - 2;
				DINT nByteIndex = 0;
				STRING sValueHexUsint[4];
				UDINT nValueUdint = 0;
				do
				{
					if(nCharIndex < -1 || nByteIndex >= nArrayLength)
					{
						bLeave = 1;
					}
					else if(nCharIndex == -1)
					{
						// Das letzte Byte könnte aus nur einem Hex-Zeichen bestehen
						nCharIndex = 0;
						BrbStringGetSubText(pHex, nCharIndex, 1, sValueHexUsint);
						BrbStringInsert(sValueHexUsint, 0, "0");
						nValueUdint = BrbHexToUdint(sValueHexUsint);
						*(pArray + nByteIndex) = (USINT)nValueUdint;
						bLeave = 1;
					}
					else
					{
						BrbStringGetSubText(pHex, nCharIndex, 2, sValueHexUsint);
						nValueUdint = BrbHexToUdint(sValueHexUsint);
						*(pArray + nByteIndex) = (USINT)nValueUdint;
						nCharIndex -= 2;
						nByteIndex += 1;
					}
				} while(bLeave == 0);
			}
		}
		else
		{
			nStatus = eBRB_ERR_INVALID_PARAMETER;
		}
	}
	else
	{
		nStatus = eBRB_ERR_NULL_POINTER;
	}
	return nStatus;
}
