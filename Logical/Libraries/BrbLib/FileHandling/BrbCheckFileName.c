/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbCheckFileName.c
 * Author: niedermeierr
 * Created: July 25, 2013
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

/* Ersetzt Sonderzeichen in einem Datei-Namen mit Unterstrichen */
unsigned short BrbCheckFileName(plcstring* pFileName)
{
	// Prüfung der Parameter
	if(pFileName == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	// Check
	UINT nResult = 0;
	UDINT nLen = strlen(pFileName);
	DINT nCharIndex = 0;
	USINT* pChar = 0;
	UINT nDotCount = 0;
	// Text von hinten nach vorne durchsuchen
	for(nCharIndex=nLen-1; nCharIndex>=0; nCharIndex--)
	{
		pChar = (USINT*)(pFileName + nCharIndex);
		if(*pChar == 46)
		{
			// Alle Punkte außer dem letzten werden ersetzt
			nDotCount += 1;
			if(nDotCount > 1)
			{
				*pChar = 95;
				nResult += 1;
			}
		}
		//										-									0 							9									A								Z									_									a								z
		else if(!((*pChar == 45) || (*pChar >= 48 && *pChar <= 57) || (*pChar >= 65 && *pChar <= 90) || (*pChar == 95) || (*pChar >= 97 && *pChar <= 122)))
		{
			// Sonderzeichen werden ersetzt
			*pChar = 95;
			nResult += 1;
		}
	}
	return nResult;
}
