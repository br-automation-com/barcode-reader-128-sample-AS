/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringGetSubText.c
 * Author: niedermeierr
 * Created: July 05, 2013
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
plcstring* BrbStringGetSubText(plcstring* pText, unsigned long nIndex, unsigned long nLen, plcstring* pSubText)
{
	memcpy(pSubText, (pText + nIndex), nLen);
	pSubText[nLen] = 0; // NullTerminierung
	return (STRING*)(pText + nIndex + nLen);
}
