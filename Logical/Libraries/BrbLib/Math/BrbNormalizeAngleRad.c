/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbNormalizeAngleRad.c
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
#include <AnsiCFunc.h>

#ifdef __cplusplus
	};
#endif

/* Normalisiert einen Bogenmass-Winkel in den Bereich 0..2Pi */
float BrbNormalizeAngleRad(float rAngleRad)
{
	REAL rResult = 0.0;
	DINT nMod = (DINT)(rAngleRad / 6.283185307179586476925286766559);
	rResult = rAngleRad - (nMod * 6.283185307179586476925286766559);
	if(BrbIsNearlyZeroReal(rResult, 0.00017453292519943))
	{
		// ~= 0.0
		rResult = 0.0;
	}
	else if(rResult < 0.0)
	{
		rResult += 6.283185307179586476925286766559;
	}
	return rResult;
}
