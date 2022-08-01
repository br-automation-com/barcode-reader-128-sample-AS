/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbGetDtFromTimeText.c
 * Author: niedermeierr
 * Created: June 25, 2013
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

/* Wandelt einen Zeit-Text in eine Zeit um */
plcdt BrbGetDtFromTimeText(plcstring* pTimeText, plcstring* pFormat)
{
	if(pTimeText == 0)
	{
		return 0;
	}
	if(pFormat == 0)
	{
		return 0;
	}
	DATE_AND_TIME dtResult = 0;
	UDINT nTimeTextLen = strlen(pTimeText);
	UDINT nFormatLen = strlen(pFormat);
	if(nTimeTextLen >= nFormatLen)
	{
		UDINT nAdr = 0;
		UDINT nOffset = 0;
		STRING sValue[20];
		DTStructure DtStruct;
		memset((USINT*)&DtStruct, 0, sizeof(DtStruct));
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "yyyy", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 4, sValue);
			DtStruct.year = (UINT)brsatoi((UDINT)&sValue);		
		}
		else
		{
			nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "yy", nFormatLen);
			if(nAdr >= (UDINT)pFormat)
			{
				nOffset = nAdr - (UDINT)pFormat;
				BrbStringGetSubText(pTimeText, nOffset, 2, sValue);
				DtStruct.year = (UINT)brsatoi((UDINT)&sValue) + 2000;		
			}
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "mm", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 2, sValue);
			DtStruct.month = (USINT)brsatoi((UDINT)&sValue);		
		}
		else
		{
			nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "m", nFormatLen);
			if(nAdr >= (UDINT)pFormat)
			{
				nOffset = nAdr - (UDINT)pFormat;
				BrbStringGetSubText(pTimeText, nOffset, 1, sValue);
				DtStruct.month = (USINT)brsatoi((UDINT)&sValue);		
			}
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "dd", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 2, sValue);
			DtStruct.day = (USINT)brsatoi((UDINT)&sValue);		
		}
		else
		{
			nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "d", nFormatLen);
			if(nAdr >= (UDINT)pFormat)
			{
				nOffset = nAdr - (UDINT)pFormat;
				BrbStringGetSubText(pTimeText, nOffset, 1, sValue);
				DtStruct.day = (USINT)brsatoi((UDINT)&sValue);		
			}
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "hh", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 2, sValue);
			DtStruct.hour = (USINT)brsatoi((UDINT)&sValue);		
		}
		else
		{
			nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "h", nFormatLen);
			if(nAdr >= (UDINT)pFormat)
			{
				nOffset = nAdr - (UDINT)pFormat;
				BrbStringGetSubText(pTimeText, nOffset, 1, sValue);
				DtStruct.hour = (USINT)brsatoi((UDINT)&sValue);		
			}
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "MM", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 2, sValue);
			DtStruct.minute = (USINT)brsatoi((UDINT)&sValue);		
		}
		else
		{
			nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "M", nFormatLen);
			if(nAdr >= (UDINT)pFormat)
			{
				nOffset = nAdr - (UDINT)pFormat;
				BrbStringGetSubText(pTimeText, nOffset, 1, sValue);
				DtStruct.minute = (USINT)brsatoi((UDINT)&sValue);		
			}
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "ss", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 2, sValue);
			DtStruct.second = (USINT)brsatoi((UDINT)&sValue);		
		}
		else
		{
			nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "s", nFormatLen);
			if(nAdr >= (UDINT)pFormat)
			{
				nOffset = nAdr - (UDINT)pFormat;
				BrbStringGetSubText(pTimeText, nOffset, 1, sValue);
				DtStruct.second = (USINT)brsatoi((UDINT)&sValue);		
			}
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "mil", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 3, sValue);
			DtStruct.millisec = (UINT)brsatoi((UDINT)&sValue);		
		}
		nAdr = (UDINT)BrbStringGetAdrOf(pFormat, "mic", nFormatLen);
		if(nAdr >= (UDINT)pFormat)
		{
			nOffset = nAdr - (UDINT)pFormat;
			BrbStringGetSubText(pTimeText, nOffset, 3, sValue);
			DtStruct.microsec = (UINT)brsatoi((UDINT)&sValue);		
		}
		dtResult = DTStructure_TO_DT((UDINT)&DtStruct);
	}
	return dtResult;
}
