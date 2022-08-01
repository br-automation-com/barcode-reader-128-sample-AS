/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetDtStruct.c
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
#include "AsTime.h"

#ifdef __cplusplus
	};
#endif

/* Setzt eine DTStructure-Zeit */
plcdt BrbSetDtStruct(struct DTStructure* pDtStruct, unsigned short nYear, unsigned char nMonth, unsigned char nDay, unsigned char nHour, unsigned char nMinute, unsigned char nSecond, unsigned short nMillisecond, unsigned short nMicrosecond)
{
	pDtStruct->year = nYear;
	pDtStruct->month = nMonth;
	pDtStruct->day = nDay;
	pDtStruct->hour = nHour;
	pDtStruct->minute = nMinute;
	pDtStruct->second = nSecond;
	pDtStruct->millisec = nMillisecond;
	pDtStruct->microsec =  nMicrosecond;
	BrbGetWeekdayDtStruct(pDtStruct);
	return DTStructure_TO_DT((UDINT)pDtStruct);
}
