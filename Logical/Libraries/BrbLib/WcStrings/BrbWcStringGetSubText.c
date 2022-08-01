/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringGetSubText.c
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
unsigned long BrbWcStringGetSubText(plcwstring* pText, unsigned long nIndex, unsigned long nLen, plcwstring* pSubText)
{
	UDINT nByteLen = nLen * 2;
	memcpy(pSubText, (pText + nIndex), nByteLen);
	pSubText[nLen] = 0; // NullTerminierung
	return (UDINT)(pText + nIndex + nLen);
}
