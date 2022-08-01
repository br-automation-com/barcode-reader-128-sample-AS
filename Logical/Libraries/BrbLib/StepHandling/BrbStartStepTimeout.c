/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStartStepTimeout.c
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

/* Startet den Timeout-Z�hler f�r eine Schrittkette */
unsigned short BrbStartStepTimeout(struct BrbStepHandling_TYP* pStepHandling, unsigned long nTimeout, signed long nContinueStep)
{
	// Pr�fen, ob Timer schon abgelaufen ist
	if(pStepHandling->Intern.fbTimeout.Q == 0)
	{
		// Timer setzen und starten
		pStepHandling->Intern.fbTimeout.PT = nTimeout;
		pStepHandling->Current.nTimeoutContinueStep = nContinueStep;
		pStepHandling->Intern.fbTimeout.IN = 1;
	}
	return 0;
}
