
TYPE
	USB_STEPS_enum : 
		(
		USB_STEPS_INIT, (*Brb Lib Init (set InitDone = TRUE)*)
		USB_STEPS_WAIT, (*wait for usb barcode reader*)
		USB_STEPS_SEARCHDEVICE, (*search for the right device*)
		USB_STEPS_DEVICEOPEN, (*open device*)
		USB_STEPS_READ, (*read data from usb stick*)
		USB_STEPS_TRANSLATE_DATA, (*translate data *)
		USB_STEPS_RESET_DATA, (*reset result data*)
		USB_STEPS_COPY_DATA, (*copy result data*)
		USB_STEPS_WAIT_TD, (*wait for translate data*)
		USB_STEPS_SHUTDOWN, (*close usb and all devices*)
		USB_STEPS_ERR (*error step*)
		);
	UsbReaderStatistic_Type : 	STRUCT 
		UsbAttach : USINT; (*usb device gesteckt*)
		UsbDetach : USINT; (*usb device abgezogen*)
		UsbFind : USINT; (*usb device gefunden*)
		FrmOpen : USINT; (*device intialisiert*)
		FrmRead : UDINT; (*usb barcode reader - Daten*)
		FrmReadNoData : UDINT; (*usb barcode reader - keine Daten*)
		FrmClose : USINT; (*usb device geschlossen*)
		TranslateOk : USINT; (*translate hid to ascii*)
		TranslateNok : USINT; (*translate hid to ascii; no data?*)
	END_STRUCT;
	UsbError_Type : 	STRUCT 
		Status : UINT; (*error number*)
		ErrStep : USB_STEPS_enum; (*step which triggered the error*)
	END_STRUCT;
	UsbReader_Type : 	STRUCT  (*Data for managing the usb stick*)
		Steps : UsbBarCodeSteps_Type; (*actual step*)
		DeviceData : UsbReaderDeviceData_Type; (*device information*)
		UsbNodeList : ARRAY[0..5]OF UDINT; (*Liste mit gefundenen USB Knoten*)
		ListNodes : UDINT; (*Anzahl der Knoten in Liste*)
		NodeIdx : USINT; (*Ausgewähltes USB Device; hier wird immer der erste Barcode-Scanner verwendet*)
		ReadIdx : UINT; (*Lese Index*)
		UsbDevice : usbNode_typ; (*Informationen um das Device zu öffnen*)
		Statistic : UsbReaderStatistic_Type; (*statistic data*)
		Err : UsbError_Type; (*error step*)
		Fubs : UsbReaderFubs_Type;
		Status : UsbReaderStatus_Type; (*some additional status for the reader*)
	END_STRUCT;
	UsbReaderStatus_Type : 	STRUCT 
		ParseResult : UINT;
	END_STRUCT;
	UsbReaderDeviceData_Type : 	STRUCT 
		FrameIdent : UDINT; (*Ident vom Frame Open FUB*)
		StringDevice : STRING[80]; (*kompletter Anmeldungsstring*)
		WaitTranslate : UINT; (*tranlate usb data to string*)
	END_STRUCT;
	UsbReaderFubs_Type : 	STRUCT 
		FRM_xopen_Fub : FRM_xopen; (*open interface*)
		FRM_read_Fub : FRM_read; (*read data from interface*)
		FRM_rbuf_Fub : FRM_rbuf; (*release data*)
		FRM_close_Fub : FRM_close; (*close interface*)
		UsbNodeGet_Fub : UsbNodeGet; (*get information from usb device*)
		UsbNodeListGet_Fub : UsbNodeListGet; (*get list of available devices*)
	END_STRUCT;
	UsbBarCodeStep_Type : 	STRUCT  (*brb step handling TemplateStep_typ*)
		Text : STRING[nBRB_STEP_TEXT_CHAR_MAX];
		Nr : USB_STEPS_enum;
		InitDone : BOOL;
	END_STRUCT;
	UsbBarCodeSteps_Type : 	STRUCT  (*main step management*)
		Step : UsbBarCodeStep_Type;
		StepHandling : BrbStepHandling_TYP;
	END_STRUCT;
	AsciiHid_Type : 	STRUCT 
		HidCode : USINT; (*from the scanner*)
		SmallChar : USINT; (*character if sign is set to small (=default)*)
		LargeChar : USINT; (*character if sign is set to large*)
	END_STRUCT;
	AsciiHidTable_Type : 	STRUCT 
		Sign : STRING[20]; (*some name for table*)
		Codes : ARRAY[0..39]OF AsciiHid_Type; (*Codes*)
	END_STRUCT;
	ASCII_HID_STATE_enum : 
		(
		ASCII_HID_STATE_UNCHANGED, (*default: nothing changed from last one*)
		ASCII_HID_STATE_LOWER,
		ASCII_HID_STATE_HIGHER
		);
	AsciiHidInternal_Type : 	STRUCT 
		HighLow : ARRAY[0..HID_ARR_LEN]OF ASCII_HID_STATE_enum; (*detects if the characters are lower or higher*)
		FoundLast : ASCII_HID_STATE_enum; (*found last sign (low, high)*)
		i : UINT; (*internal variable*)
		AsciiCnt : USINT; (*ascii count*)
		ResultChar : USINT;
		Result : ARRAY[0..HID_ARR_LEN_RESULT]OF USINT; (*result from barcode scanner*)
	END_STRUCT;
END_TYPE
