/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbRtcTimeToDtStruct.c
 * Author: niedermeierr
 * Created: June 25, 2013
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

/* Wandelt eine RTC-Zeit in eine DtStruct-Zeit */
unsigned short BrbRtcTimeToDtStruct(struct RTCtime_typ* pRtcTime, struct DTStructure* pDtStruct)
{
	// Prüfung der Parameter
	if(pRtcTime == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	if(pDtStruct == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	// Wandlung
	pDtStruct->year = pRtcTime->year;
	pDtStruct->month = pRtcTime->month;
	pDtStruct->day = pRtcTime->day;
	pDtStruct->wday = 0;
	pDtStruct->hour = pRtcTime->hour;
	pDtStruct->minute = pRtcTime->minute;
	pDtStruct->second = pRtcTime->second;
	pDtStruct->millisec = pRtcTime->millisec;
	pDtStruct->microsec = pRtcTime->microsec;
	return eBRB_ERR_OK;
}
