
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif

#include "BrbLib.h"

#ifdef __cplusplus
	};
#endif

/* Gibt den absoluten Wert zurück */
double BrbAbsLReal(double rValue)
{
	LREAL rResult = rValue;
	if(rValue < 0.0)
	{
		rResult = rValue * -1.0;
	}
	return rResult;
}
