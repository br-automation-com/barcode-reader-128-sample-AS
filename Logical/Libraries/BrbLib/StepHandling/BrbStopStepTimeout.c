/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStopStepTimeout.c
 * Author: niedermeierr
 * Created: June 11, 2013
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

/* Stoppt den Timeout-Zähler für eine Schrittkette */
unsigned short BrbStopStepTimeout(struct BrbStepHandling_TYP* pStepHandling)
{
	// Timer stoppen
	pStepHandling->Intern.fbTimeout.IN = 0;
	return 0;
}
