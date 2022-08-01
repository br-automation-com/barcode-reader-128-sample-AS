
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"

#ifdef __cplusplus
	};
#endif
/* Setzte Bits in einem bitcodierten Byte-Array */
plcbit BrbSetByteArrayBits(unsigned long pByteArray, unsigned long nIndexMax, unsigned long nBitNumberFrom, unsigned long nBitNumberTo, plcbit bBit)
{
	if(pByteArray != 0)
	{
		if(nBitNumberTo < nBitNumberFrom)
		{
			UDINT nHelp = nBitNumberTo;
			nBitNumberTo = nBitNumberFrom;
			nBitNumberFrom = nHelp;
		}
		UDINT nBitNumber = 0;
		for(nBitNumber=nBitNumberFrom; nBitNumber<=nBitNumberTo; nBitNumber++)
		{
			UINT nByteIndex = (UINT)(nBitNumber / 8);
			if(nByteIndex <= nIndexMax)
			{
				UINT nByteBitNumber = nBitNumber - (nByteIndex * 8);
				USINT* pByte = ((USINT*)pByteArray) + nByteIndex;
				if(bBit == 1)
				{
					*pByte = *pByte | ((USINT)1 << nByteBitNumber);
				}
				else
				{
					*pByte = *pByte & ~((USINT)1 << nByteBitNumber);
				}
			}
			else
			{
				break;
			}
		}
	}
	return bBit;
}
