/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStepHandler.c
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
#include <string.h>

#ifdef __cplusplus
	};
#endif

/* Behandelt Schrittketten (Protokollierung, Timeout usw.) */
signed long BrbStepHandler(struct BrbStepHandling_TYP* pStepHandling)
{
	// Muss der Schritt protokolliert werden?
	if(pStepHandling->Intern.bLogOnNextCycle == 1)
	{
		pStepHandling->Intern.bLogOnNextCycle = 0;
		// Ist Protokollierung gestoppt?
		if(pStepHandling->Log.bStop == 0)
		{
			// Zykluszähler merken
			pStepHandling->Log.Steps[0].nCycleCount = pStepHandling->Intern.nCycleCount;
			// Einträge um eins nach unten schieben
			memmove(&pStepHandling->Log.Steps[1], &pStepHandling->Log.Steps[0], sizeof(BrbStepHandlingStep_TYP)*(nBRB_STEPLOG_STEPS_MAX));
			// Aktuellen Eintrag reinkopieren
			pStepHandling->Log.Steps[0].nStepNr = pStepHandling->Intern.nStepNrOld;
			strcpy(pStepHandling->Log.Steps[0].sStepText, pStepHandling->Current.sStepText);			
		}
		// Zykluszähler wieder auf 0 setzen
		pStepHandling->Intern.nCycleCount = 0;
	}
	// Hat sich der Schritt geändert?
	if(pStepHandling->Current.nStepNr != pStepHandling->Intern.nStepNrOld)
	{
		// Schritt beim nächsten Durchlauf protokollieren, weil dann erst der richtige Schritttext gesetzt ist
		pStepHandling->Intern.bLogOnNextCycle = 1;
	}
	// Zykluszähler erhöhen
	pStepHandling->Intern.nCycleCount++;
	// Aktuellen Schritt übernehmen
	pStepHandling->Intern.nStepNrOld = pStepHandling->Current.nStepNr;
	// Kommando zum Löschen der Protokollierung
	if(pStepHandling->Log.bClear == 1)
	{
		pStepHandling->Log.bClear = 0;
		// Den gesamten Aufzeichnungspuffer löschen
		memset(&pStepHandling->Log.Steps[0], 0, sizeof(pStepHandling->Log.Steps));
		pStepHandling->Intern.bLogOnNextCycle = 1;
	}
	// StepTimeout
	TON(&pStepHandling->Intern.fbTimeout);
	if(pStepHandling->Intern.fbTimeout.Q == 1)
	{
		// Timeout abgelaufen
		pStepHandling->Intern.fbTimeout.IN = 0;
		pStepHandling->Current.bTimeoutElapsed = 1;
	}	
	return eBRB_ERR_OK;
}
