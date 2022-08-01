
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
plcbit BrbIsWithinRangeDint(signed long nValue, signed long nRangeMin, signed long nRangeMax, signed long nTolerance)
{
	BOOL bResult = 0;
	if(nRangeMin > nRangeMax)
	{
		DINT nTemp = nRangeMin;
		nRangeMin = nRangeMax;
		nRangeMax = nTemp;
	}
	if(nTolerance < 0)
	{
		nTolerance = abs(nTolerance);
	}
	DINT nMin = nRangeMin - nTolerance;
	DINT nMax = nRangeMax + nTolerance;
	if(nValue >= nMin && nValue <= nMax)
	{
		bResult = 1;
	}
	return bResult;
}
