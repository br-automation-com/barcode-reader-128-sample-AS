/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDtStructAddSeconds.c
 * Author: niedermeierr
 * Created: September 08, 2021
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

/* Addiert zu einer DTStructure die angegebenen Sekunden */
plcdt BrbDtStructAddDays(struct DTStructure* pDtStruct, signed long nDays)
{
	// Prüfung der Parameter
	if(pDtStruct == 0)
	{
		return 0;
	}
	// Addieren
	UINT nMilliSecondsBefore = pDtStruct->millisec;
	UINT nMicroSecondsBefore = pDtStruct->microsec;
	DATE_AND_TIME dtTime = DTStructure_TO_DT((UDINT)pDtStruct);
	// Es müssen Sekunden addiert werden. Durch eine Multiplikation der Stunden mit 60s*60m*24h = 86400s könnte der Bereich eines Dint nicht mehr ausreichen
	// Daher wird eine Schleife durchlaufen.
	DINT nIncrement = 86400; // Sekunden pro Tag
	if(nDays < 0)
	{
		nIncrement = -86400;
	}
	UDINT nLoop = 0;
	for(nLoop=0; nLoop<(UDINT)abs(nDays); nLoop++)
	{
		dtTime += nIncrement;
	}
	DT_TO_DTStructure(dtTime, (UDINT)pDtStruct);
	pDtStruct->millisec = nMilliSecondsBefore;
	pDtStruct->microsec = nMicroSecondsBefore;
	return dtTime;
}
