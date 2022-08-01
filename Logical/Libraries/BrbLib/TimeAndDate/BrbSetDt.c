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

/* Setzt eine DATE_AND_TIME-Zeit */
plcdt BrbSetDt(unsigned short nYear, unsigned char nMonth, unsigned char nDay, unsigned char nHour, unsigned char nMinute, unsigned char nSecond)
{
	DTStructure dtStruct;
	DATE_AND_TIME dtTime = BrbSetDtStruct(&dtStruct, nYear, nMonth, nDay, nHour, nMinute, nSecond, 0, 0);
	return dtTime;
}
