
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
plcbit BrbIsWithinRangeReal(float rValue, float rRangeMin, float rRangeMax, float rTolerance)
{
	BOOL bResult = 0;
	if(rRangeMin > rRangeMax)
	{
		REAL nTemp = rRangeMin;
		rRangeMin = rRangeMax;
		rRangeMax = nTemp;
	}
	if(rTolerance < 0.0)
	{
		rTolerance = rTolerance*-1.00000000000;
	}
	REAL rMin = rRangeMin - rTolerance;
	REAL rMax = rRangeMax + rTolerance;
	if(rValue >= rMin && rValue <= rMax)
	{
		bResult = 1;
	}
	return bResult;
}
