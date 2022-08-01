/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetBitMaskUdint.c
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

/* Liest eine Bitmaske aus einem Udint */
plcbit BrbGetBitMaskUdint(unsigned long nValue, unsigned long nBitMask)
{
	UDINT nTest = nValue & nBitMask;
	BOOL bResult = (nTest == nBitMask);
	return bResult;
}
