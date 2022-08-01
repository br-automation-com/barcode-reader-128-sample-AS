/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetRandomUdint.c
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

/* Erzeugt eine Zufallszahl zwischen nMin und nMax */
unsigned long BrbGetRandomUdint(unsigned long nMin, unsigned long nMax)
{
	UDINT nRandom = 0;
	if(nMin == 0 && nMax == 4294967295UL)
	{
		nRandom = (UDINT)(BrbGetRandomPercent()*nMax);
	}
	else
	{
		// Eigentlich ohne +1, aber dann ist es sehr unwahrscheinlich, dass sich der Max-Wert ergibt (RandomPercent muss 1.0 sein).
		// Das +1 erhöht die Chance dazu, muss dann aber korrigiert werden.
		nRandom = (UDINT)(BrbGetRandomPercent()*(nMax-nMin+1))+nMin;
		if(nRandom > nMax)
		{
			nRandom = nMax;
		}
		else if(nMin > 0 && nMax == 4294967295UL && nRandom == 0)
		{
			nRandom = 4294967295UL;
		}
	}
	return nRandom;
}
