
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

	#include "BrbLib.h"

#ifdef __cplusplus
	};
#endif

/* Setzt ein Bit in einem bitcodierten Byte-Array */
plcbit BrbSetByteArrayBit(unsigned long pByteArray, unsigned long nIndexMax, unsigned long nBitNumber, plcbit bBit)
{
	BOOL bResult = 0;
	if(pByteArray != 0)
	{
		UINT nByteIndex = (UINT)(nBitNumber / 8);
		if(nByteIndex <= nIndexMax)
		{
			UINT nByteBitNumber = nBitNumber - (nByteIndex * 8);
			USINT* pByte = ((USINT*)pByteArray) + nByteIndex;
			bResult = (*pByte & ((USINT)1 << nByteBitNumber)) > 0;
			if(bBit == 1)
			{
				*pByte = *pByte | ((USINT)1 << nByteBitNumber);
			}
			else
			{
				*pByte = *pByte & ~((USINT)1 << nByteBitNumber);
			}
		}
	}
	return bResult;
}
