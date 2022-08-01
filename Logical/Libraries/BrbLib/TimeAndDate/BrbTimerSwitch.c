/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbTimerSwitch.c
 * Author: niedermeierr
 * Created: January 22, 2016
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

void SwitchOutputByType(struct BrbTimerSwitch* inst, BrbTimerSwitchType_ENUM eSwitchType);

/* Zeitschaltuhr für einen Kanal */
void BrbTimerSwitch(struct BrbTimerSwitch* inst)
{
	// Zeitschaltuhr
	if(inst->bEnable == 1)
	{
		if(inst->pUserTime == 0)
		{
			inst->fbDTStructureGetTime.enable = 1;
			inst->fbDTStructureGetTime.pDTStructure = (UDINT)&inst->dtsUsedTime;
			DTStructureGetTime(&inst->fbDTStructureGetTime);
		}
		else
		{
			memcpy(&inst->dtsUsedTime, inst->pUserTime, sizeof(DTStructure));
		}
		if(inst->bClearOutOnNextCycle == 1)
		{
			inst->bClearOutOnNextCycle = 0;
			inst->bOut = 0;
		}
		if(inst->bCmdSwitchOff == 1)
		{
			inst->bCmdSwitchOff = 0;
			inst->bOut = 0;
		}
		else if(inst->bCmdSwitchOn == 1)
		{
			inst->bCmdSwitchOn = 0;
			inst->bOut = 1;
		}
		else if(inst->bCmdToggle == 1)
		{
			inst->bCmdToggle = 0;
			inst->bOut = !inst->bOut;
		}
		UINT nTimePointIndex = 0;
		for(nTimePointIndex=0; nTimePointIndex<=nBRB_TIMERSWITCH_POINT_INDEX_MAX; nTimePointIndex++)
		{
			BrbTimerSwitchParTimePoint_TYP* pTimePoint = &inst->Parameter.TimePoint[nTimePointIndex];
			if(pTimePoint->bActive == 1)
			{
				switch(inst->Parameter.eMode)
				{
					case eBRB_TIMERSWITCH_MODE_MONTH:
						if(inst->dtsUsedTime.month == pTimePoint->dtsTimePoint.month && inst->dtsUsedTime.day == pTimePoint->dtsTimePoint.day && inst->dtsUsedTime.hour == pTimePoint->dtsTimePoint.hour && inst->dtsUsedTime.minute == pTimePoint->dtsTimePoint.minute && inst->dtsUsedTime.second == pTimePoint->dtsTimePoint.second)
						{
							if(inst->bLocked == 0)
							{
								SwitchOutputByType(inst, pTimePoint->eSwitchType);
								inst->bLocked = 1; // Nach diesem Schalten müssen andere Schaltungen für 1 Sekunde gesperrt werden
							}
						}
						break;

					case eBRB_TIMERSWITCH_MODE_WEEK:
						if(inst->dtsUsedTime.wday == pTimePoint->dtsTimePoint.wday && inst->dtsUsedTime.hour == pTimePoint->dtsTimePoint.hour && inst->dtsUsedTime.minute == pTimePoint->dtsTimePoint.minute && inst->dtsUsedTime.second == pTimePoint->dtsTimePoint.second)
						{
							if(inst->bLocked == 0)
							{
								SwitchOutputByType(inst, pTimePoint->eSwitchType);
								inst->bLocked = 1; // Nach diesem Schalten müssen andere Schaltungen für 1 Sekunde gesperrt werden
							}
						}
						break;

					case eBRB_TIMERSWITCH_MODE_DAY:
						if(inst->dtsUsedTime.day == pTimePoint->dtsTimePoint.day && inst->dtsUsedTime.hour == pTimePoint->dtsTimePoint.hour && inst->dtsUsedTime.minute == pTimePoint->dtsTimePoint.minute && inst->dtsUsedTime.second == pTimePoint->dtsTimePoint.second)
						{
							if(inst->bLocked == 0)
							{
								SwitchOutputByType(inst, pTimePoint->eSwitchType);
								inst->bLocked = 1; // Nach diesem Schalten müssen andere Schaltungen für 1 Sekunde gesperrt werden
							}
						}
						break;

					case eBRB_TIMERSWITCH_MODE_HOUR:
						if(inst->dtsUsedTime.hour == pTimePoint->dtsTimePoint.hour && inst->dtsUsedTime.minute == pTimePoint->dtsTimePoint.minute && inst->dtsUsedTime.second == pTimePoint->dtsTimePoint.second)
						{
							if(inst->bLocked == 0)
							{
								SwitchOutputByType(inst, pTimePoint->eSwitchType);
								inst->bLocked = 1; // Nach diesem Schalten müssen andere Schaltungen für 1 Sekunde gesperrt werden
							}
						}
						break;

					case eBRB_TIMERSWITCH_MODE_MINUTE:
						if(inst->dtsUsedTime.minute == pTimePoint->dtsTimePoint.minute && inst->dtsUsedTime.second == pTimePoint->dtsTimePoint.second)
						{
							if(inst->bLocked == 0)
							{
								SwitchOutputByType(inst, pTimePoint->eSwitchType);
								inst->bLocked = 1; // Nach diesem Schalten müssen andere Schaltungen für 1 Sekunde gesperrt werden
							}
						}
						break;

					case eBRB_TIMERSWITCH_MODE_SECOND:
						if(inst->dtsUsedTime.second == pTimePoint->dtsTimePoint.second)
						{
							if(inst->bLocked == 0)
							{
								SwitchOutputByType(inst, pTimePoint->eSwitchType);
								inst->bLocked = 1; // Nach diesem Schalten müssen andere Schaltungen für 1 Sekunde gesperrt werden
							}
						}
						break;
				}
			}
		}
		inst->fbLock.IN = inst->bLocked;
		inst->fbLock.PT = 1000; // = 1s
		TON(&inst->fbLock);
		if(inst->fbLock.Q == 1)
		{
			inst->fbLock.IN = 0;
			inst->bLocked = 0;
		}
	}
}

void SwitchOutputByType(struct BrbTimerSwitch* inst, BrbTimerSwitchType_ENUM eSwitchType)
{
	switch(eSwitchType)
	{
		case eBRB_TIMERSWITCH_TYPE_OFF:
			inst->bOut = 0;
			inst->nSwitchCount += 1;
			break;

		case eBRB_TIMERSWITCH_TYPE_ON:
			inst->bOut = 1;
			inst->nSwitchCount += 1;
			break;

		case eBRB_TIMERSWITCH_TYPE_TOGGLE:
			inst->bOut = !inst->bOut;
			inst->nSwitchCount += 1;
			break;

		case eBRB_TIMERSWITCH_TYPE_IMPULSE:
			inst->bOut = 1;
			inst->nSwitchCount += 1;
			inst->bClearOutOnNextCycle = 1;
			break;

	}
}
