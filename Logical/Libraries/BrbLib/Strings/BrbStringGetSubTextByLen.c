/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringGetSubTextByLen.c
 * Author: niedermeierr
 * Created: June 17, 2013
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
plcstring* BrbStringGetSubTextByLen(plcstring* pStart, unsigned long nLen, plcstring* pSubText)
{
	memcpy(pSubText, pStart, nLen);
	pSubText[nLen] = 0; // NullTerminierung
	return (STRING*)(pStart + nLen);
}
