/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbCombinePath.c
 * Author: niedermeierr
 * Created: November 02, 2014
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

/* Fügt zwei Pfade oder einen Pfad und eine Datei-Namen zusammen */
unsigned short BrbCombinePath(plcstring* pPath, plcstring* pFilename, plcstring* pFilenameWithPath)
{
	// Prüfung der Parameter
	if(pPath == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	if(pFilename == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	if(pFilenameWithPath == 0)
	{
		return eBRB_ERR_NULL_POINTER;
	}
	// Check
	STRING sFileWithPath[nBRB_FILE_NAME_CHAR_MAX];
	memset(sFileWithPath, 0, sizeof(sFileWithPath));
	if(strlen(pPath) > 0)
	{
		strcat(sFileWithPath, pPath);
		if(BrbStringEndsWith(pPath, "\\") == 0)
		{
			strcat(sFileWithPath, "\\");
		}
	}
	strcat(sFileWithPath, pFilename);
	strcpy(pFilenameWithPath, sFileWithPath);	
	return 0;
}
