/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetBitUsint.c
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

/* Liest ein Bit aus einem Usint */
plcbit BrbGetBitUsint(unsigned char nValue, unsigned short nBitNumber)
{
	BOOL bResult = 0;
	if(nBitNumber <= 7)
	{
		bResult = (nValue & ((USINT)1 << nBitNumber)) > 0;
	}
	return bResult;
}
