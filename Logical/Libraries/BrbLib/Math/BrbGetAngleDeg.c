/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetAngleDeg.c
 * Author: niedermeierr
 * Created: July 19, 2014
 ********************************************************************
 * Implementation of library BrbLib
 ********************************************************************/

#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"
#include <math.h>

#ifdef __cplusplus
	};
#endif

/* Rechnet einen Winkel vom Bogenmass in Grad um */
float BrbGetAngleDeg(float rAngleRad)
{
	REAL nResult = rAngleRad * rBRB_FACTOR_RAD_TO_DEG;
	return nResult;
}
