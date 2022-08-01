/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetBitUdint.c
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

/* Liest ein Bit aus einem Udint */
plcbit BrbGetBitUdint(unsigned long nValue, unsigned short nBitNumber)
{
	BOOL bResult = 0;
	if(nBitNumber <= 31)
	{
		bResult = (nValue & ((UDINT)1 << nBitNumber)) > 0;
	}
	return bResult;
}
