/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetBitMaskUint.c
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

/* Liest eine Bitmaske aus einem Uint */
plcbit BrbGetBitMaskUint(unsigned short nValue, unsigned short nBitMask)
{
	UINT nTest = nValue & nBitMask;
	BOOL bResult = (nTest == nBitMask);
	return bResult;
}
