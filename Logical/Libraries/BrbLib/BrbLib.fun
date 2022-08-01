(********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: BrbLib
 * File: BrbLib.fun
 * Author: niedermeierr
 * Created: June 11, 2013
 ********************************************************************
 * Functions and function blocks of library BrbLib
 ********************************************************************)
(*StepHandling*)

FUNCTION BrbStepHandler : DINT (*Behandelt Schrittketten (Protokollierung, Timeout usw.)*)
	VAR_INPUT
		pStepHandling : BrbStepHandling_TYP; (*Zeiger auf eine lokale Struktur "BrbStepHandling_TYP"*)
	END_VAR
END_FUNCTION

FUNCTION BrbStartStepTimeout : UINT (*Startet den Timeout-Zähler für eine Schrittkette*)
	VAR_INPUT
		pStepHandling : BrbStepHandling_TYP; (*Zeiger auf eine lokale Struktur "BrbStepHandling_TYP"*)
		nTimeout : UDINT; (*Zeitangabe in [ms]*)
		nContinueStep : DINT; (*Schrittnummer bei abgelaufenem Timeout*)
	END_VAR
END_FUNCTION

FUNCTION BrbStopStepTimeout : UINT (*Stoppt den Timeout-Zähler für eine Schrittkette*)
	VAR_INPUT
		pStepHandling : BrbStepHandling_TYP; (*Zeiger auf eine lokale Struktur "BrbStepHandling_TYP"*)
	END_VAR
END_FUNCTION

FUNCTION BrbStartStopWatch : BOOL (*Startet eine Stoppuhr*)
	VAR_INPUT
		pStopWatch : BrbStopWatch_TYP; (*Zeiger auf eine lokale Struktur "BrbStopWatch_TYP"*)
	END_VAR
END_FUNCTION

FUNCTION BrbStopStopWatch : UDINT (*Stoppt eine Stoppuhr*)
	VAR_INPUT
		pStopWatch : BrbStopWatch_TYP; (*Zeiger auf eine lokale Struktur "BrbStopWatch_TYP"*)
	END_VAR
END_FUNCTION
(*TaskCommunication*)

FUNCTION BrbSetCaller : BrbCallerStates_ENUM (*Versucht, die Reservierung für ein Task-Kommando zu erhalten*)
	VAR_INPUT
		pCaller : BrbCaller_TYP; (*Zeiger auf eine Struktur "BrbCaller_TYP"*)
		nCallerId : DINT; (*Identifier für den aufrufenden Task*)
	END_VAR
END_FUNCTION

FUNCTION BrbClearDirectBox : UINT (*Setzt eine DirectBox komplett auf 0*)
	VAR_INPUT
		pDirectBox : UDINT; (*Zeiger auf eine DirectBox-Struktur*)
		nSize : UDINT; (*Größe der Struktur*)
	END_VAR
END_FUNCTION

FUNCTION BrbClearCallerBox : UINT (*Setzt eine CallerBox komplett auf 0*)
	VAR_INPUT
		pCallerBox : UDINT; (*Zeiger auf eine CallerBox-Struktur*)
		nSize : UDINT; (*Größe der Struktur*)
	END_VAR
END_FUNCTION
(*VarHandling*)

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbSaveVarAscii (*Speichert eine Variable mit allen Elementen in einer Ascii-Datei*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		pVarName : REFERENCE TO STRING[0]; (*Zeiger auf den Variablen-Namen*)
		nLinesToWriteAtOneStep : {REDUND_UNREPLICABLE} UINT; (*Anzahl der Zeilen, die in einem Aufruf geschrieben werden*)
	END_VAR
	VAR_OUTPUT
		nCharCountMaxPerWrite : {REDUND_UNREPLICABLE} UDINT; (*Größte Anzahl der Zeichen, welche in einer Zeile geschrieben wurden*)
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		fbFileDelete : {REDUND_UNREPLICABLE} FileDelete; (*Fub zum Löschen der Datei*)
		fbFileCreate : {REDUND_UNREPLICABLE} FileCreate; (*Fub zum Erzeugen der Datei*)
		sHelp : {REDUND_UNREPLICABLE} STRING[nBRB_PVNAME_CHAR_MAX]; (*Hilfsstring*)
		nOffset : {REDUND_UNREPLICABLE} UDINT; (*Schreib-Offset*)
		nLineCountWrite : {REDUND_UNREPLICABLE} UDINT; (*Zeilen-Zähler*)
		bVarFinished : {REDUND_UNREPLICABLE} BOOL; (*1= Variable komplett durchgetickert*)
		PvInfo : {REDUND_UNREPLICABLE} ARRAY[0..16] OF BrbPvInfo_TYP; (*Struktur für Variablen-Daten*)
		nPvLevel : {REDUND_UNREPLICABLE} UINT; (*Hierarchie-Level für Strukturen*)
		sLines : {REDUND_UNREPLICABLE} STRING[nBRB_FILE_LINES_CHAR_MAX]; (*Text-Zeilen*)
		fbFileWrite : {REDUND_UNREPLICABLE} FileWrite; (*Fub zum Schreiben der Datei*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Fub zum Schliessen der Datei*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbLoadVarAscii (*Lädt eine Ascii-Datei mit Variablen-Werten*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		nLinesToReadAtOneStep : {REDUND_UNREPLICABLE} UINT; (*Anzahl der Zeilen, die in einem Aufruf gelesen werden*)
	END_VAR
	VAR_OUTPUT
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		fbDatObjInfo : {REDUND_UNREPLICABLE} DatObjInfo; (*Fub zum Holen der Datenobjekt-Info*)
		fbFileOpen : {REDUND_UNREPLICABLE} FileOpen; (*Fub zum Öffnen der Datei*)
		fbDatObjCreate : {REDUND_UNREPLICABLE} DatObjCreate; (*Fub zum Erzeugen des Datenobjekts*)
		pTextStart : REFERENCE TO STRING[0]; (*Zeiger auf den Anfang des eingelesenen Texts*)
		pTextEnd : REFERENCE TO STRING[0]; (*Zeiger auf das Ende des eingelesenen Texts*)
		pText : REFERENCE TO STRING[0]; (*Zeiger auf den aktuellen Text*)
		nLineCountRead : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der eingelesenen Zeilen*)
		fbFileRead : {REDUND_UNREPLICABLE} FileRead; (*Fub zum Lesen der Datei*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Fub zum Schliessen der Datei*)
		fbDatObjDelete : {REDUND_UNREPLICABLE} DatObjDelete; (*Fub zum Löschen des Datenobjekts*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbSaveVarBin (*Speichert eine Variable mit allen Elementen in einer Binär-Datei*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		pVarName : REFERENCE TO STRING[0]; (*Zeiger auf den Variablen-Namen*)
	END_VAR
	VAR_OUTPUT
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		PvInfo : {REDUND_UNREPLICABLE} BrbPvInfo_TYP; (*Struktur für Variablen-Daten*)
		fbFileDelete : {REDUND_UNREPLICABLE} FileDelete; (*Fub zum Löschen der Datei*)
		fbFileCreate : {REDUND_UNREPLICABLE} FileCreate; (*Fub zum Erzeugen der Datei*)
		fbFileWrite : {REDUND_UNREPLICABLE} FileWrite; (*Fub zum Schreiben der Datei*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Fub zum Schliessen der Datei*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbLoadVarBin (*Lädt eine Binär-Datei mit Variablen-Werten*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		pVarName : REFERENCE TO STRING[0]; (*Zeiger auf den Variablen-Namen*)
		bAllowBiggerVar : {REDUND_UNREPLICABLE} BOOL; (*1=Der Speicher der Variablen darf größer als die Datei sein*)
	END_VAR
	VAR_OUTPUT
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		fbFileOpen : {REDUND_UNREPLICABLE} FileOpen; (*Fub zum Öffnen der Datei*)
		PvInfo : {REDUND_UNREPLICABLE} BrbPvInfo_TYP; (*Struktur für Variablen-Daten*)
		fbFileRead : {REDUND_UNREPLICABLE} FileRead; (*Fub zum Lesen der Datei*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Fub zum Schliessen der Datei*)
	END_VAR
END_FUNCTION_BLOCK
(*FileHandling*)

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbReadDir (*Gibt eine Liste der Dateien und/oder der Verzeichnisse eines Laufwerks zurück*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pPath : REFERENCE TO STRING[0]; (*Zeiger auf den Pfad*)
		eFilter : {REDUND_UNREPLICABLE} BrbDirInfoFilter_ENUM; (*Datei- und Verzeichnis-Filter*)
		bWithParentDir : {REDUND_UNREPLICABLE} BOOL; (*1=Mit übergeordnetem Verzeichnis*)
		pFileFilter : REFERENCE TO STRING[0]; (*Zeiger auf den Filtertext für Datei-Endungen*)
		bUserFilter : {REDUND_UNREPLICABLE} BOOL; (*1=Benutzerdefinierte Filterfunktion wird zusätzlich verwendet*)
		pUserFilterFunction : {REDUND_UNREPLICABLE} UDINT; (*Zeiger auf die benutzerdefinierte Filterfunktion*)
		eSorting : {REDUND_UNREPLICABLE} BrbFileSorting_ENUM; (*Sortierung*)
		bCaseSensitive : {REDUND_UNREPLICABLE} BOOL; (*1=Groß-/Klein-Schreibung beachten (gitl nur bei alphanumerischer Sortierung)*)
		pUserCompareFunction : {REDUND_UNREPLICABLE} UDINT; (*Zeiger auf die benutzerdefinierte Vergleichsfunktion bei 'eBRB_FILE_SORTING_USER'*)
		pList : {REDUND_UNREPLICABLE} UDINT; (*Zeiger auf das zu füllende Array*)
		nListIndexMax : {REDUND_UNREPLICABLE} UDINT; (*Maximaler Anzahl der Einträge im Array*)
	END_VAR
	VAR_OUTPUT
		nDirCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der Verzeichnisse in der Liste*)
		nFileCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der Dateien in der Liste*)
		nTotalCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der Verzeichnisse und Dateien in der Liste*)
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		FilterListMan : {REDUND_UNREPLICABLE} BrbMemListManagement_Typ; (*Verwaltung der Filter-Liste*)
		FilterList : {REDUND_UNREPLICABLE} ARRAY[0..nBRB_FILTER_SINGLE_INDEX_MAX] OF STRING[nBRB_FILTER_SINGLE_CHAR_MAX]; (*Liste der Filter*)
		fbDirRead : {REDUND_UNREPLICABLE} DirRead; (*Fub zum Auslesen des Laufwerks*)
		FileInfo : {REDUND_UNREPLICABLE} fiDIR_READ_DATA; (*Daten der aktueller Datei*)
		ReadListEntry : {REDUND_UNREPLICABLE} BrbReadDirListEntry_TYP; (*Daten der aktueller Datei*)
		FileListMan : {REDUND_UNREPLICABLE} BrbMemListManagement_Typ; (*Verwaltung der Datei-Liste*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbCheckUsbSticks (*Prüft, ob ein Usb-Stick gesteckt wurde*)
	VAR_INPUT
		bAutoLink : {REDUND_UNREPLICABLE} BOOL; (*1=Usb-Device wird beim Stecken automatisch gelinkt*)
	END_VAR
	VAR_OUTPUT
		nUsbDeviceCount : {REDUND_UNREPLICABLE} UINT; (*Anzahl der gesteckten Usb-Sticks*)
		bUsbDeviceCountChanged : {REDUND_UNREPLICABLE} BOOL; (*Anzahl hat sich geändert (nur 1 für einen Zyklus)*)
		UsbDeviceList : {REDUND_UNREPLICABLE} ARRAY[0..nBRB_USB_DEVICE_LIST_INDEX_MAX] OF BrbUsbDeviceListEntry_TYP; (*Liste der Usb-Devices*)
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		DeviceListMan : {REDUND_UNREPLICABLE} BrbMemListManagement_Typ; (*Verwaltung der Device-Liste*)
		nUsbDeviceIndex : {REDUND_UNREPLICABLE} UINT; (*Aktueller Index in der Device-Liste*)
		UsbDeviceListEntry : {REDUND_UNREPLICABLE} BrbUsbDeviceListEntry_TYP; (*Aktueller Eintrag in der Device-Liste*)
		fbUsbNodeListGet : {REDUND_UNREPLICABLE} UsbNodeListGet; (*Fub zum Lesen der Node-Liste*)
		nUsbNodeIndex : {REDUND_UNREPLICABLE} UINT; (*Aktueller Index in der Node-Liste*)
		UsbNodeList : {REDUND_UNREPLICABLE} ARRAY[0..nBRB_USB_DEVICE_LIST_INDEX_MAX] OF UDINT; (*Node-Liste*)
		nAttachDetachCountOld : {REDUND_UNREPLICABLE} UDINT; (*Vergleichs-Zähler*)
		fbDevUnlink : {REDUND_UNREPLICABLE} DevUnlink; (*Fub zum Unlinken*)
		fbUsbNodeGet : {REDUND_UNREPLICABLE} UsbNodeGet; (*Fub zum Lesen der Node-Info*)
		UsbNodeInfo : {REDUND_UNREPLICABLE} usbNode_typ; (*Node-Info*)
		sLinkParameter : {REDUND_UNREPLICABLE} STRING[100]; (*Parameter zum Linken*)
		fbDevLink : {REDUND_UNREPLICABLE} DevLink; (*Fub zum Linken*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbDeleteFiles (*Löscht Dateien, die über Filter festgelegt sind*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pPath : REFERENCE TO STRING[0]; (*Zeiger auf den Pfad*)
		pFileFilter : REFERENCE TO STRING[0]; (*Zeiger auf den Filtertext für Datei-Endungen*)
		dtDateStart : {REDUND_UNREPLICABLE} DATE_AND_TIME; (*Angabe des Start-Datums*)
		dtDateEnd : {REDUND_UNREPLICABLE} DATE_AND_TIME; (*Angabe des Ende-Datums*)
	END_VAR
	VAR_OUTPUT
		nDeletedFileCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der gelöschten Dateien*)
		nKeptFileCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der nicht gelöschten Dateien*)
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		FilterListMan : {REDUND_UNREPLICABLE} BrbMemListManagement_Typ; (*Verwaltung der Filter-Liste*)
		FilterList : {REDUND_UNREPLICABLE} ARRAY[0..nBRB_FILTER_SINGLE_INDEX_MAX] OF STRING[nBRB_FILTER_SINGLE_CHAR_MAX]; (*Liste der Filter*)
		fbDirRead : {REDUND_UNREPLICABLE} DirRead; (*Fub zum Auslesen des Laufwerks*)
		FileInfo : {REDUND_UNREPLICABLE} fiDIR_READ_DATA; (*Daten der aktueller Datei*)
		sFileWithPath : {REDUND_UNREPLICABLE} STRING[nBRB_FILE_NAME_CHAR_MAX]; (*Pfad mit Dateiname*)
		fbFileDelete : {REDUND_UNREPLICABLE} FileDelete; (*Fub zum Auslesen des Laufwerks*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbLoadFileDataObj (*Lädt eine Datei in ein Datenobjekt*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		pDataObjName : REFERENCE TO STRING[0]; (*Zeiger auf den Datenobjekt-Namen*)
		nDataObjMemType : {REDUND_UNREPLICABLE} USINT; (*Angabe über den Speicher-Typ des Datenobjekts*)
		nDataObjOption : {REDUND_UNREPLICABLE} UDINT; (*Angabe über die Option des Datenobjekts*)
	END_VAR
	VAR_OUTPUT
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
		nDataObjIdent : {REDUND_UNREPLICABLE} UDINT; (*Ident des Datenobjekts*)
		pDataObjMem : {REDUND_UNREPLICABLE} UDINT; (*Zeiger auf die Daten des Datenobjekts*)
		nDataObjLen : {REDUND_UNREPLICABLE} UDINT; (*Länge des Datenobjekts (= Länge der Datei)*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		fbDatObjInfo : {REDUND_UNREPLICABLE} DatObjInfo; (*Interner Fub*)
		fbDatObjDelete : {REDUND_UNREPLICABLE} DatObjDelete; (*Interner Fub*)
		fbFileOpen : {REDUND_UNREPLICABLE} FileOpen; (*Interner Fub*)
		fbDatObjCreate : {REDUND_UNREPLICABLE} DatObjCreate; (*Interner Fub*)
		fbFileRead : {REDUND_UNREPLICABLE} FileRead; (*Interner Fub*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Interner Fub*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbSaveFileDataObj (*Speichert ein Datenobjekt in eine Datei*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		pDataObjName : REFERENCE TO STRING[0]; (*Zeiger auf den Datenobjekt-Namen*)
	END_VAR
	VAR_OUTPUT
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		fbDatObjInfo : {REDUND_UNREPLICABLE} DatObjInfo; (*Interner Fub*)
		fbFileInfo : {REDUND_UNREPLICABLE} FileInfo; (*Interner Fub*)
		FileInfo : {REDUND_UNREPLICABLE} fiFILE_INFO; (*Interne Daten*)
		fbFileDelete : {REDUND_UNREPLICABLE} FileDelete; (*Interner Fub*)
		fbFileCreate : {REDUND_UNREPLICABLE} FileCreate; (*Interner Fub*)
		fbFileWrite : {REDUND_UNREPLICABLE} FileWrite; (*Interner Fub*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Interner Fub*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbLoadFileBin (*Lädt eine Datei in eine Feld-Variable*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pDevice : REFERENCE TO STRING[0]; (*Zeiger auf den Laufwerks-Namen*)
		pFile : REFERENCE TO STRING[0]; (*Zeiger auf den Datei-Namen*)
		pVar : REFERENCE TO USINT; (*Zeiger auf die Feld-Variable*)
		nVarSize : {REDUND_UNREPLICABLE} UDINT; (*Größe der Feld-Variable*)
	END_VAR
	VAR_OUTPUT
		nStatus : {REDUND_UNREPLICABLE} UINT; (*Fub-Status*)
		nValidBytes : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der gelesenen Bytes*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		nStatusIntern : {REDUND_UNREPLICABLE} UINT; (*Interner Fub-Status*)
		fbFileOpen : {REDUND_UNREPLICABLE} FileOpen; (*Interner Fub*)
		fbFileReadEx : {REDUND_UNREPLICABLE} FileReadEx; (*Interner Fub*)
		fbFileClose : {REDUND_UNREPLICABLE} FileClose; (*Interner Fub*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION BrbCheckFileName : UINT (*Ersetzt Sonderzeichen in einem Datei-Namen mit Unterstrichen*)
	VAR_INPUT
		pFileName : STRING[0]; (*Zeiger auf den Datei-Namen*)
	END_VAR
END_FUNCTION

FUNCTION BrbCheckFileEnding : BOOL (*Prüft die Endung eines Datei-Namens und berichtigt sie gegebenenfalls*)
	VAR_INPUT
		pFileName : STRING[0]; (*Zeiger auf den Datei-Namen*)
		pEnding : STRING[0]; (*Zeiger auf die Datei-Endung (mit oder ohne Punkt)*)
	END_VAR
END_FUNCTION

FUNCTION BrbCombinePath : UINT (*Fügt zwei Pfade oder einen Pfad und eine Datei-Namen zusammen*)
	VAR_INPUT
		pPath : STRING[0]; (*Zeiger auf Pfad*)
		pFilename : STRING[0]; (*Zeiger auf Pfad oder Datei-Namen*)
		pFilenameWithPath : STRING[0]; (*Zeiger auf den String, der das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION
(*Logger*)

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbLoggerReadHierarchicalList (*Liest die zu einem definierten Logger-Eintrag gehörende Hierarchie-Liste von Einträgen aus*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		sLogbookName : {REDUND_UNREPLICABLE} STRING[256]; (*Name des Logbuchs*)
		nStartRecordId : {REDUND_UNREPLICABLE} ArEventLogRecordIDType; (*RecordId, bei dem die Suche beginnen soll (0=Beginn bei jüngstem Eintrag)*)
		sObjectId : {REDUND_UNREPLICABLE} STRING[36]; (*ObjectId-Filter ('Entered by'-Angabe)*)
		nStartEventId : {REDUND_UNREPLICABLE} DINT; (*EventId oder ErrorNumber, bei dem die Liste beginnen soll (0=keine Filterung)*)
		eSeverities : {REDUND_UNREPLICABLE} BrbLogHierarchListSevFilter_ENUM; (*Bitweise codierte Filterung der Severities (0=keine Filterung)*)
		sLanguageCode : {REDUND_UNREPLICABLE} ArEventLogLanguageCodeType; (*Sprachkürzel*)
		nSearchEntryCountLimit : {REDUND_UNREPLICABLE} UDINT; (*Limitierung der durchsuchten Einträge (0=kein Limit)*)
		pLogList : {REDUND_UNREPLICABLE} UDINT; (*Zeiger auf die zu füllende Liste*)
		nLogListIndexMax : {REDUND_UNREPLICABLE} UINT; (*Maximaler Index der Liste*)
	END_VAR
	VAR_OUTPUT
		nListEntryCount : {REDUND_UNREPLICABLE} UINT; (*Anzahl der Einträge in der Liste*)
		nStatus : {REDUND_UNREPLICABLE} DINT; (*Fub-Status*)
	END_VAR
	VAR
		eStep : {REDUND_UNREPLICABLE} DINT; (*Step-Nummer für die interne Schrittkette*)
		sLastLogbookName : {REDUND_UNREPLICABLE} STRING[256]; (*Name des Logbuchs beim letzten Aufruf*)
		fbArEventLogGetIdent : {REDUND_UNREPLICABLE} ArEventLogGetIdent; (*Interner Fub*)
		nEventLogIdent : {REDUND_UNREPLICABLE} ArEventLogIdentType; (*Ident des Logbuchs*)
		ActEntry : {REDUND_UNREPLICABLE} BrbLogHierarchListEntry_TYP; (*Aktuell ermittelter Eintrag*)
		fbArEventLogGetLatestRecordID : {REDUND_UNREPLICABLE} ArEventLogGetLatestRecordID; (*Interner Fub*)
		fbArEventLogGetPreviousRecordID : {REDUND_UNREPLICABLE} ArEventLogGetPreviousRecordID; (*Interner Fub*)
		fbArEventLogRead : {REDUND_UNREPLICABLE} ArEventLogRead; (*Interner Fub*)
		fbArEventLogReadErrorNumber : {REDUND_UNREPLICABLE} ArEventLogReadErrorNumber; (*Interner Fub*)
		fbArEventLogReadObjectID : {REDUND_UNREPLICABLE} ArEventLogReadObjectID; (*Interner Fub*)
		fbArEventLogReadDescription : {REDUND_UNREPLICABLE} ArEventLogReadDescription; (*Interner Fub*)
		LogListManagement : {REDUND_UNREPLICABLE} BrbMemListManagement_Typ; (*Verwaltung der Liste*)
		nSearchEntryCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der durchsuchten Einträge*)
		bFirstEntryIsListed : {REDUND_UNREPLICABLE} BOOL; (*Erster Eintrag lt. Filter eingetragen*)
	END_VAR
END_FUNCTION_BLOCK
(*TimeAndDate*)

FUNCTION BrbSetDtStruct : DATE_AND_TIME (*Setzt eine DTStructure-Zeit*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Zeit-Struktur*)
		nYear : UINT; (*Jahr*)
		nMonth : USINT; (*Monat*)
		nDay : USINT; (*Tage*)
		nHour : USINT; (*Stunde*)
		nMinute : USINT; (*Minute*)
		nSecond : USINT; (*Sekunde*)
		nMillisecond : UINT; (*Millisekunde*)
		nMicrosecond : UINT; (*Mikrosekunden*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetDt : DATE_AND_TIME (*Setzt eine DATE_AND_TIME-Zeit*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		nYear : UINT; (*Jahr*)
		nMonth : USINT; (*Monat*)
		nDay : USINT; (*Tage*)
		nHour : USINT; (*Stunde*)
		nMinute : USINT; (*Minute*)
		nSecond : USINT; (*Sekunde*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetTimespan : TIME (*Setzt eine TIME-Zeitspanne*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		nSign : SINT; (*Vorzeichen*)
		nDays : USINT; (*Tage*)
		nHours : UINT; (*Stunden*)
		nMinutes : UINT; (*Minuten*)
		nSeconds : UDINT; (*Sekunden*)
		nMilliseconds : UDINT; (*Millisekunden*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetTimespanT : TIME (*Setzt eine TIME-Zeitspanne*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pTimeText : STRING[0]; (*Zeiger auf die Text-Angabe*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetTimeText : UINT (*Gibt die angegebene RTC-Zeit als Text zurück*)
	VAR_INPUT
		pTime : RTCtime_typ; (*Zeiger auf eine Zeit-Struktur*)
		pText : STRING[0]; (*Zeiger auf den String*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		pFormat : STRING[0]; (*Zeiger auf den Formatierungstext*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetCurrentTimeText : UINT (*Gibt die aktuelle Zeit als Text zurück*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den String*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		pFormat : STRING[0]; (*Zeiger auf den Formatierungstext*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetTimeTextDtStruct : UINT (*Gibt die angegebene DtStruct-Zeit als Text zurück*)
	VAR_INPUT
		pTime : DTStructure; (*Zeiger auf eine Zeit-Struktur*)
		pText : STRING[0]; (*Zeiger auf den String*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		pFormat : STRING[0]; (*Zeiger auf den Formatierungstext*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetTimeTextDt : UINT (*Gibt die angegebene DT-Zeit als Text zurück*)
	VAR_INPUT
		dtTime : DATE_AND_TIME; (*Zeiger auf eine DT-Zeit*)
		pText : STRING[0]; (*Zeiger auf den String*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		pFormat : STRING[0]; (*Zeiger auf den Formatierungstext*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetDtFromTimeText : DATE_AND_TIME (*Wandelt einen Zeit-Text in eine Zeit um*)
	VAR_INPUT
		pTimeText : STRING[0]; (*Zeiger auf den Zeit-Text im Format*)
		pFormat : STRING[0]; (*Zeiger auf den Formatierungstext*)
	END_VAR
END_FUNCTION

FUNCTION BrbRtcTimeToDtStruct : UINT (*Wandelt eine RTC-Zeit in eine DtStruct-Zeit*)
	VAR_INPUT
		pRtcTime : RTCtime_typ; (*Zeiger auf eine Struktur "RTCtime_typ"*)
		pDtStruct : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructCompare : BOOL (*Vergleicht zwei DtStructures*)
	VAR_INPUT
		pDtStruct1 : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
		eCompare : BrbTimeAndDateCompare_ENUM; (*Vergleichs-Angabe*)
		pDtStruct2 : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructAddDays : DATE_AND_TIME (*Addiert zu einer DTStructure die angegebenen Tage*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
		nDays : DINT; (*Zu addierende Tage*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructAddHours : DATE_AND_TIME (*Addiert zu einer DTStructure die angegebenen Stunden*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
		nHours : DINT; (*Zu addierende Stunden*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructAddMinutes : DATE_AND_TIME (*Addiert zu einer DTStructure die angegebenen Minuten*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
		nMinutes : DINT; (*Zu addierende Minuten*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructAddSeconds : DATE_AND_TIME (*Addiert zu einer DTStructure die angegebenen Sekunden*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Struktur "DTStructure"*)
		nSeconds : DINT; (*Zu addierende Sekunden*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructAddMilliseconds : DATE_AND_TIME (*Addiert zu einer DTStructure die angegebenen Millisekunden*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Zeit-Struktur*)
		nMilliseconds : DINT; (*Millisekunden*)
	END_VAR
END_FUNCTION

FUNCTION BrbDtStructAddMillisecondsLReal : DATE_AND_TIME (*Addiert zu einer DTStructure die angegebenen Millisekunden*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Zeit-Struktur*)
		rMilliseconds : LREAL; (*Millisekunden*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetTimeTextMs : UINT (*Gibt die angebenene Millisekunden-Zeit als Text zurück*) (*$GROUP=User*)
	VAR_INPUT
		nMilliseconds : DINT; (*Anzahl der Millisekunden*)
		pText : STRING[0]; (*Zeiger auf den String*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		pFormat : STRING[0]; (*Zeiger auf den Formatierungstext*)
		bClip : BOOL; (*Abziehen der nicht im Format berücksichtigten Teilwerte*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetTimeDiffMsDtStruct : DINT (*Gibt die Zeit-Differenz zweier DtStruct-Zeiten in [ms] zurück*) (*$GROUP=User*)
	VAR_INPUT
		pTimeStampBase : DTStructure; (*Basis-Zeitstempel*)
		pTimeStampCompare : DTStructure; (*Vergleichs-Zeitstempel*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetWeekdayDtStruct : BrbWeekdays_ENUM (*Ermittelt den Wochentag einer DTStructure*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pDtStruct : DTStructure; (*Zeiger auf eine Zeit-Struktur*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetWeekdayDt : BrbWeekdays_ENUM (*Gibt den Wochentag einer DT-Zeit zurück*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		dtTime : DATE_AND_TIME; (*Zeit-Angabe*)
	END_VAR
END_FUNCTION

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbTimerSwitch (*Zeitschaltuhr für einen Kanal*)
	VAR_INPUT
		bEnable : {REDUND_UNREPLICABLE} BOOL; (*1=Baustein wird ausgeführt*)
		bCmdSwitchOff : {REDUND_UNREPLICABLE} BOOL; (*1=Ausgangs-Kanal wird ausgeschaltet*)
		bCmdSwitchOn : {REDUND_UNREPLICABLE} BOOL; (*1=Ausgangs-Kanal wird eingeschaltet*)
		bCmdToggle : {REDUND_UNREPLICABLE} BOOL; (*1=Ausgangs-Kanal wird umgeschaltet*)
		pUserTime : REFERENCE TO DTStructure; (*Optionaler Zeiger auf Benutzer-Zeitstempel*)
		Parameter : {REDUND_UNREPLICABLE} BrbTimerSwitchPar_TYP; (*Parameter-Struktur*)
	END_VAR
	VAR_OUTPUT
		bOut : {REDUND_UNREPLICABLE} BOOL; (*Ausgangs-Kanal*)
		dtsUsedTime : {REDUND_UNREPLICABLE} DTStructure; (*Verwendete Zeit*)
		nSwitchCount : {REDUND_UNREPLICABLE} UDINT; (*Anzahl der Schaltungen*)
	END_VAR
	VAR
		fbDTStructureGetTime : {REDUND_UNREPLICABLE} DTStructureGetTime; (*Interner Fub*)
		bLocked : {REDUND_UNREPLICABLE} BOOL; (*Interne Variable*)
		fbLock : {REDUND_UNREPLICABLE} TON; (*Interner Fub*)
		bClearOutOnNextCycle : {REDUND_UNREPLICABLE} BOOL; (*Interne Variable*)
	END_VAR
END_FUNCTION_BLOCK
(*Strings*)

FUNCTION BrbUsintToHex : UINT (*Wandelt einen Usint in einen Hex-String*)
	VAR_INPUT
		nValue : USINT; (*USINT-Wert*)
		pHex : STRING[0]; (*Zeiger auf den String*)
		nHexSize : UDINT; (*Größe der String-Variablen*)
		bWithPraefix : BOOL; (*1=Mit Präfix '0x'*)
	END_VAR
END_FUNCTION

FUNCTION BrbUsintArrayToHex : UINT (*Wandelt ein Usint-Array in einen Hex-String*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pArray : REFERENCE TO USINT; (*Zeiger auf das Array*)
		nArrayLength : DINT; (*Länge des Arrays*)
		pHex : STRING[0]; (*Zeiger auf den String*)
		nHexSize : UDINT; (*Größe der String-Variablen*)
		bDescending : BOOL; (*0=Aufsteigend, 1=Absteigend*)
	END_VAR
END_FUNCTION

{REDUND_ERROR} FUNCTION BrbHexToUsintArray : UINT (*Wandelt einen Hex-String in ein Usint-Array*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pHex : STRING[0]; (*Zeiger auf den String*)
		pArray : REFERENCE TO USINT; (*Zeiger auf das Array*)
		nArrayLength : DINT; (*Länge des Arrays*)
		bDescending : BOOL; (*0=Aufsteigend, 1=Absteigend*)
	END_VAR
END_FUNCTION

FUNCTION BrbUdintToAscii : UINT (*Wandelt einen UDINT-Wert zu einem String*)
	VAR_INPUT
		nValue : UDINT; (*UDINT-Wert*)
		pText : STRING[0]; (*Zeiger auf den String*)
	END_VAR
END_FUNCTION

FUNCTION BrbAsciiToUdint : UDINT (*Wandelt einen String zu einem UDINT-Wert*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den String*)
	END_VAR
END_FUNCTION

FUNCTION BrbUdintToHex : UINT (*Wandelt einen Udint in einen Hex-String*)
	VAR_INPUT
		nValue : UDINT; (*UDINT-Wert*)
		pHex : STRING[0]; (*Zeiger auf den String*)
		nHexSize : UDINT; (*Größe der String-Variablen*)
		bWithPraefix : BOOL; (*1=Mit Präfix '0x'*)
	END_VAR
END_FUNCTION

FUNCTION BrbHexToUdint : UDINT (*Wandelt einen Hex-String in einen Udint*)
	VAR_INPUT
		pHex : STRING[0]; (*Zeiger auf den String*)
	END_VAR
END_FUNCTION

FUNCTION BrbUdintToBin : UINT (*Wandelt einen Udint-Wert in einen Binär-String*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		nValue : UDINT; (*UDINT-Wert*)
		pBin : STRING[0]; (*Zeiger auf den String*)
		nBinSize : UDINT; (*Größe der String-Variablen*)
		bWithPraefix : BOOL; (*1=Mit Präfix '%'*)
	END_VAR
END_FUNCTION

FUNCTION BrbDintToHex : UINT (*Wandelt einen DINT in einen Hex-String*) (*$GROUP=User*)
	VAR_INPUT
		nValue : DINT; (*DINT-Wert*)
		pHex : STRING[0]; (*Zeiger auf den String*)
		nHexSize : UDINT; (*Größe der String-Variablen*)
		bWithPraefix : BOOL; (*1=Mit Präfix '0x'*)
	END_VAR
END_FUNCTION

FUNCTION BrbHexToDint : DINT (*Wandelt einen Hex-String in einen Dint*) (*$GROUP=User*)
	VAR_INPUT
		pHex : STRING[0]; (*Zeiger auf den String*)
	END_VAR
END_FUNCTION

FUNCTION BrbAsciiFieldToString : BOOL (*Wandelt ein Ascii-Feld in einen lesbaren Text*)
	VAR_INPUT
		pAsciiField : REFERENCE TO USINT; (*Zeiger auf das Ascii-Feld*)
		nAsciiFieldLen : UDINT; (*Länge des Ascii-Felds*)
		nFinalAsciiCharCount : UDINT; (*Anzahl der letzten Ascii-Zeichen, die auf jeden Fall enthalten sein sollten*)
		pText : STRING[0]; (*Zeiger auf den String*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetIndexOf : DINT (*Gibt den ersten Index eines zu suchenden Textes zurück*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetLastIndexOf : DINT (*Gibt den letzten Index eines zu suchenden Textes zurück*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetAdrOf : STRING[0] (*Gibt die erste Adresse eines zu suchenden Textes zurück*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetLastAdrOf : STRING[0] (*Gibt die letzte Adresse eines zu suchenden Textes zurück*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringStartsWith : BOOL (*Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge beginnt*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringEndsWith : BOOL (*Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge endet*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetSubText : STRING[0] (*Gibt eine Zeichenfolge innerhalb eines Textes zurück*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		nIndex : UDINT; (*Start-Index*)
		nLen : UDINT; (*Länge*)
		pSubText : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetSubTextByLen : STRING[0] (*Gibt eine Zeichenfolge innerhalb eines Textes zurück*)
	VAR_INPUT
		pStart : STRING[0]; (*Zeiger auf den Start der Zeichenfolge*)
		nLen : UDINT; (*Länge*)
		pSubText : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringGetSubTextByAdr : STRING[0] (*Gibt eine Zeichenfolge innerhalb eines Textes zurück*)
	VAR_INPUT
		pStart : STRING[0]; (*Zeiger auf den Start der Zeichenfolge*)
		pEnd : STRING[0]; (*Zeiger auf das Ende der Zeichenfolge*)
		pSubText : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringAppend : UDINT (*Hängt eine Zeichenfolge an einen Text (Ersatz für strcat)*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pTextAppend : STRING[0]; (*Zeiger auf den anzuhängenden Text*)
		pTextOffset : REFERENCE TO UDINT; (*Zeiger auf das momentane Ende*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringCut : STRING[0] (*Schneidet eine Zeichenfolge aus einem Text heraus*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		nCutIndex : UDINT; (*Start-Index der Schneidung*)
		nCutLen : UDINT; (*Länge der Schneidung*)
		pCut : STRING[0]; (*Zeiger auf eine String-Variable, die die herausgeschnittene Zeichenfolge aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringCutFromLastSeparator : UDINT (*Schneidet die Zeichenfolge ab dem letzten Trennzeichen in einem Text ab*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pSeparator : STRING[0]; (*Zeiger auf den zu suchenden Text*)
		pCut : STRING[0]; (*Zeiger auf eine String-Variable, die die herausgeschnittene Zeichenfolge aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringInsert : STRING[0] (*Fügt eine Zeichenfolge in einen Text ein*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		nInsertIndex : UDINT; (*Start-Index der Einfügung*)
		pInsert : STRING[0]; (*Zeiger auf die einzufügende Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringReplace : UDINT (*Ersetzt eine Zeichenfolge in einem Text durch eine andere*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf die zu ersetzende Zeichenfolge*)
		pReplace : STRING[0]; (*Zeiger auf die ersetzende Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringPadLeft : BOOL (*Füllt einen String auf der linken Seite mit einem Füllzeichen auf eine bestimme Länge*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		pFillChar : STRING[0]; (*Zeiger auf das Füllzeichen*)
		nLen : UDINT; (*Länge, auf die gefüllt werden soll*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringPadRight : BOOL (*Füllt einen String auf der rechten Seite mit einem Füllzeichen auf eine bestimme Länge*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		pFillChar : STRING[0]; (*Zeiger auf das Füllzeichen*)
		nLen : UDINT; (*Länge, auf die gefüllt werden soll*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringTrimLeft : BOOL (*Schneidet eine Zeichenfolge auf der linken Seite eines Textes heraus*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		pTrim : STRING[0]; (*Zeiger auf die herauszuschneidene Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringTrimRight : BOOL (*Schneidet eine Zeichenfolge auf der rechten Seite eines Textes heraus*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		pTrim : STRING[0]; (*Zeiger auf die herauszuschneidene Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringSplit : UDINT (*Spaltet einen Text in mehrere Strings aufgrund eines Trennzeichens*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		pSep : STRING[0]; (*Zeiger auf das Trennzeichen*)
		pSplitArray : UDINT; (*Zeiger auf ein String-Array*)
		nArrayIndexMax : UDINT; (*Maximaler Index des String-Arrays*)
		nEntrySize : UDINT; (*Größe eines Strings im String-Array*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringSplitEmpty : UDINT (*Spaltet einen Text in mehrere Strings aufgrund eines Trennzeichens (inkl. Leer-Strings)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		pSep : STRING[0]; (*Zeiger auf das Trennzeichen*)
		pSplitArray : UDINT; (*Zeiger auf ein String-Array*)
		nArrayIndexMax : UDINT; (*Maximaler Index des String-Arrays*)
		nEntrySize : UDINT; (*Maximaler Index des String-Arrays*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringConvertRealFromExp : UINT (*Konvertiert eine exponentielle Notation in einen Real-Wert*)
	VAR_INPUT
		pValue : STRING[0]; (*Zeiger auf den Wert-Text*)
		pResult : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
		nResultSize : UDINT; (*Größe der Ergebnis-Variable*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringConvertRealToExp : UINT (*Konvertiert einen Real-Wert in eine exponentielle Notation*)
	VAR_INPUT
		pValue : STRING[0]; (*Zeiger auf den Wert-Text*)
		pResult : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
		nResultSize : UDINT; (*Größe der Ergebnis-Variable*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringFormatFractionDigits : UINT (*Formatiert einen Real-String-Wert auf Nachkommastellen*)
	VAR_INPUT
		pValue : STRING[0]; (*Zeiger auf den Wert-Text*)
		nValueSize : UDINT; (*Größe der Variable*)
		nFractionsDigits : UINT; (*Anzahl der Nachkomma-Stellen*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringSwap : BOOL (*Dreht die Reihenfolge der Zeichen in einem Text*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den zu drehenden Text*)
		pSwapped : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
		nSwappedSize : UDINT; (*Größe der String-Variablen*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringToUpper : BOOL (*Konvertiert einen String zu Großbuchstaben*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringToLower : BOOL (*Konvertiert einen String zu Kleinbuchstaben*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringIsNumerical : BOOL (*Gibt zurück, ob ein Text eine Nummer enthält*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringIsHex : BOOL (*Gibt zurück, ob ein Text eine hexadezimale Nummer enthält*) (*$GROUP=User*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		bLowCaseAllowed : BOOL; (*1=Kleinbuchstaben (a..f) sind erlaubt*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringCountText : UDINT (*Zählt das Vorkommen einer Zeichenfolge innerhalb eines Texts*) (*$GROUP=User*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : STRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbStringRepeat : UDINT (*Wiederholt eine Zeichenfolge bis zu einer angegebenen Zeichenanzahl*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den ganzen Text*)
		nTextSize : UDINT; (*Größe des Text-Strings*)
		pRepeat : STRING[0]; (*Zeiger auf die zu wiederholende Zeichenfolge*)
		nLen : UDINT; (*Anzahl der Zeichen, bei dem die Wiederholungen enden*)
	END_VAR
END_FUNCTION
(*WcStrings*)

FUNCTION BrbWcCopyStringToWString : UINT (*Kopiert einen STRING auf einen WSTRING*) (*$GROUP=User*)
	VAR_INPUT
		pWcString : WSTRING[0]; (*Zeiger auf WSTRING*)
		pString : STRING[0]; (*Zeiger auf STRING*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcCopyWStringToString : UINT (*Kopiert einen WSTRING auf einen STRING*) (*$GROUP=User*)
	VAR_INPUT
		pString : STRING[0]; (*Zeiger auf STRING*)
		pWcString : WSTRING[0]; (*Zeiger auf WSTRING*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetIndexOf : DINT (*Gibt den ersten Index eines zu suchenden Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetLastIndexOf : DINT (*Gibt den letzten Index eines zu suchenden Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetAdrOf : UDINT (*Gibt die erste Adresse eines zu suchenden Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetLastAdrOf : UDINT (*Gibt die letzte Adresse eines zu suchenden Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringStartsWith : BOOL (*Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge beginnt*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringEndsWith : BOOL (*Gibt zurück, ob ein Text mit einer bestimmten Zeichenfolge endet*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetSubText : UDINT (*Gibt eine Zeichenfolge innerhalb eines Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		nIndex : UDINT; (*Start-Index*)
		nLen : UDINT; (*Länge*)
		pSubText : WSTRING[0]; (*Zeiger auf eine WcString-Variable, die das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetSubTextByLen : UDINT (*Gibt eine Zeichenfolge innerhalb eines Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pStart : WSTRING[0]; (*Zeiger auf den Start der Zeichenfolge*)
		nLen : UDINT; (*Länge*)
		pSubText : WSTRING[0]; (*Zeiger auf eine WcString-Variable, die das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringGetSubTextByAdr : UDINT (*Gibt eine Zeichenfolge innerhalb eines Textes zurück*) (*$GROUP=User*)
	VAR_INPUT
		pStart : WSTRING[0]; (*Zeiger auf den Start der Zeichenfolge*)
		pEnd : WSTRING[0]; (*Zeiger auf das Ende der Zeichenfolge*)
		pSubText : WSTRING[0]; (*Zeiger auf eine WcString-Variable, die das Ergebnis aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringAppend : UDINT (*Hängt eine Zeichenfolge an einen Text (Ersatz für brwcscat)*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pTextAppend : WSTRING[0]; (*Zeiger auf den anzuhängenden Text*)
		pTextOffset : REFERENCE TO UDINT; (*Zeiger auf das momentane Ende*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringCut : UDINT (*Schneidet eine Zeichenfolge aus einem Text heraus*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		nCutIndex : UDINT; (*Start-Index der Schneidung*)
		nCutLen : UDINT; (*Länge der Schneidung*)
		pCut : WSTRING[0]; (*Zeiger auf eine WcString-Variable, die die herausgeschnittene Zeichenfolge aufnimmt*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringInsert : UDINT (*Fügt eine Zeichenfolge in einen Text ein*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		nInsertIndex : UDINT; (*Start-Index der Einfügung*)
		pInsert : WSTRING[0]; (*Zeiger auf die einzufügende Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringReplace : UDINT (*Ersetzt eine Zeichenfolge in einem Text durch eine andere*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf die zu ersetzende Zeichenfolge*)
		pReplace : WSTRING[0]; (*Zeiger auf die ersetzende Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringPadLeft : BOOL (*Füllt einen String auf der linken Seite mit einem Füllzeichen auf eine bestimme Länge*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
		pFillChar : WSTRING[0]; (*Zeiger auf das Füllzeichen*)
		nLen : UDINT; (*Länge, auf die gefüllt werden soll*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringPadRight : BOOL (*Füllt einen String auf der rechten Seite mit einem Füllzeichen auf eine bestimme Länge*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
		pFillChar : WSTRING[0]; (*Zeiger auf das Füllzeichen*)
		nLen : UDINT; (*Länge, auf die gefüllt werden soll*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringTrimLeft : BOOL (*Schneidet eine Zeichenfolge auf der linken Seite eines Textes heraus*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
		pTrim : WSTRING[0]; (*Zeiger auf die herauszuschneidene Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringTrimRight : BOOL (*Schneidet eine Zeichenfolge auf der rechten Seite eines Textes heraus*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
		pTrim : WSTRING[0]; (*Zeiger auf die herauszuschneidene Zeichenfolge*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringToUpper : BOOL (*Konvertiert einen WcString zu Großbuchstaben*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringToLower : BOOL (*Konvertiert einen WcString zu Kleinbuchstaben*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringIsNumerical : BOOL (*Gibt zurück, ob ein Text eine Nummer enthält*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den Text*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringIsHex : BOOL (*Gibt zurück, ob ein Text eine hexadezimale Nummer enthält*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[80]; (*Zeiger auf den Text*)
		bLowCaseAllowed : BOOL; (*1=Kleinbuchstaben (a..f) sind erlaubt*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringCountText : UDINT (*Zählt das Vorkommen einer Zeichenfolge innerhalb eines Texts*) (*$GROUP=User*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		pFind : WSTRING[0]; (*Zeiger auf den zu suchenden Text*)
		nTextLen : UDINT; (*Länge des ganzen Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbWcStringRepeat : UDINT (*Wiederholt eine Zeichenfolge bis zu einer angegebenen Zeichenanzahl*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pText : WSTRING[0]; (*Zeiger auf den ganzen Text*)
		nTextSize : UDINT; (*Größe des Text-Strings*)
		pRepeat : WSTRING[0]; (*Zeiger auf die zu wiederholende Zeichenfolge*)
		nLen : UDINT; (*Anzahl der Zeichen, bei dem die Wiederholungen enden*)
	END_VAR
END_FUNCTION
(*Xml*)

FUNCTION BrbXmlGetTagText : UDINT (*Gibt den Text zwischen zwei Xml-Tags zurück*)
	VAR_INPUT
		pStartTag : STRING[0]; (*Zeiger auf den Start-Tag*)
		pEndTag : STRING[0]; (*Zeiger auf den End-Tag*)
		pStart : UDINT; (*Zeiger auf den Such-Start*)
		pEnd : UDINT; (*Zeiger auf das Such-Ende*)
		pText : STRING[0]; (*Zeiger auf eine String-Variable, die das Ergebnis aufnimmt*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
	END_VAR
END_FUNCTION

FUNCTION BrbXmlGetNextTag : UDINT (*Gibt die Adresse nach einem Xml-Tag zurück*)
	VAR_INPUT
		pTag : STRING[0]; (*Zeiger auf den Tag*)
		pStart : UDINT; (*Zeiger auf den Such-Start*)
		pEnd : UDINT; (*Zeiger auf das Such-Ende*)
	END_VAR
END_FUNCTION
(*Memory*)

FUNCTION BrbMemListClear : DINT (*Löscht die gesamte Liste*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
	END_VAR
END_FUNCTION

FUNCTION BrbMemListIn : DINT (*Fügt einen Eintrag an bestimmter Stelle in der Liste ein*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		nIndex : UDINT; (*Index des Eintrags*)
		pNewEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbMemListOut : DINT (*Holt einen bestimmten Eintrag und löscht ihn aus der Liste*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		nIndex : UDINT; (*Index des Eintrags*)
		pListEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbMemListGetEntry : DINT (*Holt einen bestimmten Eintrag aus der Liste*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		nIndex : UDINT; (*Index des Eintrags*)
		pListEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbFifoIn : DINT (*Fügt einen neuen Eintrag an der letzten Stelle hinzu*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		pNewEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbFifoOut : DINT (*Gibt den Eintrag an erster Stelle zurück und löscht ihn aus der Liste*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		pListEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbLifoIn : DINT (*Fügt einen neuen Eintrag an der letzten Stelle hinzu*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		pNewEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbLifoOut : DINT (*Gibt den Eintrag an letzter Stelle zurück und löscht ihn aus der Liste*)
	VAR_INPUT
		pListManagement : BrbMemListManagement_Typ; (*Zeiger auf die Verwaltungs-Struktur*)
		pListEntry : UDINT; (*Zeiger auf die Daten des Eintrags*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetBitUdint : BOOL (*Liest ein Bit aus einem Udint*)
	VAR_INPUT
		nValue : UDINT; (*Udint-Wert*)
		nBitNumber : UINT; (*Nummer des Bits (0..31)*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetBitUdint : BOOL (*Setzt oder löscht ein Bit in einem Udint*)
	VAR_INPUT
		pValue : REFERENCE TO UDINT; (*Zeiger auf einen Udint-Wert*)
		nBitNumber : UINT; (*Nummer des Bits (0..31)*)
		bBit : BOOL; (*0=Löschen, 1=Setzen*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetBitMaskUdint : BOOL (*Liest eine Bitmaske aus einem Udint*)
	VAR_INPUT
		nValue : UDINT; (*Udint-Wert*)
		nBitMask : UDINT; (*Bit-Maske*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetBitMaskUdint : BOOL (*Setzt oder löscht eine Bitmaske in einem Udint*)
	VAR_INPUT
		pValue : REFERENCE TO UDINT; (*Zeiger auf einen Udint-Wert*)
		nBitMask : UDINT; (*Bit-Maske*)
		bSet : BOOL; (*0=Löschen, 1=Setzen*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetBitUint : BOOL (*Liest ein Bit aus einem Uint*)
	VAR_INPUT
		nValue : UINT; (*Uint-Wert*)
		nBitNumber : UINT; (*Nummer des Bits (0..15)*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetBitUint : BOOL (*Setzt oder löscht ein Bit in einem Uint*)
	VAR_INPUT
		pValue : REFERENCE TO UINT; (*Zeiger auf einen Uint-Wert*)
		nBitNumber : UINT; (*Nummer des Bits (0..15)*)
		bBit : BOOL; (*0=Löschen, 1=Setzen*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetBitMaskUint : BOOL (*Liest eine Bitmaske aus einem Uint*)
	VAR_INPUT
		nValue : UINT; (*Uint-Wert*)
		nBitMask : UINT; (*Bit-Maske*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetBitMaskUint : BOOL (*Setzt oder löscht eine Bitmaske in einem Uint*)
	VAR_INPUT
		pValue : REFERENCE TO UINT; (*Zeiger auf einen Uint-Wert*)
		nBitMask : UINT; (*Bit-Maske*)
		bSet : BOOL; (*0=Löschen, 1=Setzen*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetBitUsint : BOOL (*Liest ein Bit aus einem Usint*)
	VAR_INPUT
		nValue : USINT; (*Usint-Wert*)
		nBitNumber : UINT; (*Nummer des Bits (0..7)*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetBitUsint : BOOL (*Setzt ein Bit in einem Usint*)
	VAR_INPUT
		pValue : REFERENCE TO USINT; (*Zeiger auf einen Usint-Wert*)
		nBitNumber : UINT; (*Nummer des Bits (0..7)*)
		bBit : BOOL; (*0=Löschen, 1=Setzen*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetBitMaskUsint : BOOL (*Liest eine Bitmaske aus einem Usint*)
	VAR_INPUT
		nValue : USINT; (*Usint-Wert*)
		nBitMask : USINT; (*Bit-Maske*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetBitMaskUsint : BOOL (*Setzt oder löscht eine Bitmaske in einem Usint*)
	VAR_INPUT
		pValue : REFERENCE TO USINT; (*Zeiger auf einen Usint-Wert*)
		nBitMask : USINT; (*Bit-Maske*)
		bSet : BOOL; (*0=Löschen, 1=Setzen*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetByteArrayBit : BOOL (*Gibt ein Bit eines bitcodierten Byte-Arrays zurück*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pByteArray : UDINT; (*Zeiger auf das Byte-Array*)
		nIndexMax : UDINT; (*Maximaler Index des Byte-Arrays*)
		nBitNumber : UDINT; (*Nummer der Bits (0..n)*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetByteArrayBit : BOOL (*Setzt ein Bit in einem bitcodierten Byte-Array*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pByteArray : UDINT; (*Zeiger auf das Byte-Array*)
		nIndexMax : UDINT; (*Maximaler Index des Byte-Arrays*)
		nBitNumber : UDINT; (*Nummer der Bits (0..n)*)
		bBit : BOOL; (*Zu setzender Wert*)
	END_VAR
END_FUNCTION

FUNCTION BrbSetByteArrayBits : BOOL (*Setzte Bits in einem bitcodierten Byte-Array*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pByteArray : UDINT; (*Zeiger auf das Byte-Array*)
		nIndexMax : UDINT; (*Maximaler Index des Byte-Arrays*)
		nBitNumberFrom : UDINT; (*Start-Nummer der Bits (0..n)*)
		nBitNumberTo : UDINT; (*End-Nummer der Bits (0..n)*)
		bBit : BOOL; (*Zu setzender Wert für jedes Bit*)
	END_VAR
END_FUNCTION
(*Math*)

FUNCTION BrbAbsReal : REAL (*Gibt den absoluten Wert zurück*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rValue : REAL; (*Wert*)
	END_VAR
END_FUNCTION

FUNCTION BrbAbsLReal : LREAL (*Gibt den absoluten Wert zurück*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rValue : LREAL; (*Wert*)
	END_VAR
END_FUNCTION

FUNCTION BrbIsNearlyZeroReal : BOOL (*Gibt zurück, ob ein REAL-Wert annähernd 0 ist*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rValue : REAL; (*Wert*)
		rTolerance : REAL; (*Grenz-Toleranz*)
	END_VAR
END_FUNCTION

FUNCTION BrbIsNearlyZeroLReal : BOOL (*Gibt zurück, ob ein LREAL-Wert annähernd 0 ist*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rValue : LREAL; (*Wert*)
		rTolerance : LREAL; (*Grenz-Toleranz*)
	END_VAR
END_FUNCTION

FUNCTION BrbIsWithinRangeDint : BOOL (*Gibt zurück, ob sich ein Wert innerhalb eines Bereichs befindet*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		nValue : DINT; (*Wert*)
		nRangeMin : DINT; (*Untere Grenze*)
		nRangeMax : DINT; (*Obere Grenze*)
		nTolerance : DINT; (*Grenz-Toleranz*)
	END_VAR
END_FUNCTION

FUNCTION BrbIsWithinRangeUdint : BOOL (*Gibt zurück, ob sich ein Wert innerhalb eines Bereichs befindet*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		nValue : UDINT; (*Wert*)
		nRangeMin : UDINT; (*Untere Grenze*)
		nRangeMax : UDINT; (*Obere Grenze*)
		nTolerance : UDINT; (*Grenz-Toleranz*)
	END_VAR
END_FUNCTION

FUNCTION BrbIsWithinRangeReal : BOOL (*Gibt zurück, ob sich ein Wert innerhalb eines Bereichs befindet*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rValue : REAL; (*Wert*)
		rRangeMin : REAL; (*Untere Grenze*)
		rRangeMax : REAL; (*Obere Grenze*)
		rTolerance : REAL; (*Grenz-Toleranz*)
	END_VAR
END_FUNCTION

FUNCTION BrbIsWithinRangeLReal : BOOL (*Gibt zurück, ob sich ein Wert innerhalb eines Bereichs befindet*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rValue : LREAL; (*Wert*)
		rRangeMin : LREAL; (*Untere Grenze*)
		rRangeMax : LREAL; (*Obere Grenze*)
		rTolerance : LREAL; (*Grenz-Toleranz*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetAngleRad : REAL (*Rechnet einen Winkel von Grad ins Bogenmass um*)
	VAR_INPUT
		rAngleDeg : REAL; (*Winkel in Grad*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetAngleDeg : REAL (*Rechnet einen Winkel vom Bogenmass in Grad um*)
	VAR_INPUT
		rAngleRad : REAL; (*Winkel im Bogenmass*)
	END_VAR
END_FUNCTION

FUNCTION BrbNormalizeAngleRad : REAL (*Normalisiert einen Bogenmass-Winkel in den Bereich 0..2Pi*)
	VAR_INPUT
		rAngleRad : REAL; (*Winkel im Bogenmass*)
	END_VAR
END_FUNCTION

FUNCTION BrbNormalizeAngleDeg : REAL (*Normalisiert einen Grad-Winkel in den Bereich 0..360°*)
	VAR_INPUT
		rAngleDeg : REAL; (*Winkel in Grad*)
		bKeep360 : USINT; (*Genau 360.0° werden nicht zu 0.0°*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetDistance2d : LREAL (*Gibt die Distanz zwischen zwei Punkten im 2-dimensionalen Raum zurück*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		rX1 : LREAL; (*X-Koordinate des 1. Punkts*)
		rY1 : LREAL; (*Y-Koordinate des 1. Punkts*)
		rX2 : LREAL; (*X-Koordinate des 2. Punkts*)
		rY2 : LREAL; (*Y-Koordinate des 2. Punkts*)
	END_VAR
END_FUNCTION

FUNCTION BrbRoundDint : DINT (*Rundet einen Dint*)
	VAR_INPUT
		nValue : DINT; (*Zu rundendet Wert*)
		eRound : BrbRound_ENUM; (*Rundungstyp*)
		nDigits : USINT; (*Stelle, auf die gerundet wird*)
	END_VAR
END_FUNCTION

FUNCTION BrbScaleAnalogInput : REAL (*Skaliert ein analoges Eingangssignal linear auf einen Einheitswert*) (*$GROUP=User*)
	VAR_INPUT
		nRaw : INT; (*Rohwert*)
		nRawMin : INT; (*Minimaler Rohwert*)
		nRawMax : INT; (*Maximaler Rohwert*)
		rScaledMin : REAL; (*Minimaler skalierter Wert*)
		rScaledMax : REAL; (*Maximaler skalierter Wert*)
		rFactor : REAL; (*Umrechnungs-Faktor*)
		rOffset : REAL; (*Umrechnungs-Offset*)
	END_VAR
END_FUNCTION

FUNCTION BrbScaleAnalogOutput : INT (*Skaliert ein analoges Ausgangssignal von einem Einheitswert*) (*$GROUP=User*)
	VAR_INPUT
		rScaled : REAL; (*Einheitswert*)
		nRawMin : INT; (*Minimaler Rohwert*)
		nRawMax : INT; (*Maximaler Rohwert*)
		rScaledMin : REAL; (*Minimaler skalierter Wert*)
		rScaledMax : REAL; (*Maximaler skalierter Wert*)
		rFactor : REAL; (*Umrechnungs-Faktor*)
		rOffset : REAL; (*Umrechnungs-Offset*)
	END_VAR
END_FUNCTION
(*Random*)

FUNCTION BrbGetRandomPercent : REAL (*Erzeugt eine Zufallszahl zwischen 0.0 und 1.0*)
END_FUNCTION

FUNCTION BrbGetRandomBool : BOOL (*Erzeugt einen boolschen Zufallswert*)
END_FUNCTION

FUNCTION BrbGetRandomUdint : UDINT (*Erzeugt eine Zufallszahl zwischen nMin und nMax*)
	VAR_INPUT
		nMin : UDINT; (*Minimaler Zahlenwert*)
		nMax : UDINT; (*Maximaler Zahlenwert*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetRandomDint : DINT (*Erzeugt eine Zufallszahl zwischen nMin und nMax*)
	VAR_INPUT
		nMin : DINT; (*Minimaler Zahlenwert*)
		nMax : DINT; (*Maximaler Zahlenwert*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetRandomText : UDINT (*Erzeugt einen Zufallstext mit bestimmter Länge (nur Zahlen, Groß- und Kleinbuchstaben)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		nTextLength : UDINT; (*Länge der zu erstellenden Textes*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetRandomString : UDINT (*Erzeugt einen Zufallstext mit bestimmter Länge aus einer Zeichenblockauswahl*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		nTextLength : UDINT; (*Länge der zu erstellenden Textes*)
		eSelection : BrbRandomString_ENUM; (*Auswahl*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetRandomStringExt : UDINT (*Erzeugt einen Zufallstext mit bestimmter Länge aus einer Zeichenauswahl*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pText : STRING[0]; (*Zeiger auf den Text*)
		nTextSize : UDINT; (*Größe der String-Variablen*)
		nTextLength : UDINT; (*Länge der zu erstellenden Textes*)
		pSelection : STRING[0]; (*Zeiger auf den Auswahl-Text*)
	END_VAR
END_FUNCTION
(*Additional*)

FUNCTION BrbCheckIpAddress : BOOL (*Prüft eine Ip-Adresse und berichtigt sie gegebenenfalls*)
	VAR_INPUT
		pIpAddress : STRING[0]; (*Zeiger auf den Ip-Adress-Text*)
	END_VAR
END_FUNCTION

{REDUND_UNREPLICABLE} FUNCTION_BLOCK BrbDebounceInput (*Entprellt einen Eingang*)
	VAR_INPUT
		bInput : {REDUND_UNREPLICABLE} BOOL; (*Zu entprellender Eingang*)
		nDebounceTime : {REDUND_UNREPLICABLE} UDINT; (*Entprell-Zeit [ms]*)
	END_VAR
	VAR_OUTPUT
		bOutput : {REDUND_UNREPLICABLE} BOOL; (*Entprellter Eingang*)
	END_VAR
	VAR
		fbDebounce : {REDUND_UNREPLICABLE} TON; (*Interner Fub*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION BrbGetStructMemberOffset : UINT (*Gibt den Offset eines Struktur-Members zurück*)
	VAR_INPUT
		pStructName : STRING[0]; (*Zeiger auf den Namen der Struktur-Instanz*)
		pMemberName : STRING[0]; (*Zeiger auf dem Namen des Struktur-Elements*)
		pOffset : REFERENCE TO UDINT; (*Zeiger auf den Offset*)
	END_VAR
END_FUNCTION

FUNCTION BrbGetCompilerVersion : UINT (*Gibt die Version des verwendeten GCC-Compilers zurück*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pCompilerVersion : BrbCompilerVersion_TYP; (*Zeiger auf die Version-Struktur*)
	END_VAR
END_FUNCTION
