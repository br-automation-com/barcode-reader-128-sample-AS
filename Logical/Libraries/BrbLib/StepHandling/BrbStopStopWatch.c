/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStopStopWatch.c
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

/* Stoppt eine Stoppuhr */
unsigned long BrbStopStopWatch(struct BrbStopWatch_TYP* pStopWatch)
{
	// Stoppuhr stoppen
	pStopWatch->tStopTime = clock_ms();
	pStopWatch->nTimeDiff = DiffT(pStopWatch->tStopTime, pStopWatch->tStartTime);
	memset(pStopWatch->sTimeDiff, 0, sizeof(pStopWatch->sTimeDiff));
	TIMEStructure TimeDiff;
	TIME_TO_TIMEStructure(pStopWatch->nTimeDiff, (UDINT)&TimeDiff);
	STRING sItoA[20];
	memset(sItoA, 0, sizeof(sItoA));
	if(TimeDiff.day < 10)
	{
		strcat((STRING*)pStopWatch->sTimeDiff, "0");
	}
	brsitoa((DINT)TimeDiff.day, (UDINT)&sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, "d_");
	if(TimeDiff.hour < 10)
	{
		strcat((STRING*)pStopWatch->sTimeDiff, "0");
	}
	brsitoa((DINT)TimeDiff.hour, (UDINT)&sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, "h_");
	if(TimeDiff.minute < 10)
	{
		strcat((STRING*)pStopWatch->sTimeDiff, "0");
	}
	brsitoa((DINT)TimeDiff.minute, (UDINT)&sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, "m_");
	if(TimeDiff.second < 10)
	{
		strcat((STRING*)pStopWatch->sTimeDiff, "0");
	}
	brsitoa((DINT)TimeDiff.second, (UDINT)&sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, "s_");
	if(TimeDiff.millisec < 10)
	{
		strcat((STRING*)pStopWatch->sTimeDiff, "00");
	}
	else if(TimeDiff.millisec < 100)
	{
		strcat((STRING*)pStopWatch->sTimeDiff, "0");
	}
	brsitoa((DINT)TimeDiff.millisec, (UDINT)&sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, sItoA);
	strcat((STRING*)pStopWatch->sTimeDiff, "ms");
	return pStopWatch->nTimeDiff;
}
