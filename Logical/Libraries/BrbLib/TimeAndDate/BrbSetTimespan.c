/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetTimespan.c
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

#ifdef __cplusplus
	};
#endif

/* Setzt eine TIME-Zeitspanne */
plctime BrbSetTimespan(signed char nSign, unsigned char nDays, unsigned short nHours, unsigned short nMinutes, unsigned long nSeconds, unsigned long nMilliseconds)
{
	TIME tTime = 2147483647; // Bei Überschreitung positiven Höchstwert zurückgeben
	UDINT nTime = 0;
	nTime += nMilliseconds;
	nTime += nSeconds * 1000;
	nTime += nMinutes * 60 * 1000;
	nTime += nHours * 60 * 60 * 1000;
	nTime += nDays * 24 * 60 * 60 * 1000;
	if(nSign >= 0)
	{
		if(nTime <= 2147483647)
		{
			tTime = nTime;
		}
	}
	else
	{
		if(nTime <= 2147483648UL)
		{
			LREAL rTime = (LREAL)nTime * -1.0;
			tTime = (DINT)rTime;
		}
		else
		{
			tTime = (DINT)(-2147483647-1); // Bei Unterschreitung negativen Höchstwert zurückgeben
		}
	}
	return tTime;
}
