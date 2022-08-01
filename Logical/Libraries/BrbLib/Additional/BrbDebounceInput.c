/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDebounceInput.c
 * Author: niedermeierr
 * Created: April 15, 2015
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

/* Entprellt einen Eingang */
void BrbDebounceInput(struct BrbDebounceInput* inst)
{
	inst->bOutput = 0;
	inst->fbDebounce.PT = inst->nDebounceTime;
	TON(&inst->fbDebounce);
	if(inst->fbDebounce.Q == 1)
	{
		inst->fbDebounce.IN = 0;
		TON(&inst->fbDebounce);
	}
	if(inst->bInput == 1 && inst->fbDebounce.IN == 0)
	{
		inst->bOutput = 1;
		inst->fbDebounce.IN = 1;
		TON(&inst->fbDebounce);
	}
}
