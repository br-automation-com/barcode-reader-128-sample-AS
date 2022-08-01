/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetTimeTextMs.c
 * Author: niedermeierr
 * Created: April 29, 2016
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

/* Gibt die angebenene Millisekunden-Zeit als Text zurück */
unsigned short BrbGetTimeTextMs(signed long nMilliseconds, plcstring* pText, unsigned long nTextSize, plcstring* pFormat, plcbit bClip)
{
	// Prüfung der Parameter
	if(pText == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	if(nTextSize == 0)
	{
		return eBRB_ERR_INVALID_PARAMETER;
	}
	if(pFormat == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	// Text ermitteln
	memset((USINT*)pText, 0, nTextSize);
	strcpy(pText, pFormat);
	STRING sItoA[20];
	memset((USINT*)sItoA, 0, sizeof(sItoA));
	if(BrbStringGetAdrOf(pText, "dd", 0) != 0)
	{
		DINT nDays = (DINT)(nMilliseconds / 86400000);
		nMilliseconds -= nDays * 86400000;
		brsitoa(nDays, (UDINT)&sItoA);
		if(nDays < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "dd", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "d", 0) != 0)
	{
		DINT nDays = (DINT)(nMilliseconds / 86400000);
		nMilliseconds -= nDays * 86400000;
		brsitoa(nDays, (UDINT)&sItoA);
		BrbStringReplace(pText, "d", sItoA);
	}
	else if(bClip == 1)
	{
		DINT nDays = (DINT)(nMilliseconds / 86400000);
		nMilliseconds -= nDays * 86400000;
	}
	if(BrbStringGetAdrOf(pText, "hh", 0) != 0)
	{
		DINT nHours = (DINT)(nMilliseconds / 3600000);
		nMilliseconds -= nHours * 3600000;
		brsitoa(nHours, (UDINT)&sItoA);
		if(nHours < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "hh", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "h", 0) != 0)
	{
		DINT nHours = (DINT)(nMilliseconds / 3600000);
		nMilliseconds -= nHours * 3600000;
		brsitoa(nHours, (UDINT)&sItoA);
		BrbStringReplace(pText, "h", sItoA);
	}
	else if(bClip == 1)
	{
		DINT nHours = (DINT)(nMilliseconds / 3600000);
		nMilliseconds -= nHours * 3600000;
	}
	if(BrbStringGetAdrOf(pText, "MM", 0) != 0)
	{
		DINT nMinutes = (DINT)(nMilliseconds / 60000);
		nMilliseconds -= nMinutes * 60000;
		brsitoa(nMinutes, (UDINT)&sItoA);
		if(nMinutes < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "MM", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "M", 0) != 0)
	{
		DINT nMinutes = (DINT)(nMilliseconds / 60000);
		nMilliseconds -= nMinutes * 60000;
		brsitoa(nMinutes, (UDINT)&sItoA);
		BrbStringReplace(pText, "M", sItoA);
	}
	else if(bClip == 1)
	{
		DINT nMinutes = (DINT)(nMilliseconds / 60000);
		nMilliseconds -= nMinutes * 60000;
	}
	if(BrbStringGetAdrOf(pText, "ss", 0) != 0)
	{
		DINT nSeconds = (DINT)(nMilliseconds / 1000);
		nMilliseconds -= nSeconds * 1000;
		brsitoa(nSeconds, (UDINT)&sItoA);
		if(nSeconds < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "ss", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "s", 0) != 0)
	{
		DINT nSeconds = (DINT)(nMilliseconds / 1000);
		nMilliseconds -= nSeconds * 1000;
		brsitoa((DINT)nSeconds, (UDINT)&sItoA);
		BrbStringReplace(pText, "s", sItoA);
	}
	else if(bClip == 1)
	{
		DINT nSeconds = (DINT)(nMilliseconds / 1000);
		nMilliseconds -= nSeconds * 1000;
	}
	if(BrbStringGetAdrOf(pText, "mil", 0) != 0)
	{
		brsitoa(nMilliseconds, (UDINT)&sItoA);
		if(nMilliseconds < 10)
		{
			BrbStringInsert(sItoA, 0, "00");
		}
		else if(nMilliseconds < 100)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "mil", sItoA);
	}
	return eBRB_ERR_OK;
}
