
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	
	#include "BrbLib.h"
	#include <math.h>

#ifdef __cplusplus
	};
#endif

/* Gibt die Distanz zwischen zwei Punkten im 2-dimensionalen Raum zurück */
double BrbGetDistance2d(double rX1, double rY1, double rX2, double rY2)
{
	return sqrt(pow((rX1-rX2), 2) + pow((rY1-rY2), 2));
}
