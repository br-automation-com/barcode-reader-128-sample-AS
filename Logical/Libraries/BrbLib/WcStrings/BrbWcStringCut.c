/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbWcStringCut.c
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

/* Schneidet eine Zeichenfolge aus einem Text heraus */
unsigned long BrbWcStringCut(plcwstring* pText, unsigned long nCutIndex, unsigned long nCutLen, plcwstring* pCut)
{
	UDINT nResult = 0;
	UDINT nCutByteLen = nCutLen * 2;
	UDINT nTextLen = brwcslen(pText);
	if(nCutIndex+nCutLen > nTextLen)
	{
		nCutLen = nTextLen-nCutIndex;
	}
	if(pCut != 0)
	{
		// Herauszuschneidenden String kopieren
		memcpy(pCut, (pText + nCutIndex), nCutByteLen);
		*(pCut + nCutLen) = 0;
	}
	UDINT nRestByteLen = (nTextLen - nCutIndex - nCutLen + 1) * 2;
	// Zeichen nach dem Cut nach vorne ziehen
	memmove((pText + nCutIndex), (pText + nCutIndex + nCutLen), nRestByteLen);
	nResult = (UDINT)(pText + nCutIndex + nCutLen);
	return nResult;
}
