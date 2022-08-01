/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetRandomPercent.c
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
#include "AnsiCFunc.h"

#ifdef __cplusplus
	};
#endif

/* Erzeugt eine Zufallszahl zwischen 0.0 und 1.0 */
float BrbGetRandomPercent()
{
	INT	 	nRandom1 = rand();
	DINT	nRandom2 = (DINT)nRandom1 + 32768;
	REAL	rRandom3 = (REAL)nRandom2 / 65535.0;
	return rRandom3;
}
