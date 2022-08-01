/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbCheckIpAddress.c
 * Author: niedermeierr
 * Created: November 25, 2014
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

/* Prüft eine Ip-Adresse und berichtigt sie gegebenenfalls */
plcbit BrbCheckIpAddress(plcstring* pIpAddress)
{
	// Eine Ip-Adresse darf keine Buchstaben enthalten.
	// Die einzelnen Segmente dürfen keine führenden Nullen haben, weil sie sonst vom Betriebssystem als Oktal-Zahl erkannt werden.
	BOOL bResult = 0;
	STRING sAdr[4][nBRB_IP_ADR_CHAR_MAX];
	BrbStringSplitEmpty(pIpAddress, ".", (UDINT)&sAdr, 3, sizeof(sAdr[0]));
	UINT nIndex = 0;
	for(nIndex=0; nIndex<=3; nIndex++)
	{
		UDINT nLen = strlen(sAdr[nIndex]);
		if(nLen == 0)
		{
			// Leerstring wird zu "0"
			strcpy(sAdr[nIndex], "0");
		}
		else
		{
			// Nummern-Prüfung
			DINT nNumber = brsatoi((UDINT)&sAdr[nIndex]);
			brsitoa(nNumber, (UDINT)&sAdr[nIndex]);
		}
	}
	strcpy(pIpAddress, "");
	strcat(pIpAddress, sAdr[0]);
	strcat(pIpAddress, ".");
	strcat(pIpAddress, sAdr[1]);
	strcat(pIpAddress, ".");
	strcat(pIpAddress, sAdr[2]);
	strcat(pIpAddress, ".");
	strcat(pIpAddress, sAdr[3]);
	return bResult;
}
