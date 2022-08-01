/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDtStructAddMillisecondsLReal.c
 * Author: niedermeierr
 * Created: September 10, 2021
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

/* Addiert zu einer DTStructure die angegebenen Millisekunden */
plcdt BrbDtStructAddMillisecondsLReal(struct DTStructure* pDtStruct, double rMilliseconds)
{
	// Prüfung der Parameter
	if(pDtStruct == 0)
	{
		return 0;
	}
	// Addieren
	LREAL rDays = rMilliseconds / (LREAL)86400000.0; // [ms/day]
	DINT nDays = (DINT)rDays; // Nachkomma-Stellen werden abgeschnitten
	BrbDtStructAddDays(pDtStruct, nDays);
	rMilliseconds -= (LREAL)nDays * (LREAL)86400000.0;
	DINT nMilliseconds = (DINT)rMilliseconds;
	BrbDtStructAddMilliseconds(pDtStruct, nMilliseconds);
	DATE_AND_TIME dtTime = DTStructure_TO_DT((UDINT)pDtStruct);
	return dtTime;
}
