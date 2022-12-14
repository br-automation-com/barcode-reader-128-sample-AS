/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringIsNumerical.c
 * Author: niedermeierr
 * Created: March 18, 2015
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

/* Gibt zur?ck, ob ein Text eine Nummer enth?lt */
plcbit BrbStringIsNumerical(plcstring* pText)
{
	BOOL bResult = 1;
	UDINT nTextLen = strlen(pText);
	if(nTextLen == 0)
	{
		bResult = 0;
	}
	else
	{
		UDINT nIndex;
		USINT* pChar;
		UINT nCountSeparator = 0;
		DINT nIndexExp = -1;
		for(nIndex=0; nIndex<nTextLen; nIndex++)
		{
			pChar = (USINT*)(pText + nIndex);
			if(*pChar == 43)
			{
				// Plus
				if(nIndex != 0 && nIndexExp != (DINT)nIndex-1)
				{
					// Nicht am Anfang oder nach "e"
					bResult = 0;
					break;
				}
			}
			else if(*pChar == 45)
			{
				// Minus
				if(nIndex != 0 && nIndexExp != (DINT)nIndex-1)
				{
					// Nicht am Anfang oder nach "e"
					bResult = 0;
					break;
				}
			}
			else if(*pChar == 44)
			{
				// Komma
				nCountSeparator++;
				if(nCountSeparator > 1)
				{
					// Mehr als 1 Separartor
					bResult = 0;
					break;
				}
			}
			else if(*pChar == 46)
			{
				// Punkt
				nCountSeparator++;
				if(nCountSeparator > 1)
				{
					// Mehr als 1 Separartor
					bResult = 0;
					break;
				}
			}
			else if(*pChar == 101)
			{
				// e
				if(nIndexExp == -1)
				{
					nIndexExp = nIndex;
					if(nIndex >= nTextLen-1)
					{
						// Nach "e" folgt nichts
						bResult = 0;
						break;
					}
				}
				else
				{
					// Zweites "e"
					bResult = 0;
					break;
				}
			}
			else if(!(*pChar >= 48 && *pChar <= 57))
			{
				bResult = 0;
				break;
			}
		}
	}
	return bResult;
}
