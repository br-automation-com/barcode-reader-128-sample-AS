/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbUdintToHex.c
 * Author: niedermeierr
 * Created: July 14, 2021
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

/* Wandelt ein Usint-Array in einen Hex-String */
unsigned short BrbUsintArrayToHex(unsigned char* pArray, signed long nArrayLength, plcstring* pHex, unsigned long nHexSize, plcbit bDescending)
{
	UINT nStatus = eBRB_ERR_OK;
	if(pHex != 0)
	{
		memset(pHex, 0, nHexSize);
		UDINT nTextOffset = 0;
		if(nArrayLength > 0)
		{
			if(bDescending == 0)
			{
				// Aufsteigend
				UINT nIndex = 0;
				for(nIndex=0; nIndex<nArrayLength; nIndex++)
				{
					USINT nValue = *(pArray+nIndex);
					STRING sValueHexUsint[16];
					BrbUsintToHex(nValue, sValueHexUsint, sizeof(sValueHexUsint), 0);
					BrbStringAppend(pHex, sValueHexUsint, &nTextOffset);
				}
			}
			else
			{
				// Absteigend
				INT nIndex = 0;
				for(nIndex=nArrayLength-1; nIndex>-1; nIndex--)
				{
					USINT nValue = *(pArray+nIndex);
					STRING sValueHexUsint[16];
					BrbUsintToHex(nValue, sValueHexUsint, sizeof(sValueHexUsint), 0);
					BrbStringAppend(pHex, sValueHexUsint, &nTextOffset);
				}
			}
		}
		else
		{
			nStatus = eBRB_ERR_INVALID_PARAMETER;
		}
	}
	else
	{
		nStatus = eBRB_ERR_NULL_POINTER;
	}
	return nStatus;
}
