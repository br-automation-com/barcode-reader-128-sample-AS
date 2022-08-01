/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbSetCaller.c
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

/* Versucht, die Reservierung für ein Task-Kommando zu erhalten */
BrbCallerStates_ENUM BrbSetCaller(struct BrbCaller_TYP* pCaller, signed long nCallerId)
{
	// Prüfung der Parameter
	if(pCaller == 0)
	{
		return eBRB_CALLER_STATE_NOT_READY;
	}
	// Prüfen, ob die CallerId gesetzt werden kann
	if((pCaller->nCallerId == 0) && (pCaller->bLock == 0))
	{
		// CallerId setzen
		pCaller->nCallerId = nCallerId;
		// Prüfen, ob der Aufruf von einer schnelleren Taskklasse unterbrochen wurde
		if((pCaller->nCallerId == nCallerId) && (pCaller->bLock == 0))
		{
			// CallerId konnte gesetzt werden
			pCaller->bLock = 1;
			return eBRB_CALLER_STATE_OK;
		}
		else
		{
			// Das Setzen wurde von einer schnelleren Taskklasse unterbrochen
			return eBRB_CALLER_STATE_BUSY;
		}
	}
	else if(pCaller->nCallerId == -1)
	{
		// Der Task ist noch nicht bereit
		return eBRB_CALLER_STATE_NOT_READY;
	}
	else
	{
		// Der Task ist schon reserviert
		return eBRB_CALLER_STATE_BUSY;
	}
}
