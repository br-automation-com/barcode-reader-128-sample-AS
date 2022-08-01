/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetTimespanT.c
 * Author: niedermeierr
 * Created: May 18, 2021
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

/* Setzt eine TIME-Zeitspanne */
plctime BrbSetTimespanT(plcstring* pTimeText)
{
	TIME tTime = 0;
	if(pTimeText != 0)
	{
		UDINT nTextLen = strlen(pTimeText);
		UDINT nIndex = 0;
		UDINT nIndexStart = 0;
		USINT* pChar = 0;
		USINT nSign = +1;
		STRING sValue[128];
		memset((USINT*)&sValue, 0, sizeof(sValue));
		UDINT nValueLen = 0;
		UDINT nDays = 0;
		UDINT nHours = 0;
		UDINT nMinutes = 0;
		UDINT nSeconds = 0;
		UDINT nMilliseconds = 0;
		if(BrbStringStartsWith(pTimeText, "T#-") == 1)
		{
			nSign = -1;
			nIndexStart += 3;
		}
		else if(BrbStringStartsWith(pTimeText, "T#") == 1)
		{
			nIndexStart += 2;
		}
		else if(BrbStringStartsWith(pTimeText, "-") == 1)
		{
			nSign = -1;
			nIndexStart += 1;
		}
		for(nIndex=nIndexStart; nIndex<nTextLen; nIndex++)
		{
			pChar = (USINT*)(pTimeText + nIndex);
			if(*pChar >= 48 && *pChar <= 57) // 0..9
			{
				sValue[nValueLen] = *pChar;	// Zahl in den Wert übernehmen
				nValueLen += 1;
				sValue[nValueLen] = 0; // Nullterminierung
			}
			else if(*pChar == 100) // d
			{
				if(nValueLen > 0)
				{
					nDays = BrbAsciiToUdint(sValue);
					memset((USINT*)&sValue, 0, sizeof(sValue));
					nValueLen = 0;
				}
			}
			else if(*pChar == 104) // h
			{
				if(nValueLen > 0)
				{
					nHours = BrbAsciiToUdint(sValue);
					memset((USINT*)&sValue, 0, sizeof(sValue));
					nValueLen = 0;
				}
			}
			else if(*pChar == 109 && *(pChar+1) != 115) // m
			{
				if(nValueLen > 0)
				{
					nMinutes = BrbAsciiToUdint(sValue);
					memset((USINT*)&sValue, 0, sizeof(sValue));
					nValueLen = 0;
				}
			}
			else if(*pChar == 115 && (nIndex == 0 || *(pChar-1) != 109)) // s
			{
				if(nValueLen > 0)
				{
					nSeconds = BrbAsciiToUdint(sValue);
					memset((USINT*)&sValue, 0, sizeof(sValue));
					nValueLen = 0;
				}
			}
			else if(*pChar == 109 && *(pChar+1) == 115) // ms
			{
				if(nValueLen > 0)
				{
					nMilliseconds = BrbAsciiToUdint(sValue);
					memset((USINT*)&sValue, 0, sizeof(sValue));
					nValueLen = 0;
					nIndex++; // nächstes 's' überspringen
				}
			}
		}
		tTime = BrbSetTimespan(nSign, nDays, nHours, nMinutes, nSeconds, nMilliseconds); 
	}
	return tTime;
}
