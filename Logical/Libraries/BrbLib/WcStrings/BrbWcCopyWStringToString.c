/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcCopyWStringToString.c
 * Author: niedermeierr
 * Created: Septemer 13, 2021
 ********************************************************************
 * Implementation of library BrbLib
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"
#include <string.h>

#ifdef __cplusplus
	};
#endif

/* Kopiert einen WSTRING auf einen STRING */
unsigned short BrbWcCopyWStringToString(plcstring* pString, plcwstring* pWcString)
{
	brwcsconv((USINT*)pWcString, (USINT*)pString, brwUCtoU8);
	return eBRB_ERR_OK;
}
