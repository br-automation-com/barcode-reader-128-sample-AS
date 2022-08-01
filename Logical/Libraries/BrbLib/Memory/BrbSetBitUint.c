/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetBitUint.c
 * Author: niedermeierr
 * Created: June 25, 2013
 ********************************************************************
 * Implementation of library BrbLib
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"

#ifdef __cplusplus
	};
#endif

/* Setzt oder löscht ein Bit in einem Uint */
plcbit BrbSetBitUint(unsigned short* pValue, unsigned short nBitNumber, plcbit bBit)
{
	BOOL bResult = 0;
	if(nBitNumber <= 15)
	{
		bResult = BrbGetBitUint(*pValue, nBitNumber);
		if(bBit == 1)
		{
			*pValue = *pValue | ((UINT)1 << nBitNumber);
		}
		else
		{
			*pValue = *pValue & ~((UINT)1 << nBitNumber);
		}
	}
	return bResult;
}
