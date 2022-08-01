/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringAppend.c
 * Author: niedermeierr
 * Created: May 02, 2016
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

/* Hängt eine Zeichenfolge an einen Text (Ersatz für brwcscat) */
unsigned long BrbWcStringAppend(plcwstring* pText, plcwstring* pTextAppend, unsigned long* pTextOffset)
{
	UDINT nAppendLen = brwcslen(pTextAppend);
	UDINT nAppendByteLen = nAppendLen * 2;
	memcpy(pText + *pTextOffset, pTextAppend, nAppendByteLen);
	*pTextOffset += nAppendLen;
	*(pText + *pTextOffset) = 0; // String-Abschluss
	return nAppendLen;
}
