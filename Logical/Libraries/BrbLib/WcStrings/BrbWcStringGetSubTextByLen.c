/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringGetSubTextByLen.c
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

/* Gibt eine Zeichenfolge innerhalb eines Textes zurück */
unsigned long BrbWcStringGetSubTextByLen(plcwstring* pStart, unsigned long nLen, plcwstring* pSubText)
{
	UDINT nByteLen = nLen * 2;
	memcpy(pSubText, pStart, nByteLen);
	pSubText[nLen] = 0; // NullTerminierung
	return (UDINT)(pStart + nLen);
}
