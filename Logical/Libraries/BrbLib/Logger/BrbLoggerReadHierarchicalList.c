
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "BrbLib.h"
#ifdef __cplusplus
	};
#endif

#include <string.h>

// Eigene Enums und Typen
enum Steps_ENUM
{
	eSTEP_INIT,
	eSTEP_GET_LOGBOOK,
	eSTEP_CHECK_STARTRECORDID,
	eSTEP_GET_LATEST_RECORDID,
	eSTEP_GET_PREVIOUS_RECORDID,
	eSTEP_READ_RECORD,
	eSTEP_READ_ERRORNUMBER,
	eSTEP_READ_OBJECTID,
	eSTEP_READ_DESCRIPTION,
	eSTEP_INSERT_LIST_ENTRY,
};

/* Liest die zu einem definierten Logger-Eintrag gehörende Hierarchie-Liste von Einträgen aus */
void BrbLoggerReadHierarchicalList(struct BrbLoggerReadHierarchicalList* inst)
{
	switch(inst->eStep)
	{
		case eSTEP_INIT:
			// Fub initialisieren
			inst->nStatus = 65535;
			memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
			inst->LogListManagement.pList = inst->pLogList;
			inst->LogListManagement.nEntryLength = sizeof(BrbLogHierarchListEntry_TYP);
			inst->LogListManagement.nIndexMax = inst->nLogListIndexMax;
			BrbMemListClear(&inst->LogListManagement);
			inst->nSearchEntryCount = 0;
			inst->bFirstEntryIsListed = 0;
			inst->nListEntryCount = 0;
			// Prüfen, ob LogbookIdent ermittelt werden muss
			if(inst->nEventLogIdent != 0)
			{
				if(strlen(inst->sLogbookName) > 0 && strcmp(inst->sLogbookName, inst->sLastLogbookName) == 0)
				{
					// Gleiches Logbuch wie beim letzten Aufruf
					inst->eStep = eSTEP_CHECK_STARTRECORDID;
				}
				else
				{
					// Logbuch-Name hat sich geändert
					inst->eStep = eSTEP_GET_LOGBOOK;
				}
			}
			else
			{
				// Logbuch wurde noch nicht ermittelt
				inst->eStep = eSTEP_GET_LOGBOOK;
			}
			break;

		case eSTEP_GET_LOGBOOK:
			// Ident des Logbuchs ermitteln
			inst->fbArEventLogGetIdent.Execute = 1;
			strcpy(inst->fbArEventLogGetIdent.Name, inst->sLogbookName);
			ArEventLogGetIdent(&inst->fbArEventLogGetIdent);
			if(inst->fbArEventLogGetIdent.Done == 1)
			{
				strcpy(inst->sLastLogbookName, inst->sLogbookName);
				inst->nEventLogIdent = inst->fbArEventLogGetIdent.Ident;
				inst->fbArEventLogGetIdent.Execute = 0;
				ArEventLogGetIdent(&inst->fbArEventLogGetIdent);
				inst->eStep = eSTEP_CHECK_STARTRECORDID;
			}
			else if(inst->fbArEventLogGetIdent.Error == 1)
			{
				inst->nEventLogIdent = 0;
				inst->nStatus = inst->fbArEventLogGetIdent.StatusID;
				inst->fbArEventLogGetIdent.Execute = 0;
				ArEventLogGetIdent(&inst->fbArEventLogGetIdent);
				inst->eStep = eSTEP_INIT;
			}
			break;
		
		case eSTEP_CHECK_STARTRECORDID:
			// Prüfen, ob eine StartRecordId angegeben ist
			if(inst->nStartRecordId != 0)
			{
				inst->ActEntry.nRecordId = inst->nStartRecordId;
				inst->eStep = eSTEP_READ_OBJECTID;
			}
			else
			{
				// Bei jüngstem Eintrag beginnen
				inst->eStep = eSTEP_GET_LATEST_RECORDID;
			}
			break;
		
		case eSTEP_GET_LATEST_RECORDID:
			// RecordId des jüngsten Eintrag ermitteln
			inst->fbArEventLogGetLatestRecordID.Execute = 1;
			inst->fbArEventLogGetLatestRecordID.Ident = inst->nEventLogIdent;
			ArEventLogGetLatestRecordID(&inst->fbArEventLogGetLatestRecordID);
			if(inst->fbArEventLogGetLatestRecordID.Done == 1)
			{
				inst->ActEntry.nRecordId = inst->fbArEventLogGetLatestRecordID.RecordID;
				inst->fbArEventLogGetLatestRecordID.Execute = 0;
				ArEventLogGetLatestRecordID(&inst->fbArEventLogGetLatestRecordID);
				inst->eStep = eSTEP_READ_OBJECTID;
			}
			else if(inst->fbArEventLogGetLatestRecordID.Error == 1)
			{
				memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
				inst->nStatus = inst->fbArEventLogGetLatestRecordID.StatusID;
				inst->fbArEventLogGetLatestRecordID.Execute = 0;
				ArEventLogGetLatestRecordID(&inst->fbArEventLogGetLatestRecordID);
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_GET_PREVIOUS_RECORDID:
			// RecordId des chronologisch vorherigen Eintrags ermitteln
			inst->fbArEventLogGetPreviousRecordID.Execute = 1;
			inst->fbArEventLogGetPreviousRecordID.Ident = inst->nEventLogIdent;
			inst->fbArEventLogGetPreviousRecordID.RecordID = inst->ActEntry.nRecordId;
			ArEventLogGetPreviousRecordID(&inst->fbArEventLogGetPreviousRecordID);
			if(inst->fbArEventLogGetPreviousRecordID.Done == 1)
			{
				inst->ActEntry.nRecordId = inst->fbArEventLogGetPreviousRecordID.PrevRecordID;
				inst->fbArEventLogGetPreviousRecordID.Execute = 0;
				ArEventLogGetPreviousRecordID(&inst->fbArEventLogGetPreviousRecordID);
				inst->eStep = eSTEP_READ_OBJECTID;
			}
			else if(inst->fbArEventLogGetPreviousRecordID.Error == 1)
			{
				memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
				if(inst->fbArEventLogGetPreviousRecordID.StatusID == arEVENTLOG_ERR_RECORDID_INVALID)
				{
					// Am Ende des Logbuchs angekommen
					inst->nStatus = 0;
				}
				else
				{
					inst->nStatus = inst->fbArEventLogGetPreviousRecordID.StatusID;
				}
				inst->fbArEventLogGetPreviousRecordID.Execute = 0;
				ArEventLogGetPreviousRecordID(&inst->fbArEventLogGetPreviousRecordID);
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_READ_OBJECTID:
			// Limit prüfen
			if(inst->bFirstEntryIsListed == 0 && (inst->nSearchEntryCountLimit > 0 && inst->nSearchEntryCount > inst->nSearchEntryCountLimit))
			{
				// Limit erreicht -> Ende
				inst->nStatus = 0;
				inst->eStep = eSTEP_INIT;
			}
			else
			{
				// ObjectId ('Entered by') lesen
				inst->fbArEventLogReadObjectID.Execute = 1;
				inst->fbArEventLogReadObjectID.Ident = inst->nEventLogIdent;
				inst->fbArEventLogReadObjectID.RecordID = inst->ActEntry.nRecordId;
				ArEventLogReadObjectID(&inst->fbArEventLogReadObjectID);
				if(inst->fbArEventLogReadObjectID.Done == 1)
				{
					inst->nSearchEntryCount += 1;
					strncpy(inst->ActEntry.sObjectId, inst->fbArEventLogReadObjectID.ObjectID, sizeof(inst->ActEntry.sObjectId));
					inst->fbArEventLogReadObjectID.Execute = 0;
					ArEventLogReadObjectID(&inst->fbArEventLogReadObjectID);
					// ObjectId filtern
					if(strlen(inst->sObjectId) == 0)
					{
						// Kein Filter
						inst->eStep = eSTEP_READ_RECORD;
					}
					else if(strcmp(inst->sObjectId, inst->ActEntry.sObjectId) == 0)
					{
						inst->eStep = eSTEP_READ_RECORD;
					}
					else
					{
						if(inst->bFirstEntryIsListed == 0)
						{
							// Nächsten Eintrag lesen
							inst->ActEntry.nOriginRecordId = 0;
							inst->ActEntry.nEventId = 0;
							inst->ActEntry.nSeverity = 0;
							memset(inst->ActEntry.sObjectId, 0, sizeof(inst->ActEntry.sObjectId));
							inst->eStep = eSTEP_GET_PREVIOUS_RECORDID;
						}
						else
						{
							// Alle passenden Einträge schon gelistet -> Ende
							inst->nStatus = 0;
							inst->eStep = eSTEP_INIT;
						}
					}
				}
				else if(inst->fbArEventLogReadObjectID.Error == 1)
				{
					memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
					inst->nStatus = inst->fbArEventLogReadObjectID.StatusID;
					inst->fbArEventLogReadObjectID.Execute = 0;
					ArEventLogReadObjectID(&inst->fbArEventLogReadObjectID);
					inst->eStep = eSTEP_INIT;
				}
			}
			break;

		case eSTEP_READ_RECORD:
			// Logbuch-Eintrag lesen
			inst->fbArEventLogRead.Execute = 1;
			inst->fbArEventLogRead.Ident = inst->nEventLogIdent;
			inst->fbArEventLogRead.RecordID = inst->ActEntry.nRecordId;
			ArEventLogRead(&inst->fbArEventLogRead);
			if(inst->fbArEventLogRead.Done == 1)
			{
				inst->ActEntry.nOriginRecordId = inst->fbArEventLogRead.OriginRecordID;
				inst->ActEntry.nEventId = inst->fbArEventLogRead.EventID;
				UDINT nSeverityMasked = inst->ActEntry.nEventId & 3221225472UL; // =0xC0000000 = Bit30..31
				inst->ActEntry.nSeverity = (USINT)(nSeverityMasked>>30);
				inst->fbArEventLogRead.Execute = 0;
				ArEventLogRead(&inst->fbArEventLogRead);
				if(inst->ActEntry.nEventId == 0)
				{
					// Eintrag im ErrorNumber-Format
					inst->ActEntry.nEventId = 0;
					inst->ActEntry.nSeverity = 0;
					inst->eStep = eSTEP_READ_ERRORNUMBER;
				}
				else
				{
					// Eintrag im EventID-Format 
					// EventId filtern
					if(inst->nStartEventId == 0 || inst->ActEntry.nEventId == inst->nStartEventId || inst->bFirstEntryIsListed == 1)
					{
						// Severity filtern
						BOOL bInFilter = 0;
						if(inst->eSeverities == eBRB_LOG_SEVERITY_FILTER_ALL)
						{
							bInFilter = 1;
						}
						else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_SUCCESS) == eBRB_LOG_SEVERITY_FILTER_SUCCESS) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_SUCCESS)
						{
							bInFilter = 1;
						}
						else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_INFO) == eBRB_LOG_SEVERITY_FILTER_INFO) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_INFO)
						{
							bInFilter = 1;
						}
						else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_WARNING) == eBRB_LOG_SEVERITY_FILTER_WARNING) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_WARNING)
						{
							bInFilter = 1;
						}
						else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_ERROR) == eBRB_LOG_SEVERITY_FILTER_ERROR) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_ERROR)
						{
							bInFilter = 1;
						}
						if(bInFilter == 1)
						{
							inst->eStep = eSTEP_READ_DESCRIPTION;
						}
						else
						{
							if(inst->bFirstEntryIsListed == 0)
							{
								// Nächsten Eintrag lesen
								inst->ActEntry.nOriginRecordId = 0;
								inst->ActEntry.nEventId = 0;
								inst->ActEntry.nSeverity = 0;
								inst->eStep = eSTEP_GET_PREVIOUS_RECORDID;
							}
							else
							{
								// Alle passenden Einträge schon gelistet -> Ende
								inst->nStatus = 0;
								inst->eStep = eSTEP_INIT;
							}
						}
					}
					else
					{
						// Nächsten Eintrag lesen
						inst->ActEntry.nOriginRecordId = 0;
						inst->ActEntry.nEventId = 0;
						inst->ActEntry.nSeverity = 0;
						inst->eStep = eSTEP_GET_PREVIOUS_RECORDID;
					}
				}
			}
			else if(inst->fbArEventLogRead.Error == 1)
			{
				memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
				inst->nStatus = inst->fbArEventLogRead.StatusID;
				inst->fbArEventLogRead.Execute = 0;
				ArEventLogRead(&inst->fbArEventLogRead);
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_READ_ERRORNUMBER:
			// Fehlernummer und Severity eines alten Eintrags lesen
			inst->fbArEventLogReadErrorNumber.Execute = 1;
			inst->fbArEventLogReadErrorNumber.Ident = inst->nEventLogIdent;
			inst->fbArEventLogReadErrorNumber.RecordID = inst->ActEntry.nRecordId;
			ArEventLogReadErrorNumber(&inst->fbArEventLogReadErrorNumber);
			if(inst->fbArEventLogReadErrorNumber.Done == 1)
			{
				inst->ActEntry.nEventId = inst->fbArEventLogReadErrorNumber.ErrorNumber;
				inst->ActEntry.nSeverity = inst->fbArEventLogReadErrorNumber.Severity;
				inst->fbArEventLogReadErrorNumber.Execute = 0;
				ArEventLogReadErrorNumber(&inst->fbArEventLogReadErrorNumber);
				// ErrorNumber filtern
				if(inst->nStartEventId == 0 || inst->ActEntry.nEventId == inst->nStartEventId || inst->bFirstEntryIsListed == 1)
				{
					// Severity filtern
					BOOL bInFilter = 0;
					if(inst->eSeverities == eBRB_LOG_SEVERITY_FILTER_ALL)
					{
						bInFilter = 1;
					}
					else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_SUCCESS) == eBRB_LOG_SEVERITY_FILTER_SUCCESS) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_SUCCESS)
					{
						bInFilter = 1;
					}
					else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_INFO) == eBRB_LOG_SEVERITY_FILTER_INFO) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_INFO)
					{
						bInFilter = 1;
					}
					else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_WARNING) == eBRB_LOG_SEVERITY_FILTER_WARNING) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_WARNING)
					{
						bInFilter = 1;
					}
					else if(((inst->eSeverities & eBRB_LOG_SEVERITY_FILTER_ERROR) == eBRB_LOG_SEVERITY_FILTER_ERROR) && inst->ActEntry.nSeverity == arEVENTLOG_SEVERITY_ERROR)
					{
						bInFilter = 1;
					}
					if(bInFilter == 1)
					{
						inst->eStep = eSTEP_READ_DESCRIPTION;
					}
					else
					{
						if(inst->bFirstEntryIsListed == 0)
						{
							// Nächsten Eintrag lesen
							inst->ActEntry.nOriginRecordId = 0;
							inst->ActEntry.nEventId = 0;
							inst->ActEntry.nSeverity = 0;
							inst->eStep = eSTEP_GET_PREVIOUS_RECORDID;
						}
						else
						{
							// Alle passenden Einträge schon gelistet -> Ende
							inst->nStatus = 0;
							inst->eStep = eSTEP_INIT;
						}
					}
				}
				else
				{
					// Nächsten Eintrag lesen
					inst->ActEntry.nOriginRecordId = 0;
					inst->ActEntry.nEventId = 0;
					inst->ActEntry.nSeverity = 0;
					inst->eStep = eSTEP_GET_PREVIOUS_RECORDID;
				}
			}
			else if(inst->fbArEventLogReadErrorNumber.Error == 1)
			{
				memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
				inst->nStatus = inst->fbArEventLogReadErrorNumber.StatusID;
				inst->fbArEventLogReadErrorNumber.Execute = 0;
				ArEventLogReadErrorNumber(&inst->fbArEventLogReadErrorNumber);
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_READ_DESCRIPTION:
			// Description lesen
			inst->fbArEventLogReadDescription.Execute = 1;
			inst->fbArEventLogReadDescription.Ident = inst->nEventLogIdent;
			inst->fbArEventLogReadDescription.RecordID = inst->ActEntry.nRecordId;
			strcpy(inst->fbArEventLogReadDescription.LanguageCode, inst->sLanguageCode);
			inst->fbArEventLogReadDescription.TextBuffer = (UDINT)&inst->ActEntry.sDecription;
			inst->fbArEventLogReadDescription.TextBufferSize = sizeof(inst->ActEntry.sDecription);
			ArEventLogReadDescription(&inst->fbArEventLogReadDescription);
			if(inst->fbArEventLogReadDescription.Done == 1)
			{
				inst->fbArEventLogReadDescription.Execute = 0;
				ArEventLogReadDescription(&inst->fbArEventLogReadDescription);
				inst->eStep = eSTEP_INSERT_LIST_ENTRY;
			}
			else if(inst->fbArEventLogReadDescription.Error == 1)
			{
				memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
				inst->nStatus = inst->fbArEventLogReadDescription.StatusID;
				inst->fbArEventLogReadDescription.Execute = 0;
				ArEventLogReadDescription(&inst->fbArEventLogReadDescription);
				inst->eStep = eSTEP_INIT;
			}
			break;

		case eSTEP_INSERT_LIST_ENTRY:
			// Einfügen des Eintrags in die Liste
			BrbMemListIn(&inst->LogListManagement, 0, (UDINT)&inst->ActEntry);
			inst->nListEntryCount = inst->LogListManagement.nEntryCount;
			inst->bFirstEntryIsListed = 1;
			if(inst->ActEntry.nOriginRecordId != 0)
			{
				// Nächsten hierarchischen Eintrag lesen
				ArEventLogRecordIDType nOriginRecordId = inst->ActEntry.nOriginRecordId;
				memset((USINT*)&inst->ActEntry, 0, sizeof(inst->ActEntry));
				inst->ActEntry.nRecordId = nOriginRecordId;
				inst->eStep = eSTEP_READ_OBJECTID;
			}
			else
			{
				// Kein weiterer hierachischer Eintrag -> Ende
				inst->nStatus = 0;
				inst->eStep = eSTEP_INIT;
			}
			break;

	}
}
