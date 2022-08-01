/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetCurrentTimeText.c
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

#ifdef __cplusplus
	};
#endif

/* Gibt die aktuelle Zeit als Text zurück */
unsigned short BrbGetCurrentTimeText(plcstring* pText, unsigned long nTextSize, plcstring* pFormat)
{
	RTCtime_typ Time;
	RTC_gettime(&Time);
	return BrbGetTimeText(&Time, pText, nTextSize, pFormat);
}
