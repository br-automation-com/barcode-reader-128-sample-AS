/********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbDtStructCompare.c
 * Author: niedermeierr
 * Created: December 11, 2013
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

/* Vergleicht zwei DtStructures */
plcbit BrbDtStructCompare(struct DTStructure* pDtStruct1, enum BrbTimeAndDateCompare_ENUM eCompare, struct DTStructure* pDtStruct2)
{
	// Prüfung der Parameter
	if(pDtStruct1 == 0)
	{
		return 0;
	}
	if(pDtStruct2 == 0)
	{
		return 0;
	}
	// Vergleich
	BOOL bResult = 0;
	switch(eCompare)
	{
		case eBRB_TAD_COMPARE_YOUNGER:
			if(pDtStruct1->year < pDtStruct2->year)
			{
				bResult = 1;
			}
			else if(pDtStruct1->year == pDtStruct2->year)
			{
				if(pDtStruct1->month < pDtStruct2->month)
				{
					bResult = 1;
				}
				else if(pDtStruct1->month == pDtStruct2->month)
				{
					if(pDtStruct1->day < pDtStruct2->day)
					{
						bResult = 1;
					}
					else if(pDtStruct1->day == pDtStruct2->day)
					{
						if(pDtStruct1->hour < pDtStruct2->hour)
						{
							bResult = 1;
						}
						else if(pDtStruct1->hour == pDtStruct2->hour)
						{
							if(pDtStruct1->minute < pDtStruct2->minute)
							{
								bResult = 1;
							}
							else if(pDtStruct1->minute == pDtStruct2->minute)
							{
								if(pDtStruct1->second < pDtStruct2->second)
								{
									bResult = 1;
								}
								else if(pDtStruct1->second == pDtStruct2->second)
								{
									if(pDtStruct1->millisec < pDtStruct2->millisec)
									{
										bResult = 1;
									}
									else if(pDtStruct1->millisec == pDtStruct2->millisec)
									{
										if(pDtStruct1->microsec < pDtStruct2->microsec)
										{
											bResult = 1;
										}
										else if(pDtStruct1->microsec == pDtStruct2->microsec)
										{
											bResult = 0;
										}
									}
								}
							}
						}
					}
				}
			}
			break;

		case eBRB_TAD_COMPARE_YOUNGEREQUAL:
			if(pDtStruct1->year < pDtStruct2->year)
			{
				bResult = 1;
			}
			else if(pDtStruct1->year == pDtStruct2->year)
			{
				if(pDtStruct1->month < pDtStruct2->month)
				{
					bResult = 1;
				}
				else if(pDtStruct1->month == pDtStruct2->month)
				{
					if(pDtStruct1->day < pDtStruct2->day)
					{
						bResult = 1;
					}
					else if(pDtStruct1->day == pDtStruct2->day)
					{
						if(pDtStruct1->hour < pDtStruct2->hour)
						{
							bResult = 1;
						}
						else if(pDtStruct1->hour == pDtStruct2->hour)
						{
							if(pDtStruct1->minute < pDtStruct2->minute)
							{
								bResult = 1;
							}
							else if(pDtStruct1->minute == pDtStruct2->minute)
							{
								if(pDtStruct1->second < pDtStruct2->second)
								{
									bResult = 1;
								}
								else if(pDtStruct1->second == pDtStruct2->second)
								{
									if(pDtStruct1->millisec < pDtStruct2->millisec)
									{
										bResult = 1;
									}
									else if(pDtStruct1->millisec == pDtStruct2->millisec)
									{
										if(pDtStruct1->microsec < pDtStruct2->microsec)
										{
											bResult = 1;
										}
										else if(pDtStruct1->microsec == pDtStruct2->microsec)
										{
											bResult = 1;
										}
									}
								}
							}
						}
					}
				}
			}
			break;

		case eBRB_TAD_COMPARE_EQUAL:
			if((pDtStruct1->year == pDtStruct2->year) &&
				(pDtStruct1->month == pDtStruct2->month) && 
				(pDtStruct1->day == pDtStruct2->day) && 
				(pDtStruct1->hour == pDtStruct2->hour) && 
				(pDtStruct1->minute == pDtStruct2->minute) &&
				(pDtStruct1->second == pDtStruct2->second) &&
				(pDtStruct1->millisec == pDtStruct2->millisec) &&
				(pDtStruct1->microsec == pDtStruct2->microsec)
				)
			{
				bResult = 1;
			}
			break;

		case eBRB_TAD_COMPARE_OLDEREQUAL:
			if(pDtStruct1->year > pDtStruct2->year)
			{
				bResult = 1;
			}
			else if(pDtStruct1->year == pDtStruct2->year)
			{
				if(pDtStruct1->month > pDtStruct2->month)
				{
					bResult = 1;
				}
				else if(pDtStruct1->month == pDtStruct2->month)
				{
					if(pDtStruct1->day > pDtStruct2->day)
					{
						bResult = 1;
					}
					else if(pDtStruct1->day == pDtStruct2->day)
					{
						if(pDtStruct1->hour > pDtStruct2->hour)
						{
							bResult = 1;
						}
						else if(pDtStruct1->hour == pDtStruct2->hour)
						{
							if(pDtStruct1->minute > pDtStruct2->minute)
							{
								bResult = 1;
							}
							else if(pDtStruct1->minute == pDtStruct2->minute)
							{
								if(pDtStruct1->second > pDtStruct2->second)
								{
									bResult = 1;
								}
								else if(pDtStruct1->second == pDtStruct2->second)
								{
									if(pDtStruct1->millisec > pDtStruct2->millisec)
									{
										bResult = 1;
									}
									else if(pDtStruct1->millisec == pDtStruct2->millisec)
									{
										if(pDtStruct1->microsec > pDtStruct2->microsec)
										{
											bResult = 1;
										}
										else if(pDtStruct1->microsec == pDtStruct2->microsec)
										{
											bResult = 1;
										}
									}
								}
							}
						}
					}
				}
			}
			break;

		case eBRB_TAD_COMPARE_OLDER:
			if(pDtStruct1->year > pDtStruct2->year)
			{
				bResult = 1;
			}
			else if(pDtStruct1->year == pDtStruct2->year)
			{
				if(pDtStruct1->month > pDtStruct2->month)
				{
					bResult = 1;
				}
				else if(pDtStruct1->month == pDtStruct2->month)
				{
					if(pDtStruct1->day > pDtStruct2->day)
					{
						bResult = 1;
					}
					else if(pDtStruct1->day == pDtStruct2->day)
					{
						if(pDtStruct1->hour > pDtStruct2->hour)
						{
							bResult = 1;
						}
						else if(pDtStruct1->hour == pDtStruct2->hour)
						{
							if(pDtStruct1->minute > pDtStruct2->minute)
							{
								bResult = 1;
							}
							else if(pDtStruct1->minute == pDtStruct2->minute)
							{
								if(pDtStruct1->second > pDtStruct2->second)
								{
									bResult = 1;
								}
								else if(pDtStruct1->second == pDtStruct2->second)
								{
									if(pDtStruct1->millisec > pDtStruct2->millisec)
									{
										bResult = 1;
									}
									else if(pDtStruct1->millisec == pDtStruct2->millisec)
									{
										if(pDtStruct1->microsec > pDtStruct2->microsec)
										{
											bResult = 1;
										}
										else if(pDtStruct1->microsec == pDtStruct2->microsec)
										{
											bResult = 0;
										}
									}
								}
							}
						}
					}
				}
			}
			break;

	}
		
	return bResult;
}
