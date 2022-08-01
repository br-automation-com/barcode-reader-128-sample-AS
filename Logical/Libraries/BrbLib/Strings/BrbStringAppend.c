/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringAppend.c
 * Author: niedermeierr
 * Created: February 17, 2016
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

/* H�ngt eine Zeichenfolge an einen Text (Ersatz f�r strcat) */
unsigned long BrbStringAppend(plcstring* pText, plcstring* pTextAppend, unsigned long* pTextOffset)
{
	UDINT nAppendLen = strlen(pTextAppend);
	memcpy(pText + *pTextOffset, pTextAppend, nAppendLen);
	*pTextOffset += nAppendLen;
	*(pText + *pTextOffset) = 0; // String-Abschluss
	return nAppendLen;
}
