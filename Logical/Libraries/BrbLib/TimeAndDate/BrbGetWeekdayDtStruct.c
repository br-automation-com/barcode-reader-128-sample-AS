/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetWeekdayDt.c
 * Author: niedermeierr
 * Created: May 17, 2021
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

/* Ermittelt den Wochentag einer DTStructure */
BrbWeekdays_ENUM BrbGetWeekdayDtStruct(struct DTStructure* pDtStruct)
{
	DINT nMilliSecondsBefore = (DINT)pDtStruct->millisec;
	UINT nMicroSecondsBefore = pDtStruct->microsec;
	DATE_AND_TIME dtTime = DTStructure_TO_DT((UDINT)pDtStruct);
	DT_TO_DTStructure(dtTime, (UDINT)pDtStruct);
	pDtStruct->millisec = nMilliSecondsBefore;
	pDtStruct->microsec = nMicroSecondsBefore;
	return (BrbWeekdays_ENUM)pDtStruct->wday;
}
