/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbXmlGetTagText.c
 * Author: niedermeierr
 * Created: June 11, 2013
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

/* Gibt den Text zwischen zwei Xml-Tags zurück */
unsigned long BrbXmlGetTagText(plcstring* pStartTag, plcstring* pEndTag, unsigned long pStart, unsigned long pEnd, plcstring* pText, unsigned long nTextSize)
{
	UDINT nResult = 0;
	// Zu füllenden Text löschen
	memset(pText, 0, nTextSize);
	// Start-Tag suchen
	STRING* pStartTagAdr = 0;
	if(pStart > 0 && pEnd > 0 && pEnd > pStart)
	{
		UDINT nSearchLen = pEnd - pStart;
		pStartTagAdr = BrbStringGetAdrOf((STRING*)pStart, pStartTag, nSearchLen);
		if(pStartTagAdr != 0)
		{
			UDINT nStartTagLen = strlen(pStartTag);
			nResult = (UDINT)(pStartTagAdr + nStartTagLen);
			// Start-Tag gefunden -> End-Tag suchen
			STRING* pEndTagAdr = 0;
			pEndTagAdr = BrbStringGetAdrOf(pStartTagAdr, pEndTag, pEnd - (UDINT)pStartTagAdr);
			if(pEndTagAdr != 0)
			{
				// End-Tag gefunden -> Zeichen zwischen Start- und End-Tag durchtickern
				USINT* pCharSource;
				UDINT nCharDestIndex = 0;
				for(pCharSource=(USINT*)pStartTagAdr + nStartTagLen; pCharSource<(USINT*)pEndTagAdr; pCharSource++)
				{
					// Nur Ascii-Zeichen übernehmen
					if((*pCharSource >= 32) && (*pCharSource <= 126))
					{
						USINT* pCharDest = (USINT*)pText + nCharDestIndex;
						*pCharDest = *pCharSource;
						nCharDestIndex += 1;
						if(nCharDestIndex >= nTextSize)
						{
							// Größe des zu füllenden Strings darf nicht überschritten werden
							break;
						}
					}
				}
				UDINT nEndTagLen = strlen(pEndTag);
				nResult = (UDINT)(pEndTagAdr + nEndTagLen);
			}
		}
	}
	return nResult;
}
