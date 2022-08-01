/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringConvertRealFromExp.c
 * Author: niedermeierr
 * Created: December 03, 2013
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
#include <AnsiCFunc.h>

#ifdef __cplusplus
	};
#endif

/* Konvertiert eine exponentielle Notation in einen Real-Wert */
unsigned short BrbStringConvertRealFromExp(plcstring* pValue, plcstring* pResult, unsigned long nResultSize)
{
	memset(pResult, 0, nResultSize);
	UDINT nValueLen = strlen(pValue);
	STRING* pExpStart = BrbStringGetAdrOf(pValue, "e", nValueLen);
	if(pExpStart > pValue)
	{
		// "e" enthalten -> Zahl vor dem "e"
		STRING sReal[50];
		memset(sReal, 0, sizeof(sReal));
		BrbStringGetSubTextByAdr(pValue, pExpStart, sReal);
		// Vorzeichen, Anzahl der Vorkomma-Stellen, Stelle des Kommas und Anzahl der Nachkomma-Stellen
		BOOL bMinus = 0;
		// Vorzeichen l�schen
		BrbStringTrimLeft(sReal, "+");
		if(BrbStringStartsWith(sReal, "-") == 1)
		{
			bMinus = 1;
			BrbStringTrimLeft(sReal, "-");
		}
		// F�hrende Nullen l�schen
		BrbStringTrimLeft(sReal, "0");

		UDINT nRealLen = strlen(sReal);
		STRING* pRealDot = BrbStringGetAdrOf(sReal, ".", nValueLen);
		UDINT nIntegerDigits = 0;
		DINT nDotDigit = 0;		
		UDINT nFractionDigits = 0;
		if((UDINT)pRealDot >= (UDINT)&sReal)
		{
			// Punkt enthalten
			// Folgende Nullen nach dem Punkt l�schen
			BrbStringTrimRight(sReal, "0");
			if(BrbStringEndsWith(sReal, ".") == 1)
			{
				brsstrcat((UDINT)&sReal, (UDINT)&"0");
			}
			nRealLen = strlen(sReal);
			nIntegerDigits = (UDINT)pRealDot - (UDINT)sReal;
			nDotDigit = (UDINT)pRealDot - (UDINT)&sReal;
			nFractionDigits = (UDINT)&sReal + nRealLen - (UDINT)pRealDot - 1;
			// Punkt l�schen
			BrbStringReplace(sReal, ".", "");
		}
		else
		{
			// Punkt nicht enthalten
			nIntegerDigits = (UDINT)pExpStart - (UDINT)pValue;
			nDotDigit = -1;
			nFractionDigits = 0;
		}
		brsstrcpy((UDINT)pResult, (UDINT)&sReal);
		UDINT nExpLen = strlen(pExpStart);
		if(nExpLen > 1)
		{
			// Zahl nach dem "e"
			STRING sExp[50];
			memset(sExp, 0, sizeof(sExp));
			BrbStringGetSubTextByLen(pExpStart + 1, nExpLen - 1, sExp);
			DINT nExp = brsatoi((UDINT)&sExp);
			// Vorzeichen des Exponenten
			STRING* pExpMinus = BrbStringGetAdrOf(pExpStart, "-", nExpLen);
			if(pExpMinus > pExpStart && nExp != 0)
			{
				// Exponent ist negativ -> Nullen voranstellen
				DINT nAddZero = abs(nExp) - nIntegerDigits + 1;
				UINT nCount = 0;
				for(nCount=0; nCount<nAddZero; nCount++)
				{
					BrbStringInsert(pResult, 0, "0");
				}
				// Punkt einf�gen
				BrbStringInsert(pResult, 1, ".");
			}
			else
			{
				// Exponent ist positiv
				if((DINT)nFractionDigits <= nExp)
				{
					// Nullen anh�ngen
					DINT nAddZero = nExp - nFractionDigits;
					UINT nCount = 0;
					for(nCount=0; nCount<nAddZero; nCount++)
					{
						brsstrcat((UDINT)pResult, (UDINT)&"0");
					}
				}
				else
				{
					// Punkt einf�gen
					if(nDotDigit > -1)
					{
						UDINT nIndex = nDotDigit + nExp;
						BrbStringInsert(pResult, nIndex, ".");
					}
				}
			}
			// F�hrende Nullen l�schen
			BrbStringTrimLeft(pResult, "0");
			// Null vor dem Punkt wieder einf�gen
			if(BrbStringStartsWith(pResult, ".") == 1)
			{
				BrbStringInsert(pResult, 0, "0");
			}
			// Vorzeichen einf�gen
			if(bMinus == 1)
			{
				BrbStringInsert(pResult, 0, "-");
			}
		}
	}
	else
	{
		// "e" nicht enthalten
		brsstrcpy((UDINT)pResult, (UDINT)pValue);
		// Vorzeichen l�schen
		BrbStringTrimLeft(pResult, "+");
		BOOL bMinus = 0;
		if(BrbStringStartsWith(pResult, "-") == 1)
		{
			bMinus = 1;
			BrbStringTrimLeft(pResult, "-");
		}
		// F�hrende Nullen l�schen
		BrbStringTrimLeft(pResult, "0");
		// Null vor dem Punkt wieder einf�gen
		if(BrbStringStartsWith(pResult, ".") == 1)
		{
			BrbStringInsert(pResult, 0, "0");
		}
		// Folgende Nullen nach dem Punkt l�schen
		UDINT nLen = strlen(pResult);
		STRING* pDot = BrbStringGetAdrOf(pResult, ".", nLen);
		if(pDot > pResult)
		{
			BrbStringTrimRight(pResult, "0");
			if(BrbStringEndsWith(pResult, ".") == 1)
			{
				brsstrcat((UDINT)pResult, (UDINT)&"0");
			}
		}
		if(strlen(pResult) == 0)
		{
			brsstrcat((UDINT)pResult, (UDINT)&"0.0");
		}
		// Vorzeichen einf�gen
		if(bMinus == 1)
		{
			BrbStringInsert(pResult, 0, "-");
		}
	}
	return eBRB_ERR_OK;
}
