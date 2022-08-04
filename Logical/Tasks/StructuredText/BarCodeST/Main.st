//********************************************************************
// PROGRAM:  BarRead
// File:     Main.c
// Author:   B&R
// Created:  24.08.2021
// Cycle Time: 10ms
// ********************************************************************
// * Implementation OF PROGRAM BarRead
// * shows bar code reader implementation with code 128 decoding
// ********************************************************************/
    
PROGRAM _INIT
	(* Insert code here *)
	 
   // get the node lists for some usb devices
  UsbManager.Fubs.UsbNodeListGet_Fub.enable := TRUE;
  UsbManager.Fubs.UsbNodeListGet_Fub.pBuffer := ADR(UsbManager.UsbNodeList);
  UsbManager.Fubs.UsbNodeListGet_Fub.bufferSize := SIZEOF(UsbManager.UsbNodeList);
  UsbManager.Fubs.UsbNodeListGet_Fub.filterInterfaceClass := asusb_CLASS_HID;
  UsbManager.Fubs.UsbNodeListGet_Fub.filterInterfaceSubClass := 0;	// for the barcode reader
	
  UsbManager.NodeIdx := 0;
	
  // get the usb device
  UsbManager.Fubs.UsbNodeGet_Fub.enable := TRUE;
  UsbManager.Fubs.UsbNodeGet_Fub.pBuffer := ADR(UsbManager.UsbDevice);
  UsbManager.Fubs.UsbNodeGet_Fub.bufferSize := SIZEOF(UsbManager.UsbDevice);
	
  // open frame driver
  UsbManager.Fubs.FRM_xopen_Fub.enable := TRUE;
  UsbManager.Fubs.FRM_xopen_Fub.mode := 0;
  UsbManager.Fubs.FRM_xopen_Fub.config := 0;
  
  // timer
  UsbManager.Fubs.TON_WaitForInput.PT := WAIT_FOR_DATA;
  
  // init some HID Table
  SetHIDStandardTable_Action;
  
END_PROGRAM


//1. check IF a new usb device is available
//2. if it is comes through the hid filter THEN go on
//3. initialize dvframe:frmxopen()
//4. wait FOR input with dvframe:frm_read()
//5. parse data with code 128
//6. copy the result TO the ResultArray
PROGRAM _CYCLIC

  // cyclic call: usb device detected  - - - START 
  // controls finally the usb reader...
  UsbManager.Fubs.UsbNodeListGet_Fub();
  IF UsbManager.Fubs.UsbNodeListGet_Fub.status <> ERR_FUB_BUSY THEN
    
    IF UsbManager.Fubs.UsbNodeListGet_Fub.status = ERR_OK THEN
    
      // something has changed:
      IF UsbManager.Fubs.UsbNodeListGet_Fub.listNodes <> UsbManager.ListNodes THEN
      
        UsbManager.ListNodes := UsbManager.Fubs.UsbNodeListGet_Fub.listNodes;
				
        UsbManager.Statistic.UsbAttach := UsbManager.Statistic.UsbAttach + 1;
        UsbManager.Steps.Step.Nr := USB_STEPS_SEARCHDEVICE;
    
    
      ELSIF (UsbManager.Fubs.UsbNodeListGet_Fub.status = asusbERR_USB_NOTFOUND) THEN
    
        UsbManager.ListNodes := 0;
        UsbManager.Statistic.UsbDetach := UsbManager.Statistic.UsbDetach + 1;
        IF UsbManager.DeviceData.FrameIdent > 0 THEN
          UsbManager.Steps.Step.Nr := USB_STEPS_SHUTDOWN;	// Verbindung schliessen
        ELSE
          UsbManager.Steps.Step.Nr := USB_STEPS_WAIT; 
        END_IF
      END_IF
     END_IF 

    END_IF
    // cyclic call: usb device detected  - - - STOP

    // - - - StepHandling: done with BrbLib
    IF UsbManager.Steps.StepHandling.Current.bTimeoutElapsed = TRUE THEN    
      UsbManager.Steps.StepHandling.Current.bTimeoutElapsed := FALSE; 
      UsbManager.Steps.Step.Nr := UsbManager.Steps.StepHandling.Current.nTimeoutContinueStep; 
    END_IF
      
  
    // cyclic settings for the step management
    UsbManager.Steps.StepHandling.Current.nStepNr := UsbManager.Steps.Step.Nr;   
    brsstrcpy(ADR(UsbManager.Steps.StepHandling.Current.sStepText), ADR(UsbManager.Steps.Step.Text)); 
    BrbStepHandler(UsbManager.Steps.StepHandling);
      
  // - - - Stephandler
  CASE UsbManager.Steps.Step.Nr OF
  
    // init step management
    USB_STEPS_INIT:
      brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_INIT'));
      UsbManager.Steps.Step.InitDone := TRUE;
      UsbManager.Steps.Step.Nr := USB_STEPS_WAIT;
    
    // wait for commando from usb device detection; just an idle step..
    USB_STEPS_WAIT:
      brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_WAIT'));
      
    // search for device
    USB_STEPS_SEARCHDEVICE:     
    
      brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_SEARCHDEVICE'));
        
      UsbManager.Fubs.UsbNodeGet_Fub.nodeId := UsbManager.UsbNodeList[UsbManager.NodeIdx];
      UsbManager.Fubs.UsbNodeGet_Fub();
        
      IF UsbManager.Fubs.UsbNodeGet_Fub.status <> ERR_FUB_BUSY THEN
        
        IF UsbManager.Fubs.UsbNodeGet_Fub.status = ERR_OK THEN
        
          brsstrcpy(ADR(UsbManager.DeviceData.StringDevice), ADR(UsbManager.UsbDevice.ifName));
          UsbManager.Statistic.UsbFind := UsbManager.Statistic.UsbFind + 1;
              
          UsbManager.Steps.Step.Nr := USB_STEPS_DEVICEOPEN;
        
        ELSE
        
          UsbManager.Err.ErrStep := USB_STEPS_SEARCHDEVICE;
          UsbManager.Err.Status := UsbManager.Fubs.UsbNodeGet_Fub.status;
                
          UsbManager.Steps.Step.Nr := USB_STEPS_ERR;
        END_IF
      END_IF
      
      // initialize found usb Device
      USB_STEPS_DEVICEOPEN:
        brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_DEVICEOPEN'));
      
        UsbManager.Fubs.FRM_xopen_Fub.device := ADR(UsbManager.DeviceData.StringDevice);
        UsbManager.Fubs.FRM_xopen_Fub();
      
        IF UsbManager.Fubs.FRM_xopen_Fub.status <> ERR_FUB_BUSY THEN
        
          IF UsbManager.Fubs.FRM_xopen_Fub.status = ERR_OK THEN
          
            UsbManager.DeviceData.FrameIdent := UsbManager.Fubs.FRM_xopen_Fub.ident;
            UsbManager.Statistic.FrmOpen := UsbManager.Statistic.FrmOpen + 1;
     
            UsbManager.Steps.Step.Nr := USB_STEPS_READ;
          
          ELSE
        
            UsbManager.Err.ErrStep := USB_STEPS_DEVICEOPEN;
            UsbManager.Err.Status := UsbManager.Fubs.UsbNodeGet_Fub.status;
                
            UsbManager.Steps.Step.Nr := USB_STEPS_ERR;
          END_IF;
        END_IF;
        
    // read data stream from usb device:
    USB_STEPS_READ:
      brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_READ'));
        
      UsbManager.Fubs.FRM_read_Fub.enable := TRUE;
      UsbManager.Fubs.FRM_read_Fub.ident := UsbManager.DeviceData.FrameIdent;
      UsbManager.Fubs.FRM_read_Fub();
          
      IF UsbManager.Fubs.FRM_read_Fub.status <> ERR_FUB_BUSY THEN
        
        // data received
        IF UsbManager.Fubs.FRM_read_Fub.status = ERR_OK THEN
          
          UsbManager.Statistic.FrmRead := UsbManager.Statistic.FrmRead + 1;
          UsbManager.Fubs.TON_WaitForInput.IN := TRUE; // start timeout for data traffic
          
          IF UsbManager.ReadIdx < SIZEOF(ReadData) THEN
            
            // copy new data to the receive buffer
            brsmemcpy(ADR(ReadData[UsbManager.ReadIdx]), UsbManager.Fubs.FRM_read_Fub.buffer, UsbManager.Fubs.FRM_read_Fub.buflng);
            UsbManager.ReadIdx := UsbManager.ReadIdx + UsbManager.Fubs.FRM_read_Fub.buflng;
              
            // loop through the characters and search for 16#28 and 16#A8 = end of data stream
            IF ReadData[UsbManager.ReadIdx-2] = HID_RETURN THEN          
              UsbManager.DeviceData.WaitTranslate := 0;
              UsbManager.Steps.Step.Nr := USB_STEPS_WAIT_TD;
            END_IF
          END_IF // IF UsbManager.ReadIdx < SIZEOF(ReadData) THEN
   
          // after reading the data: release reading buffer
          UsbManager.Fubs.FRM_rbuf_Fub.enable := TRUE;
          UsbManager.Fubs.FRM_rbuf_Fub.ident := UsbManager.DeviceData.FrameIdent;
          UsbManager.Fubs.FRM_rbuf_Fub.buffer := UsbManager.Fubs.FRM_read_Fub.buffer;
            
          UsbManager.Fubs.FRM_rbuf_Fub();
          IF UsbManager.Fubs.FRM_rbuf_Fub.status <> ERR_OK THEN
            
            UsbManager.Err.ErrStep := USB_STEPS_READ;
            UsbManager.Err.Status := UsbManager.Fubs.FRM_rbuf_Fub.status;
              
            UsbManager.Steps.Step.Nr := USB_STEPS_ERR;
          
          ELSIF (UsbManager.Fubs.FRM_read_Fub.status = frmERR_NOINPUT) THEN
          
            UsbManager.Statistic.FrmReadNoData := UsbManager.Statistic.FrmReadNoData + 1; // statistic
          END_IF //IF UsbManager.Fubs.FRM_rbuf_Fub.status <> ERR_OK THEN
        END_IF // IF UsbManager.Fubs.FRM_read_Fub.status = ERR_OK THEN
     END_IF // IF UsbManager.Fubs.FRM_read_Fub.status <> ERR_FUB_BUSY THEN
     
     // check timeout
     UsbManager.Fubs.TON_WaitForInput();
     IF UsbManager.Fubs.TON_WaitForInput.Q = TRUE THEN
        // timeout: no data => Clear the existing data
        UsbManager.Fubs.TON_WaitForInput.IN := FALSE; 
        UsbManager.Fubs.TON_WaitForInput();
        
        UsbManager.ReadIdx := 0;
        brsmemset(ADR(ReadData), 0, SIZEOF(ReadData));
        UsbManager.Statistic.FrmReadReset := UsbManager.Statistic.FrmReadReset + 1; 
      END_IF
      
        
   USB_STEPS_WAIT_TD: 
      brsstrcpy( ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_WAIT_TD'));
        
      UsbManager.DeviceData.WaitTranslate := UsbManager.DeviceData.WaitTranslate + 1;
      IF UsbManager.DeviceData.WaitTranslate > 10 THEN
  
        UsbManager.Steps.Step.Nr := USB_STEPS_TRANSLATE_DATA;
      END_IF
    
    
      USB_STEPS_TRANSLATE_DATA:
        brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_TRANSLATE_DATA'));
        
        // parse the raw data from the barcode scanner:
        // translate the hid codes to the ascii codes
        // at the moment some "standard" table is configured (=> Documentation/USB_HID_Table.pdf)
        //UsbManager.Status.ParseResult = ParseScannerCode((AsciiHidTable_Type*) &AsciiHidTableStandard, (USINT*) &ReadData, UsbManager.ReadIdx,(AsciiHidInternal_Type*) &AsciiHidInternal);
        ParseScannerCode_ACTION;
      // change it!
      
        // some data available
        IF UsbManager.Status.ParseResult <> 0 THEN
        
          UsbManager.Statistic.TranslateOk := UsbManager.Statistic.TranslateOk + 1;
          
          // copy some results for the watch window
          UsbManager.Steps.Step.Nr := USB_STEPS_COPY_DATA;
          
        ELSE
        
          UsbManager.Statistic.TranslateNok := UsbManager.Statistic.TranslateNok + 1;
          UsbManager.Steps.Step.Nr := USB_STEPS_RESET_DATA;
        END_IF
    
      // clear some data (arrays, index,...)
      USB_STEPS_RESET_DATA:
        brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_RESET_DATA'));
        
        // reset some result vars - just for the watch window:
        brsmemset(ADR(ResultData), 0, SIZEOF(ResultData));
        brsmemset(ADR(ReadData), 0, SIZEOF(ReadData));
        
        //internal reset
        UsbManager.DeviceData.WaitTranslate := 0;
        UsbManager.ReadIdx := 0;
          
        // go back and wait for new data
        UsbManager.Steps.Step.Nr := USB_STEPS_READ;
      
      // copy the translated data to some output variable
      USB_STEPS_COPY_DATA:
        brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_COPY_DATA'));
        
        // copy result - just for the watch window:
        brsmemset(ADR(ResultData), 0, SIZEOF(ResultData));
        brsmemcpy(ADR(ResultData), ADR(AsciiHidInternal.Result), SIZEOF(AsciiHidInternal.Result));
        brsmemset(ADR(ReadData), 0, SIZEOF(ReadData));
        
        //internal reset
        UsbManager.DeviceData.WaitTranslate := 0;
        UsbManager.ReadIdx := 0;
          
        // go back and wait for new data
      UsbManager.Steps.Step.Nr := USB_STEPS_READ;
    
      // close the open dvframe connection 
      USB_STEPS_SHUTDOWN:
        brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_SHUTDOWN'));
        
        UsbManager.Fubs.FRM_close_Fub.enable := TRUE;
        UsbManager.Fubs.FRM_close_Fub.ident := UsbManager.DeviceData.FrameIdent;
        UsbManager.Fubs.FRM_close_Fub();
        
        IF (UsbManager.Fubs.FRM_close_Fub.status <> ERR_FUB_BUSY) THEN
          IF (UsbManager.Fubs.FRM_close_Fub.status = ERR_OK) THEN
          
            UsbManager.Statistic.FrmClose := UsbManager.Statistic.FrmClose + 1;
            UsbManager.DeviceData.FrameIdent := 0;
            UsbManager.Steps.Step.Nr := USB_STEPS_WAIT;
          
          ELSE
          
            UsbManager.Steps.Step.Nr := USB_STEPS_ERR;
          END_IF
        END_IF
      
      // error step: at the moment just a placeholder
      USB_STEPS_ERR:
        
        brsstrcpy(ADR(UsbManager.Steps.Step.Text), ADR('USB_STEPS_ERR'));
        
        // back to start
        UsbManager.Steps.Step.Nr := USB_STEPS_WAIT;

  END_CASE;   
  
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

