/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetBitMaskUint.c
 * Author: niedermeierr
 * Created: May 16, 2014
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

/* Setzt oder löscht eine Bitmaske in einem Uint */
plcbit BrbSetBitMaskUint(unsigned short* pValue, unsigned short nBitMask, plcbit bSet)
{
	BOOL bResult = 0;
	bResult = BrbGetBitMaskUint(*pValue, nBitMask);
	if(bSet == 1)
	{
		*pValue = *pValue | nBitMask;
	}
	else
	{
		*pValue = *pValue & ~nBitMask;
	}
	return bResult;
}
