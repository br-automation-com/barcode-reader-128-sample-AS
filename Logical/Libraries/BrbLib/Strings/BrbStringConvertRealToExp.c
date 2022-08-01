/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbStringConvertRealToExp.c
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

/* Konvertiert einen Real-Wert in eine exponentielle Notation */
unsigned short BrbStringConvertRealToExp(plcstring* pValue, plcstring* pResult, unsigned long nResultSize)
{
	memset(pResult, 0, nResultSize);
	UDINT nValueLen = strlen(pValue);
	STRING* pExpStart = BrbStringGetAdrOf(pValue, "e", nValueLen);
	if(pExpStart == 0)
	{
		// "e" nicht enthalten
		brsstrcpy((UDINT)pResult, (UDINT)pValue);
		// Vorzeichen löschen
		BrbStringTrimLeft(pResult, "+");
		BOOL bMinus = 0;
		if(BrbStringStartsWith(pResult, "-") == 1)
		{
			bMinus = 1;
			BrbStringTrimLeft(pResult, "-");
		}
		// Führende Nullen löschen
		BrbStringTrimLeft(pResult, "0");
		// Null vor dem Punkt wieder einfügen
		if(BrbStringStartsWith(pResult, ".") == 1)
		{
			BrbStringInsert(pResult, 0, "0");
		}
		else if(brsstrlen((UDINT)pResult) == 0)
		{
			brsstrcat((UDINT)pResult, (UDINT)&"0.0");
		}
		// Folgende Nullen nach dem Punkt löschen
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
		// Vorzeichen des Exponenten feststellen
		BOOL bExpMinus = 0;
		if(BrbStringStartsWith(pResult, "0") == 1)
		{
			// 0.0012345
			// Erstes Zeichen ist "0" -> Exponent negativ
			bExpMinus = 1;
		}
		else
		{
			// 123.45
			// Erstes Zeichen ist keine "0" -> Exponent positiv
			bExpMinus = 0;
		}
		// Exponent feststellen
		DINT nExp = 0;
		if(bExpMinus == 1)
		{
			// Negativer Exponent
			UDINT nLen = strlen(pResult);
			STRING* pDot = BrbStringGetAdrOf(pResult, ".", nLen);
			if(pDot > pResult)
			{
				// Punkt enthalten
				nExp = 1;
				USINT* pChar = 0;
				USINT* pEnd =(USINT*)pResult + nLen - 1;
				// Anzahl der Nullen vom Punkt bis zur ersten relevanten Ziffer
				for(pChar=(USINT*)pDot+1; pChar<=pEnd; pChar++)
				{
					if(*pChar == 48)
					{
						nExp += 1;
					}
					else
					{
						break;
					}
				}
				DINT nDotDigit = (UDINT)pDot - (UDINT)pResult;
				UDINT nCutLen = nDotDigit + nExp;
				// Alle Zeichen bis zur ersten relevanten Ziffer löschen
				BrbStringCut(pResult, 0, nCutLen, 0);
			}
			else
			{
				// Punkt nicht enthalten
				nExp = 0;
			}
		}
		else
		{
			// Positiver Exponent
			UDINT nLen = strlen(pResult);
			STRING* pDot = BrbStringGetAdrOf(pResult, ".", nLen);
			if(pDot > pResult)
			{
				// Punkt enthalten
				DINT nDotDigit = (UDINT)pDot - (UDINT)pResult;
				nExp = nDotDigit - 1;
				// Punkt löschen
				BrbStringReplace(pResult, ".", "");
			}
			else
			{
				// Punkt nicht enthalten
				UDINT nLen = strlen(pResult);
				nExp = nLen - 1;
			}
		}
		if(brsstrlen((UDINT)pResult) == 0)
		{
			brsstrcat((UDINT)pResult, (UDINT)&"0.0e+000");
		}
		else
		{
			// Punkt an 2. Stelle einfügen (1 Vorkomma-Stelle)
			BrbStringInsert(pResult, 1, ".");
			// Folgende Nullen löschen
			BrbStringTrimRight(pResult, "0");
			// "e" anhängen
			brsstrcat((UDINT)pResult, (UDINT)&"e");
			// Exponent-Vorzeichen
			if(bExpMinus == 0)
			{
				brsstrcat((UDINT)pResult, (UDINT)&"+");
			}
			else
			{
				brsstrcat((UDINT)pResult, (UDINT)&"-");
			}
			// Exponent mit mindestens 3 Stellen anhängen
			STRING sExp[50];
			brsitoa(nExp, (UDINT)&sExp);
			BrbStringPadLeft(sExp, "0", 3);
			brsstrcat((UDINT)pResult, (UDINT)&sExp);
			// Vorzeichen einfügen
			if(bMinus == 1)
			{
				BrbStringInsert(pResult, 0, "-");
			}
		}
	}
	else
	{
		// "e" enthalten
		STRING sTemp[nResultSize];
		BrbStringConvertRealFromExp(pValue, sTemp, sizeof(sTemp));
		BrbStringConvertRealToExp(sTemp, pResult, nResultSize);
	}
	return eBRB_ERR_OK;
}
