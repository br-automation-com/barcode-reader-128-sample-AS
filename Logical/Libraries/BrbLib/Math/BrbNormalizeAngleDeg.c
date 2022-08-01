/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbNormalizeAngleDeg.c
 * Author: niedermeierr
 * Created: September 05, 2014
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

/* Normalisiert einen Grad-Winkel in den Bereich 0..360° */
float BrbNormalizeAngleDeg(float rAngleDeg, plcbit bKeep360)
{
	REAL rResult = 0.0;
	if(bKeep360 == 1 && rAngleDeg == 360.0)
	{
		// 360.0° ist eigentlich 0°. Diese Korrektur kann hier verhindert werden.
		rResult = 360.0;
	}
	else
	{
		DINT nMod = (DINT)(rAngleDeg / 360.0);
		rResult = rAngleDeg - (nMod * 360.0);
		if(rResult < 0.0)
		{
			rResult += 360.0;
		}
	}
	return rResult;
}
