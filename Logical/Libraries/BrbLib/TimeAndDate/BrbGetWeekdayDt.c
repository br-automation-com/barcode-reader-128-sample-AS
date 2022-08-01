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

/* Gibt den Wochentag einer DT-Zeit zurück */
BrbWeekdays_ENUM BrbGetWeekdayDt(plcdt dtTime)
{
	DTStructure DtStruct;
	DT_TO_DTStructure(dtTime, (UDINT)&DtStruct);
	return (BrbWeekdays_ENUM)DtStruct.wday;
}
