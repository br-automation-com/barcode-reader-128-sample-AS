/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetCompilerVersion.c
 * Author: niedermeierr
 * Created: October 24, 2019
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

/* Gibt die Version des verwendeten GCC-Compilers zurück */
unsigned short BrbGetCompilerVersion(struct BrbCompilerVersion_TYP* pCompilerVersion)
{
	UINT nResult = 0;
	STRING sItoA[20];
	pCompilerVersion->bValid = 0;
	pCompilerVersion->nMajor = 0;
	pCompilerVersion->nMinor = 0;
	pCompilerVersion->nPatch = 0;
	pCompilerVersion->nTotal = 0;
	memset(&pCompilerVersion->sText, 0, sizeof(pCompilerVersion->sText));

	#ifdef __GNUC__
		pCompilerVersion->nMajor = __GNUC__;
		nResult += 4;
	#endif
	pCompilerVersion->nTotal += ((DINT)pCompilerVersion->nMajor * 10000);
	brsitoa((DINT)pCompilerVersion->nMajor, (UDINT)&sItoA);
	strcpy((STRING*)&pCompilerVersion->sText, sItoA);
	strcat((STRING*)&pCompilerVersion->sText, ".");
	
	#ifdef __GNUC_MINOR__
		pCompilerVersion->nMinor = __GNUC_MINOR__;
		nResult += 2;
	#endif
	pCompilerVersion->nTotal += ((DINT)pCompilerVersion->nMinor * 100);
	brsitoa((DINT)pCompilerVersion->nMinor, (UDINT)&sItoA);
	strcat((STRING*)&pCompilerVersion->sText, sItoA);
	strcat((STRING*)&pCompilerVersion->sText, ".");

	#ifdef __GNUC_PATCHLEVEL__
		pCompilerVersion->nPatch = __GNUC_PATCHLEVEL__;
		nResult += 1;
	#endif
	pCompilerVersion->nTotal += (DINT)pCompilerVersion->nPatch;
	brsitoa((DINT)pCompilerVersion->nPatch, (UDINT)&sItoA);
	strcat((STRING*)&pCompilerVersion->sText, sItoA);
	
	if(nResult >= 7)
	{
		pCompilerVersion->bValid = 1;
	}
	return nResult;
}
