/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetTimeText.c
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

/* Gibt die angegebene RTC-Zeit als Text zurück */
unsigned short BrbGetTimeText(struct RTCtime_typ* pTime, plcstring* pText, unsigned long nTextSize, plcstring* pFormat)
{
	// Prüfung der Parameter
	if(pTime == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
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
	// Alle Stellen werden mit führenden Nullen aufgefüllt
	memset((USINT*)pText, 0, nTextSize);
	strcpy(pText, pFormat);
	STRING sItoA[20];
	memset((USINT*)sItoA, 0, sizeof(sItoA));
	if(BrbStringGetAdrOf(pText, "yyyy", 0) != 0)
	{
		brsitoa((DINT)pTime->year, (UDINT)&sItoA);
		BrbStringPadLeft(sItoA, "0", 4);
		BrbStringReplace(pText, "yyyy", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "yy", 0) != 0)
	{
		pTime->year -= 2000;
		brsitoa((DINT)pTime->year, (UDINT)&sItoA);
		BrbStringPadLeft(sItoA, "0", 2);
		BrbStringReplace(pText, "yy", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "mil", 0) != 0)
	{
		brsitoa((DINT)pTime->millisec, (UDINT)&sItoA);
		if(pTime->millisec < 10)
		{
			BrbStringInsert(sItoA, 0, "00");
		}
		else if(pTime->millisec < 100)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "mil", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "mic", 0) != 0)
	{
		brsitoa((DINT)pTime->microsec, (UDINT)&sItoA);
		if(pTime->microsec < 10)
		{
			BrbStringInsert(sItoA, 0, "00");
		}
		else if(pTime->microsec < 100)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "mic", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "mm", 0) != 0)
	{
		brsitoa((DINT)pTime->month, (UDINT)&sItoA);
		if(pTime->month < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "mm", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "m", 0) != 0)
	{
		brsitoa((DINT)pTime->month, (UDINT)&sItoA);
		BrbStringReplace(pText, "m", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "dd", 0) != 0)
	{
		brsitoa((DINT)pTime->day, (UDINT)&sItoA);
		if(pTime->day < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "dd", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "d", 0) != 0)
	{
		brsitoa((DINT)pTime->day, (UDINT)&sItoA);
		BrbStringReplace(pText, "d", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "hh", 0) != 0)
	{
		brsitoa((DINT)pTime->hour, (UDINT)&sItoA);
		if(pTime->hour < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "hh", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "h", 0) != 0)
	{
		brsitoa((DINT)pTime->hour, (UDINT)&sItoA);
		BrbStringReplace(pText, "h", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "MM", 0) != 0)
	{
		brsitoa((DINT)pTime->minute, (UDINT)&sItoA);
		if(pTime->minute < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "MM", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "M", 0) != 0)
	{
		brsitoa((DINT)pTime->minute, (UDINT)&sItoA);
		BrbStringReplace(pText, "M", sItoA);
	}
	if(BrbStringGetAdrOf(pText, "ss", 0) != 0)
	{
		brsitoa((DINT)pTime->second, (UDINT)&sItoA);
		if(pTime->second < 10)
		{
			BrbStringInsert(sItoA, 0, "0");
		}
		BrbStringReplace(pText, "ss", sItoA);
	}
	else if(BrbStringGetAdrOf(pText, "s", 0) != 0)
	{
		brsitoa((DINT)pTime->second, (UDINT)&sItoA);
		BrbStringReplace(pText, "s", sItoA);
	}
	return eBRB_ERR_OK;
}
