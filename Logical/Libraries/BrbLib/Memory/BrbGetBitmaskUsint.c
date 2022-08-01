/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetBitMaskUsint.c
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

/* Liest eine Bitmaske aus einem Usint */
plcbit BrbGetBitMaskUsint(unsigned char nValue, unsigned char nBitMask)
{
	USINT nTest = nValue & nBitMask;
	BOOL bResult = (nTest == nBitMask);
	return bResult;
}
