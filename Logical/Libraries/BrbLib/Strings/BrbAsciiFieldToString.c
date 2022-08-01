/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbAsciiFieldToString.c
 * Author: niedermeierr
 * Created: April 04, 2014
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

void AddCharText(USINT nAsciiChar, STRING* pText);

/* Wandelt ein Ascii-Feld in einen lesbaren Text */
plcbit BrbAsciiFieldToString(unsigned char* pAsciiField, unsigned long nAsciiFieldLen, unsigned long nFinalAsciiCharCount, plcstring* pText, unsigned long nTextSize)
{
	BOOL bFuncRetVal = 0;
	if(pAsciiField != 0)
	{
		if(pText != 0)
		{
			// Textlänge für FinalAscii ermitteln
			if(nFinalAsciiCharCount > nAsciiFieldLen)
			{
				nFinalAsciiCharCount = nAsciiFieldLen;
			}
			UDINT nFinalAsciiTextLen = 0;
			if(nFinalAsciiCharCount > 0)
			{
				// Letzte Zeichenanzahl solange vermindern, bis übersetzter FinalText in pText passt
				BOOL bTextSizeToSmall = 0;
				DINT nIndex = 0;
				for(nIndex=nFinalAsciiCharCount; nIndex>0; nIndex--)
				{
					USINT* pFinalAsciiBegin = pAsciiField + nAsciiFieldLen - nFinalAsciiCharCount;
					bTextSizeToSmall = BrbAsciiFieldToString(pFinalAsciiBegin, nFinalAsciiCharCount, 0, pText, nTextSize);
					nFinalAsciiTextLen = strlen(pText);
					if(bTextSizeToSmall == 0)
					{
						// Übersetzter FinalText passt in pText
						break;
					}
					nFinalAsciiCharCount--;
				}
				if(bTextSizeToSmall == 1)
				{
					// Auch das letzte Zeichen passt übersetzt nicht in pText
					nFinalAsciiCharCount = 0;
					nFinalAsciiTextLen = 0;
				}
			}
			memset((STRING*)pText, 0, nTextSize);
			UDINT nAsciiFieldIndex = 0;
			USINT* pAsciiChar = 0;
			DINT nTextIndex = 0;
			STRING sCharDescription[30];
			STRING sCharDescriptionStart[5];
			strcpy(sCharDescriptionStart, "<");
			STRING sCharDescriptionEnd[5];
			strcpy(sCharDescriptionEnd, ">");
			UDINT nCharDescriptionLen = 0;
			// Platz vorsehen für die letzten Ascii-Zeichen und Platzhalter
			DINT nPlaceHolderLen = 7; // "<+9999>"
			DINT nTextLimit = nTextSize - nFinalAsciiTextLen - nPlaceHolderLen - 1;
			BOOL bTextOverrun = 0;
			BOOL bHandleFinalAscii = 0;
			BOOL bLeave = 0;
			do
			{
				pAsciiChar = ((USINT*)pAsciiField) + nAsciiFieldIndex;
				if(*pAsciiChar < 32)
				{
					// Sonderzeichen -> Anzahl dieses Sonderzeichens zählen
					USINT* pAsciiCharPreview = 0;
					DINT nSameAsciiCharCount = 1;
					for(pAsciiCharPreview=pAsciiChar+1; pAsciiCharPreview<=pAsciiField+nAsciiFieldLen-1; pAsciiCharPreview++)
					{
						if(*pAsciiCharPreview == *pAsciiChar)
						{
							nSameAsciiCharCount++;
						}
						else
						{
							break;
						}
					}
					if(nSameAsciiCharCount == 1)
					{
						// Zeichen kommt nur einmal vor
						memset(sCharDescription, 0, sizeof(sCharDescription));
						strcat(sCharDescription, sCharDescriptionStart);
						AddCharText(*pAsciiChar, sCharDescription);
						strcat(sCharDescription, sCharDescriptionEnd);
						nCharDescriptionLen = strlen(sCharDescription);
						if(nTextIndex < nTextLimit-(DINT)nCharDescriptionLen)
						{
							strcat((STRING*)pText, sCharDescription);
							nTextIndex += nCharDescriptionLen;
							nAsciiFieldIndex += 1;
						}
						else
						{
							bTextOverrun = 1;
							bFuncRetVal = 1;
						}
					}
					else
					{
						// Zeichen kommt öfters vor
						memset(sCharDescription, 0, sizeof(sCharDescription));
						strcat(sCharDescription, sCharDescriptionStart);
						STRING sItoa[10];
						memset(sItoa, 0, sizeof(sItoa));
						brsitoa((DINT)nSameAsciiCharCount, (UDINT)&sItoa);
						strcat(sCharDescription, sItoa);
						strcat(sCharDescription, "*");
						AddCharText(*pAsciiChar, sCharDescription);
						strcat(sCharDescription, sCharDescriptionEnd);
						nCharDescriptionLen = strlen(sCharDescription);
						if(nTextIndex < nTextLimit-(DINT)nCharDescriptionLen)
						{
							strcat((STRING*)pText, sCharDescription);
							nTextIndex += nCharDescriptionLen;
							nAsciiFieldIndex += nSameAsciiCharCount;
						}
						else
						{
							bTextOverrun = 1;
							bFuncRetVal = 1;
						}
					}
				}
				else
				{
					// Normalzeichen -> übernehmen
					if(nTextIndex < nTextLimit-1)
					{
						USINT* pTextChar = ((USINT*)pText) + nTextIndex;
						*pTextChar = *pAsciiChar;
						nTextIndex++;
						nAsciiFieldIndex += 1;
					}
					else
					{
						bTextOverrun = 1;
						bFuncRetVal = 1;
					}
				}
				if(bTextOverrun == 1 || nAsciiFieldIndex >= (nAsciiFieldLen-nFinalAsciiCharCount))
				{
					if(bHandleFinalAscii == 0 && nFinalAsciiCharCount > 0)
					{
						// Die letzten Zeichen müssen enthalten sein (Platz im Text dafür wurde vorgesehen)
						DINT nPlaceholderCharCount = nAsciiFieldLen - nAsciiFieldIndex - nFinalAsciiCharCount;
						if(nPlaceholderCharCount > 0)
						{
							STRING sPlaceholder[10];
							memset(sPlaceholder, 0, sizeof(sPlaceholder));
							strcat(sPlaceholder, "<+");
							STRING sItoa[10];
							memset(sItoa, 0, sizeof(sItoa));
							brsitoa((DINT)nPlaceholderCharCount, (UDINT)&sItoa);
							strcat(sPlaceholder, sItoa);
							strcat(sPlaceholder, ">");
							strcat((STRING*)pText, sPlaceholder);
							nTextIndex += strlen(sPlaceholder);
						}
						nTextLimit = nTextSize - 1;
						bTextOverrun = 0;
						if(nAsciiFieldIndex < nAsciiFieldLen - nFinalAsciiCharCount)
						{
							nAsciiFieldIndex = nAsciiFieldLen - nFinalAsciiCharCount;
						}
						bHandleFinalAscii = 1;
						bLeave = 0;
					}
					else if(nFinalAsciiCharCount == 0)
					{
						// Es gibt keine Finalzeichen -> Anzahl der austehenden Zeichen schreiben und beenden
						DINT nPlaceholderCharCount = nAsciiFieldLen - nAsciiFieldIndex;
						if(nPlaceholderCharCount > 0)
						{
							STRING sPlaceholder[10];
							memset(sPlaceholder, 0, sizeof(sPlaceholder));
							strcat(sPlaceholder, "<+");
							STRING sItoa[10];
							memset(sItoa, 0, sizeof(sItoa));
							brsitoa((DINT)nPlaceholderCharCount, (UDINT)&sItoa);
							strcat(sPlaceholder, sItoa);
							strcat(sPlaceholder, ">");
							strcat((STRING*)pText, sPlaceholder);
							nTextIndex += strlen(sPlaceholder);
						}
						bLeave =1;
					}
				}
				if(nAsciiFieldIndex >= nAsciiFieldLen)
				{
					// Ende des Ascii-Feldes erreicht
					bLeave = 1;
				}
			} while(bLeave == 0);
		}
	}
	return bFuncRetVal;
}

void AddCharText(USINT nAsciiChar, STRING* pText)
{
	if(nAsciiChar == 0)
	{
		strcat(pText, "0=NUL");
	}
	else if(nAsciiChar == 1)
	{
		strcat(pText, "1=SOH");
	}
	else if(nAsciiChar == 2)
	{
		strcat(pText, "2=STX");
	}
	else if(nAsciiChar == 3)
	{
		strcat(pText, "3=ETX");
	}
	else if(nAsciiChar == 4)
	{
		strcat(pText, "4=EOT");
	}
	else if(nAsciiChar == 5)
	{
		strcat(pText, "5=ENQ");
	}
	else if(nAsciiChar == 6)
	{
		strcat(pText, "6=ACK");
	}
	else if(nAsciiChar == 7)
	{
		strcat(pText, "7=BEL");
	}
	else if(nAsciiChar == 8)
	{
		strcat(pText, "8=BS");
	}
	else if(nAsciiChar == 9)
	{
		strcat(pText, "9=HT");
	}
	else if(nAsciiChar == 10)
	{
		strcat(pText, "10=LF");
	}
	else if(nAsciiChar == 11)
	{
		strcat(pText, "11=VT");
	}
	else if(nAsciiChar == 12)
	{
		strcat(pText, "12=FF");
	}
	else if(nAsciiChar == 13)
	{
		strcat(pText, "13=CR");
	}
	else if(nAsciiChar == 14)
	{
		strcat(pText, "14=SO");
	}
	else if(nAsciiChar == 15)
	{
		strcat(pText, "15=SI");
	}
	else if(nAsciiChar == 16)
	{
		strcat(pText, "16=DL");
	}
	else if(nAsciiChar == 17)
	{
		strcat(pText, "17=DC");
	}
	else if(nAsciiChar == 18)
	{
		strcat(pText, "18=DC2");
	}
	else if(nAsciiChar == 19)
	{
		strcat(pText, "19=DC3");
	}
	else if(nAsciiChar == 20)
	{
		strcat(pText, "20=DC4");
	}
	else if(nAsciiChar == 21)
	{
		strcat(pText, "21=NAK");
	}
	else if(nAsciiChar == 22)
	{
		strcat(pText, "22=SYN");
	}
	else if(nAsciiChar == 23)
	{
		strcat(pText, "23=ETB");
	}
	else if(nAsciiChar == 24)
	{
		strcat(pText, "24=CAN");
	}
	else if(nAsciiChar == 25)
	{
		strcat(pText, "25=EM");
	}
	else if(nAsciiChar == 26)
	{
		strcat(pText, "26=SUB");
	}
	else if(nAsciiChar == 27)
	{
		strcat(pText, "27=ESC");
	}
	else if(nAsciiChar == 28)
	{
		strcat(pText, "28=FS");
	}
	else if(nAsciiChar == 29)
	{
		strcat(pText, "29=GS");
	}
	else if(nAsciiChar == 30)
	{
		strcat(pText, "30=RS");
	}
	else if(nAsciiChar == 31)
	{
		strcat(pText, "31=US");
	}
	else
	{
		STRING sItoa[10];
		memset(sItoa, 0, sizeof(sItoa));
		brsitoa((DINT)nAsciiChar, (UDINT)&sItoa);
		strcat(pText, sItoa);
	}
}
