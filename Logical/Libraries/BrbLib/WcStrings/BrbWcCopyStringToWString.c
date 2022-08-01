/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcCopyStringToWString.c
 * Author: niedermeierr
 * Created: April 30, 2016
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

/* Kopiert einen STRING auf einen WSTRING */
unsigned short BrbWcCopyStringToWString(plcwstring* pWcString, plcstring* pString)
{
	brwcsconv((USINT*)pWcString, (USINT*)pString, brwU8toUC);
	return eBRB_ERR_OK;
}
