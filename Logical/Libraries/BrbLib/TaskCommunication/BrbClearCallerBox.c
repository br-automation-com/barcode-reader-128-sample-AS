/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbClearCallerBox.c
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

/* Setzt eine CallerBox komplett auf 0 */
unsigned short BrbClearCallerBox(unsigned long pCallerBox, unsigned long nSize)
{
	memset((USINT*)pCallerBox, 0, nSize);
	return eBRB_ERR_OK;
}
