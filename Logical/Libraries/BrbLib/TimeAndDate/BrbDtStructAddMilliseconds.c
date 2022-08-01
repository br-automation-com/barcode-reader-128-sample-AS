/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDtStructAddMilliseconds.c
 * Author: niedermeierr
 * Created: December 11, 2013
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
plcdt BrbDtStructAddMilliseconds(struct DTStructure* pDtStruct, signed long nMilliseconds)
{
	// Prüfung der Parameter
	if(pDtStruct == 0)
	{
		return 0;
	}
	// Addieren
	DINT nMilliSecondsBefore = (DINT)pDtStruct->millisec;
	UINT nMicroSecondsBefore = pDtStruct->microsec;
	DATE_AND_TIME dtTime = DTStructure_TO_DT((UDINT)pDtStruct);
	DINT nSeconds = (DINT)(nMilliseconds / 1000);
	dtTime += nSeconds;
	DINT nMillisecondsLeft = nMilliseconds - (DINT)(nSeconds * 1000);
	if(nMilliSecondsBefore + nMillisecondsLeft >= 1000)
	{
		dtTime += 1;
		nMillisecondsLeft -= 1000;
	}
	else if(nMilliSecondsBefore + nMillisecondsLeft < 0)
	{
		dtTime -= 1;
		nMillisecondsLeft += 1000;
	}
	DT_TO_DTStructure(dtTime, (UDINT)pDtStruct);
	pDtStruct->millisec = nMilliSecondsBefore + nMillisecondsLeft;
	pDtStruct->microsec = nMicroSecondsBefore;
	return dtTime;
}
