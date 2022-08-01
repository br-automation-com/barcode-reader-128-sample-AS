
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"

#ifdef __cplusplus
	};
#endif

/* Gibt ein Bit eines bitcodierten Byte-Arrays zurück */
plcbit BrbGetByteArrayBit(unsigned long pByteArray, unsigned long nIndexMax, unsigned long nBitNumber)
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
		}
	}
	return bResult;
}
