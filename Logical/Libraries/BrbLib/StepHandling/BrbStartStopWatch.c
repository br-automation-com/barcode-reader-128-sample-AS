/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStartStopWatch.c
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

/* Startet eine Stoppuhr */
plcbit BrbStartStopWatch(struct BrbStopWatch_TYP* pStopWatch)
{
	// Stoppuhr starten
	pStopWatch->tStartTime = clock_ms();
	pStopWatch->nTimeDiff = 0;
	memset(pStopWatch->sTimeDiff, 0, sizeof(pStopWatch->sTimeDiff));
	return 0;
}
