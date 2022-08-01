
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

/* Wiederholt eine Zeichenfolge bis zu einer angegebenen Zeichenanzahl */
unsigned long BrbStringRepeat(plcstring* pText, unsigned long nTextSize, plcstring* pRepeat, unsigned long nLen)
{
	UDINT nCharCount = 0;
	UDINT nTextOffset = strlen(pText);
	UDINT nRepeatLen = strlen(pRepeat);
	// Länge limitieren
	if(nLen >= nTextSize)
	{
		nLen = nTextSize-1;
	}
	// Gesamten Wiederholtext wiederholend anhängen
	while(nTextOffset+nRepeatLen <= nLen)
	{
		BrbStringAppend(pText, pRepeat, &nTextOffset);
		nCharCount += nRepeatLen;
	}
	// Restliche Zeichen auffüllen
	DINT nCharCountLeft = (DINT)nLen - (DINT)nTextOffset;
	if(nCharCountLeft > 0)
	{
		UINT nRepeatCharIndex = 0;
		for(nRepeatCharIndex=0; nRepeatCharIndex<nCharCountLeft; nRepeatCharIndex++)
		{
			*(pText + nTextOffset) = *(pRepeat + nRepeatCharIndex);
			nTextOffset++;
			nCharCount++;
		}
		*(pText + nTextOffset) = 0; // String-Abschluss
	}
	return nCharCount;
}
