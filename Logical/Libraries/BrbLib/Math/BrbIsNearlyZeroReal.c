
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

/* Gibt zurück, ob ein REAL-Wert annähernd 0 ist */
plcbit BrbIsNearlyZeroReal(float rValue, float rTolerance)
{
	BOOL bResult = 0;
	if(rTolerance < 0.0)
	{
		rTolerance = rTolerance*-1.00000000000;
	}
	REAL rMin = -rTolerance;
	REAL rMax = rTolerance;
	if(rValue >= rMin && rValue <= rMax)
	{
		bResult = 1;
	}
	return bResult;
}
