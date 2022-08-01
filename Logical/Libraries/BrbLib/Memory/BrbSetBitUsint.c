/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetBitUsint.c
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

/* Setzt ein Bit in einem Usint */
plcbit BrbSetBitUsint(unsigned char* pValue, unsigned short nBitNumber, plcbit bBit)
{
	BOOL bResult = 0;
	if(nBitNumber <= 7)
	{
		bResult = BrbGetBitUsint(*pValue, nBitNumber);
		if(bBit == 1)
		{
			*pValue = *pValue | ((USINT)1 << nBitNumber);
		}
		else
		{
			*pValue = *pValue & ~((USINT)1 << nBitNumber);
		}
	}
	return bResult;
}
