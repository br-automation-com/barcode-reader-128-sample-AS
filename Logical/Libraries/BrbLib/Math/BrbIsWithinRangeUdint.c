
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
plcbit BrbIsWithinRangeUdint(unsigned long nValue, unsigned long nRangeMin, unsigned long nRangeMax, unsigned long nTolerance)
{
	BOOL bResult = 0;
	if(nRangeMin > nRangeMax)
	{
		UDINT nTemp = nRangeMin;
		nRangeMin = nRangeMax;
		nRangeMax = nTemp;
	}
	UDINT nMin = 0;
	if(nTolerance < nRangeMin)
	{
		nMin = nRangeMin - nTolerance;
	}
	UDINT nMax = nRangeMax + nTolerance;
	if(nValue >= nMin && nValue <= nMax)
	{
		bResult = 1;
	}
	return bResult;
}
