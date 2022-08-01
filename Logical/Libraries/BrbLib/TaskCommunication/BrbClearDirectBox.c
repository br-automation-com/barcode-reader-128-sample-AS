/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbClearDirectBox.c
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

/* Setzt eine DirectBox komplett auf 0 */
unsigned short BrbClearDirectBox(unsigned long pDirectBox, unsigned long nSize)
{
	memset((USINT*)pDirectBox, 0, nSize);
	return eBRB_ERR_OK;
}
