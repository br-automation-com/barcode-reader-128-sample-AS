
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

	#include "BrbLib.h"
	#include <AnsiCFunc.h>

#ifdef __cplusplus
	};
#endif

/* Gibt zurück, ob sich ein Wert innerhalb eines Bereichs befindet */
plcbit BrbIsWithinRangeLReal(double rValue, double rRangeMin, double rRangeMax, double rTolerance)
{
	BOOL bResult = 0;
	if(rRangeMin > rRangeMax)
	{
		LREAL nTemp = rRangeMin;
		rRangeMin = rRangeMax;
		rRangeMax = nTemp;
	}
	if(rTolerance < 0.0)
	{
		rTolerance = rTolerance*(LREAL)-1.00000000000;
	}
	LREAL rMin = rRangeMin - rTolerance;
	LREAL rMax = rRangeMax + rTolerance;
	if(rValue >= rMin && rValue <= rMax)
	{
		bResult = 1;
	}
	return bResult;
}
