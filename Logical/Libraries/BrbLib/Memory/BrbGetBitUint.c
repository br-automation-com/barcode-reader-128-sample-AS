/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetBitUint.c
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

/* Liest ein Bit aus einem Uint */
plcbit BrbGetBitUint(unsigned short nValue, unsigned short nBitNumber)
{
	BOOL bResult = 0;
	if(nBitNumber <= 15)
	{
		bResult = (nValue & ((UINT)1 << nBitNumber)) > 0;
	}
	return bResult;
}
