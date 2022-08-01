(********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbLib.typ
 * Author: niedermeierr
 * Created: June 11, 2013
 ********************************************************************
 * Data types of library BrbLib
 ********************************************************************)
(*Error numbers*)

TYPE
	BrbError_ENUM : 
		(
		eBRB_ERR_OK := 0,
		eBRB_ERR_NULL_POINTER := 50000,
		eBRB_ERR_INVALID_PARAMETER := 50001,
		eBRB_ERR_NOT_ENABLED := 50002,
		eBRB_ERR_WRONG_VERSION := 50700,
		eBRB_ERR_BUSY := 65535
		);
END_TYPE

(*StepHandling*)

TYPE
	BrbStepHandlingCurrent_TYP : 	STRUCT 
		nStepNr : DINT; (*Aktuelle Schrittnummer*)
		sStepText : STRING[nBRB_STEP_TEXT_CHAR_MAX]; (*Aktueller Schritttext*)
		bTimeoutElapsed : BOOL; (*1=Timeout abgelaufen*)
		nTimeoutContinueStep : DINT; (*Schrittnummer nach Timeout*)
	END_STRUCT;
	BrbStepHandlingStep_TYP : 	STRUCT 
		nStepNr : DINT; (*Schrittnummer*)
		sStepText : STRING[nBRB_STEP_TEXT_CHAR_MAX]; (*Schritttext*)
		nCycleCount : UDINT; (*Zyklen*)
	END_STRUCT;
	BrbStepHandlingLog_TYP : 	STRUCT 
		bClear : BOOL; (*Kommando zum Löschen der Protokollierung*)
		bStop : BOOL; (*Kommando zum Stoppen der Protokollierung*)
		Steps : ARRAY[0..nBRB_STEPLOG_STEPS_MAX]OF BrbStepHandlingStep_TYP; (*Protokollierung*)
	END_STRUCT;
	BrbStepHandlingIntern_TYP : 	STRUCT 
		nStepNrOld : DINT; (*Alte Schrittnummer*)
		nCycleCount : UDINT; (*Aktueller Zykluszähler*)
		bLogOnNextCycle : BOOL; (*Flag bei Schrittwechsel*)
		fbTimeout : TON; (*Timeout-Fub*)
	END_STRUCT;
	BrbStepHandling_TYP : 	STRUCT 
		Current : BrbStepHandlingCurrent_TYP; (*Daten zum aktuellen Schritt*)
		Log : BrbStepHandlingLog_TYP; (*Protokollierung*)
		Intern : BrbStepHandlingIntern_TYP; (*Interne Daten*)
	END_STRUCT;
	BrbStopWatch_TYP : 	STRUCT 
		tStartTime : TIME; (*Start-Zeitstempel*)
		tStopTime : TIME; (*End-Zeitstempel*)
		nTimeDiff : UDINT; (*Berechnete Differenz*)
		sTimeDiff : STRING[24]; (*Differenz als Text*)
	END_STRUCT;
END_TYPE

(*TaskCommunication*)

TYPE
	BrbCallerStates_ENUM : 
		(
		eBRB_CALLER_STATE_NOT_READY := -1, (*-1=Nicht bereit*)
		eBRB_CALLER_STATE_OK := 0, (*0=Bereit*)
		eBRB_CALLER_STATE_BUSY := 1 (*1=Besetzt*)
		);
	BrbCaller_TYP : 	STRUCT 
		nCallerId : DINT; (*Task-ID*)
		bLock : BOOL; (*Semaphor-Flag*)
	END_STRUCT;
END_TYPE

(*VarHandling*)

TYPE
	BrbPvInfo_TYP : 	STRUCT 
		sName : STRING[nBRB_PVNAME_CHAR_MAX]; (*Variablen-Name*)
		nAdr : UDINT; (*Variablen-Adresse*)
		nDataType : UDINT; (*Variablen-Datentyp*)
		nLen : UDINT; (*Variablen-Größe*)
		nDimension : UINT; (*Max. Array-Index*)
		nItemIndex : UINT; (*Index des Array- oder Struktur-Items*)
	END_STRUCT;
END_TYPE

(*FileHandling*)

TYPE
	BrbUsbDeviceListEntry_TYP : 	STRUCT 
		sInterfaceName : STRING[nBRB_DEVICE_NAME_CHAR_MAX]; (*Schnittstellen-Name*)
		sDeviceName : STRING[nBRB_DEVICE_NAME_CHAR_MAX]; (*Schnittstellen-Name*)
		nNode : UDINT; (*Interne Node-Nummer*)
		nHandle : UDINT; (*Internes Handle*)
	END_STRUCT;
	BrbDirInfoFilter_ENUM : 
		(
		eBRB_DIR_INFO_ONLY_FILES, (*0=Nur Dateien*)
		eBRB_DIR_INFO_ONLY_DIRS, (*1=Nur Verzeichnisse*)
		eBRB_DIR_INFO_FILES_AND_DIRS (*2=Dateien und Verzeichnisse*)
		);
	BrbFileSorting_ENUM : 
		(
		eBRB_FILE_SORTING_NONE, (*0=Keine Sortierung*)
		eBRB_FILE_SORTING_ALPH_UP, (*1=Sortierung nach aufsteigendem Alphabet*)
		eBRB_FILE_SORTING_ALPH_DOWN, (*2=Sortierung nach aufsteigendem Alphabet*)
		eBRB_FILE_SORTING_OLDEST, (*3=Sortierung nach ältesten Dateien*)
		eBRB_FILE_SORTING_YOUNGEST, (*4=Sortierung nach jüngsten Dateien*)
		eBRB_FILE_SORTING_BIGGEST, (*5=Sortierung nach größten Dateien*)
		eBRB_FILE_SORTING_SMALLEST, (*6=Sortierung nach kleinsten Dateien*)
		eBRB_FILE_SORTING_USER (*7=Sortierung mit benutzerdefinierter Vergleichsfunktion*)
		);
	BrbReadDirListEntry_TYP : 	STRUCT 
		sName : STRING[nBRB_FILE_NAME_CHAR_MAX]; (*Verzeichnis- oder Datei-Name*)
		dtDate : DATE_AND_TIME; (*Zeitstempel*)
		nSize : UDINT; (*Größe (0=Verzeichnis)*)
	END_STRUCT;
END_TYPE

(*Logger*)

TYPE
	BrbLogHierarchListEntry_TYP : 	STRUCT 
		nRecordId : ArEventLogRecordIDType;
		nOriginRecordId : ArEventLogRecordIDType;
		nEventId : DINT;
		nSeverity : USINT;
		sObjectId : STRING[36];
		sDecription : STRING[512];
	END_STRUCT;
	BrbLogHierarchListSevFilter_ENUM : 
		(
		eBRB_LOG_SEVERITY_FILTER_ALL := 0, (*Keine Filterung*)
		eBRB_LOG_SEVERITY_FILTER_SUCCESS := 1, (*Nur Erfolgs-Einträge*)
		eBRB_LOG_SEVERITY_FILTER_INFO := 2, (*Nur Info-Einträge*)
		eBRB_LOG_SEVERITY_FILTER_WARNING := 4, (*Nur Warnungs-Einträge*)
		eBRB_LOG_SEVERITY_FILTER_ERROR := 8 (*Nur Fehler-Einträge*)
		);
END_TYPE

(*TimeAndDate*)

TYPE
	BrbWeekdays_ENUM : 
		( (*Wochentage*)
		eBRB_WD_SUNDAY, (*0=Sonntag*)
		eBRB_WD_MONDAY, (*1=Montag*)
		eBRB_WD_TUESDAY, (*2=Dienstag*)
		eBRB_WD_WEDNESDAY, (*3=Mittwoch*)
		eBRB_WD_THURSDAY, (*4=Donnerstag*)
		eBRB_WD_FRIDAY, (*5=Freitag*)
		eBRB_WD_SATURDAY (*6=Samstag*)
		);
	BrbTimeAndDateCompare_ENUM : 
		(
		eBRB_TAD_COMPARE_YOUNGER,
		eBRB_TAD_COMPARE_YOUNGEREQUAL,
		eBRB_TAD_COMPARE_EQUAL,
		eBRB_TAD_COMPARE_OLDEREQUAL,
		eBRB_TAD_COMPARE_OLDER
		);
	BrbTimerSwitchParTimePoint_TYP : 	STRUCT  (*Schaltpunkt*)
		bActive : BOOL; (*1=Zeitpunkt ist aktiv*)
		dtsTimePoint : DTStructure; (*Zeitpunkt*)
		eSwitchType : BrbTimerSwitchType_ENUM; (*Typ der Schaltung (Aus, An, Umschalten, Puls)*)
	END_STRUCT;
	BrbTimerSwitchPar_TYP : 	STRUCT  (*Parameter der Zeitschaltuhr*)
		eMode : BrbTimerSwitchMode_ENUM; (*Modus der Schaltung (Sekunde, Minute, Stunde...)*)
		TimePoint : ARRAY[0..nBRB_TIMERSWITCH_POINT_INDEX_MAX]OF BrbTimerSwitchParTimePoint_TYP; (*Liste der Schaltpunkte*)
	END_STRUCT;
	BrbTimerSwitchType_ENUM : 
		( (*Typ der Schaltung*)
		eBRB_TIMERSWITCH_TYPE_OFF, (*0 = Aus*)
		eBRB_TIMERSWITCH_TYPE_ON, (*1 = An*)
		eBRB_TIMERSWITCH_TYPE_TOGGLE, (*2 = Umschalten*)
		eBRB_TIMERSWITCH_TYPE_IMPULSE (*3 = Puls*)
		);
	BrbTimerSwitchMode_ENUM : 
		( (*Modus der Schaltung*)
		eBRB_TIMERSWITCH_MODE_MONTH, (*0=Monat*)
		eBRB_TIMERSWITCH_MODE_WEEK, (*1=Woche*)
		eBRB_TIMERSWITCH_MODE_DAY, (*2=Tag*)
		eBRB_TIMERSWITCH_MODE_HOUR, (*3=Stunde*)
		eBRB_TIMERSWITCH_MODE_MINUTE, (*4=Minute*)
		eBRB_TIMERSWITCH_MODE_SECOND (*5=Sekunde*)
		);
	BrbTimerSwitchWeekdays_ENUM : 
		( (*Wochentage*)
		eBRB_TIMERSWITCH_WD_SUNDAY, (*0=Sonntag*)
		eBRB_TIMERSWITCH_WD_MONDAY, (*1=Montag*)
		eBRB_TIMERSWITCH_WD_TUESDAY, (*2=Dienstag*)
		eBRB_TIMERSWITCH_WD_WEDNESDAY, (*3=Mittwoch*)
		eBRB_TIMERSWITCH_WD_THURSDAY, (*4=Donnerstag*)
		eBRB_TIMERSWITCH_WD_FRIDAY, (*5=Freitag*)
		eBRB_TIMERSWITCH_WD_SATURDAY (*6=Samstag*)
		);
END_TYPE

(*Memory*)

TYPE
	BrbMemListManagement_Typ : 	STRUCT 
		pList : UDINT; (*Adresse des Array-Anfangs*)
		nEntryLength : UDINT; (*Größe eines Eintrags*)
		nIndexMax : UINT; (*Maximaler Index des Arrays*)
		nEntryCount : UINT; (*Momentane Anzahl der gültigen Eintrage*)
	END_STRUCT;
END_TYPE

(*Math*)

TYPE
	BrbRound_ENUM : 
		(
		eBRB_ROUND_APPROP, (*Rundung gemäss letzter Stelle(n), Grenze bei 5*)
		eBRB_ROUND_UP, (*Aufrundung*)
		eBRB_ROUND_DOWN (*Abrundung*)
		);
END_TYPE

(*Random*)

TYPE
	BrbRandomString_ENUM : 
		(
		eBRB_RANDOM_STRING_NUMBERS := 1, (*Zahler*)
		eBRB_RANDOM_STRING_UPPER_LETTERS := 2, (*Großbuchstaben*)
		eBRB_RANDOM_STRING_LOWER_LETTERS := 4, (*Kleinbuchstaben*)
		eBRB_RANDOM_STRING_SPACE := 8, (*Leerzeichen*)
		eBRB_RANDOM_STRING_PUNCTUATION := 16, (*Satzzeichen*)
		eBRB_RANDOM_STRING_SYMBOLS := 32, (*Symbole*)
		eBRB_RANDOM_STRING_CONTROLCHARS := 64 (*Steuerzeichen*)
		);
END_TYPE

(*Additional*)

TYPE
	BrbCompilerVersion_TYP : 	STRUCT 
		bValid : BOOL; (*1=Version konnte ausgelesen werden*)
		nMajor : INT; (*Haupt-Versionsnummer*)
		nMinor : INT; (*Unter-Versionsnummer*)
		nPatch : INT; (*Patch-Versionsnummer*)
		nTotal : DINT; (*Komplette Versionnummer*)
		sText : STRING[10]; (*Versionsnummer als Text*)
	END_STRUCT;
END_TYPE
