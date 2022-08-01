/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetAngleRad.c
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

/* Rechnet einen Winkel von Grad ins Bogenmass um */
float BrbGetAngleRad(float rAngleDeg)
{
	REAL nResult = rAngleDeg * rBRB_FACTOR_DEG_TO_RAD;
	return nResult;
}
