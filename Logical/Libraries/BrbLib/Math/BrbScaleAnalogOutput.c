/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbScaleAnalogOutput.c
 * Author: niedermeierr
 * Created: April 22, 2016
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

/* Skaliert ein analoges Ausgangssignal von einem Einheitswert */
signed short BrbScaleAnalogOutput(float rScaled, signed short nRawMin, signed short nRawMax, float rScaledMin, float rScaledMax, float rFactor, float rOffset)
{
	INT nResult = 0;
	REAL rScaleDiff = (rScaledMax - rScaledMin);
	if(rFactor != 0.0 && rScaleDiff != 0.0) // Division durch 0 vermeiden
	{
		REAL rRawMin = (REAL)nRawMin;
		REAL rRawMax = (REAL)nRawMax;
		REAL rValue = ((((rScaled - rOffset) / rFactor) - rScaledMin) * (rRawMax - rRawMin) / rScaleDiff) + rRawMin;
		if(rValue < rRawMin)
		{
			nResult = nRawMin;
		}
		else if(rValue > rRawMax)
		{
			nResult = nRawMax;
		}
		else
		{
			nResult = (INT)rValue;
		}
	}
	return nResult;
}
