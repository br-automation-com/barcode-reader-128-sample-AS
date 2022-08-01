
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
/* Wandelt einen Udint-Wert in einen Binär-String */
unsigned short BrbUdintToBin(unsigned long nValue, plcstring* pBin, unsigned long nBinSize, plcbit bWithPraefix)
{
	memset(pBin, 0, nBinSize);
	UDINT nCharIndex = 0;
	if(bWithPraefix == 1)
	{
		strcat(pBin, "%");
		nCharIndex += 1;
	}
	INT nBitNumber = 0;
	for(nBitNumber=31; nBitNumber>=0; nBitNumber--)
	{
		if(nCharIndex < nBinSize)
		{
			BOOL bBit = (nValue & ((UDINT)1 << nBitNumber)) > 0;
			if(bBit == 0)
			{
				*(pBin + nCharIndex) = 48;
			}
			else
			{
				*(pBin + nCharIndex) = 49;
			}
			nCharIndex++;
		}
		else
		{
			break;
		}
	}
	return 0;
}
