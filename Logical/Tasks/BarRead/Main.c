/********************************************************************
	* Program:  BarRead
	* File:     Main.c
	* Author:   B&R
	* Created:  24.08.2021
	* Cycle Time: 10ms
********************************************************************
* Implementation of program BarRead
* shows bar code reader implementation with code 128 decoding
********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE  1
#endif

void SetHIDStandardTable(AsciiHidTable_Type* pTable);
UINT ParseScannerCode(AsciiHidTable_Type* pTable, USINT* pStartData, USINT DataLen, AsciiHidInternal_Type* pHidInternal);
UINT GetAsciiCodeFromTable(AsciiHidTable_Type* pTable, USINT CheckForCode, ASCII_HID_STATE_enum LowHighValue,  USINT* pResult);


// Init part of the task
void _INIT ProgramInit(void)
{
  // get the node lists for some usb devices
  UsbManager.Fubs.UsbNodeListGet_Fub.enable = TRUE;
  UsbManager.Fubs.UsbNodeListGet_Fub.pBuffer = (UDINT) &UsbManager.UsbNodeList;
  UsbManager.Fubs.UsbNodeListGet_Fub.bufferSize = sizeof(UsbManager.UsbNodeList);
  UsbManager.Fubs.UsbNodeListGet_Fub.filterInterfaceClass = asusb_CLASS_HID;
  UsbManager.Fubs.UsbNodeListGet_Fub.filterInterfaceSubClass = 0;	// for the barcode reader
	
  UsbManager.NodeIdx = 0;
	
  // get the usb device
  UsbManager.Fubs.UsbNodeGet_Fub.enable = TRUE;
  UsbManager.Fubs.UsbNodeGet_Fub.pBuffer = (UDINT) &UsbManager.UsbDevice;
  UsbManager.Fubs.UsbNodeGet_Fub.bufferSize = sizeof(UsbManager.UsbDevice);
	
  // open frame driver
  UsbManager.Fubs.FRM_xopen_Fub.enable = TRUE;
  UsbManager.Fubs.FRM_xopen_Fub.mode = 0;
  UsbManager.Fubs.FRM_xopen_Fub.config = 0;
  
  // init some HID Table
  SetHIDStandardTable((AsciiHidTable_Type*) &AsciiHidTableStandard);
  
}


/*
  1. check if a new usb device is available
  2. if it is comes through the hid filter then go on
  3. initialize dvframe:frmxopen()
  4. wait for input with dvframe:frm_read()
  5. parse data with code 128
  6. copy the result to the ResultArray
*/
void _CYCLIC ProgramCyclic(void)
{
  // cyclic call: usb device detected  - - - START 
  // controls finally the usb reader...
  UsbNodeListGet(&UsbManager.Fubs.UsbNodeListGet_Fub);
  
  if (UsbManager.Fubs.UsbNodeListGet_Fub.status != ERR_FUB_BUSY)
  {
    if (UsbManager.Fubs.UsbNodeListGet_Fub.status == ERR_OK)
    {
      // something has changed:
      if (UsbManager.Fubs.UsbNodeListGet_Fub.listNodes != UsbManager.ListNodes)
      {
        UsbManager.ListNodes = UsbManager.Fubs.UsbNodeListGet_Fub.listNodes;
				
        UsbManager.Statistic.UsbAttach++;
        UsbManager.Steps.Step.Nr = USB_STEPS_SEARCHDEVICE;
      }
    }
    else if (UsbManager.Fubs.UsbNodeListGet_Fub.status == asusbERR_USB_NOTFOUND)
    {
      UsbManager.ListNodes = 0;
      UsbManager.Statistic.UsbDetach++;
      if (UsbManager.DeviceData.FrameIdent > 0)
      {
        UsbManager.Steps.Step.Nr = USB_STEPS_SHUTDOWN;	// Verbindung schliessen
      }
      else
      {
        UsbManager.Steps.Step.Nr = USB_STEPS_WAIT; 
      }
    }
  }
  // cyclic call: usb device detected  - - - STOP
  
  // - - - StepHandling: done with BrbLib
  if (UsbManager.Steps.StepHandling.Current.bTimeoutElapsed == TRUE) 
  { 
    UsbManager.Steps.StepHandling.Current.bTimeoutElapsed = FALSE; 
    UsbManager.Steps.Step.Nr = UsbManager.Steps.StepHandling.Current.nTimeoutContinueStep; 
  } 
  
  // cyclic settings for the step management
  UsbManager.Steps.StepHandling.Current.nStepNr = (DINT)UsbManager.Steps.Step.Nr;   
  brsstrcpy((UDINT) &UsbManager.Steps.StepHandling.Current.sStepText, (UDINT) &UsbManager.Steps.Step.Text); 
  BrbStepHandler(&UsbManager.Steps.StepHandling);
  
  // - - - Stephandler
  switch (UsbManager.Steps.Step.Nr)
  {
    // init step management
    case USB_STEPS_INIT:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_INIT");
        
        UsbManager.Steps.Step.InitDone = TRUE;
        UsbManager.Steps.Step.Nr = USB_STEPS_WAIT;
        break;
      }
    
    // wait for commando from usb device detection; just an idle step..
    case USB_STEPS_WAIT:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_WAIT");
        
        break;
      }
    
    // search for device
    case USB_STEPS_SEARCHDEVICE:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_SEARCHDEVICE");
        
        UsbManager.Fubs.UsbNodeGet_Fub.nodeId = UsbManager.UsbNodeList[UsbManager.NodeIdx];
        UsbNodeGet(&UsbManager.Fubs.UsbNodeGet_Fub);
        
        if (UsbManager.Fubs.UsbNodeGet_Fub.status != ERR_FUB_BUSY)
        {
          if (UsbManager.Fubs.UsbNodeGet_Fub.status == ERR_OK)
          {
            brsstrcpy((UDINT) &UsbManager.DeviceData.StringDevice, (UDINT) &UsbManager.UsbDevice.ifName);
            UsbManager.Statistic.UsbFind++;
            
            UsbManager.Steps.Step.Nr = USB_STEPS_DEVICEOPEN;
          }
          else
          {
            UsbManager.Err.ErrStep = USB_STEPS_SEARCHDEVICE;
            UsbManager.Err.Status = UsbManager.Fubs.UsbNodeGet_Fub.status;
              
            UsbManager.Steps.Step.Nr = USB_STEPS_ERR;
          }
        }
        
        break;
      }
    
    // initialize found usb Device
    case USB_STEPS_DEVICEOPEN:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_DEVICEOPEN");
        
        UsbManager.Fubs.FRM_xopen_Fub.device = (UDINT) &UsbManager.DeviceData.StringDevice;
        FRM_xopen(&UsbManager.Fubs.FRM_xopen_Fub);
        
        if (UsbManager.Fubs.FRM_xopen_Fub.status != ERR_FUB_BUSY)
        {
          if (UsbManager.Fubs.FRM_xopen_Fub.status == ERR_OK)
          {
            UsbManager.DeviceData.FrameIdent = UsbManager.Fubs.FRM_xopen_Fub.ident;
            UsbManager.Statistic.FrmOpen++;
   
            UsbManager.Steps.Step.Nr = USB_STEPS_READ;
          }
          else
          {
            UsbManager.Err.ErrStep = USB_STEPS_DEVICEOPEN;
            UsbManager.Err.Status = UsbManager.Fubs.UsbNodeGet_Fub.status;
              
            UsbManager.Steps.Step.Nr = USB_STEPS_ERR;
          }
        }
        break;
      }
    
    // read data stream from usb device:
    case USB_STEPS_READ:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_READ");
        
        UsbManager.Fubs.FRM_read_Fub.enable = TRUE;
        UsbManager.Fubs.FRM_read_Fub.ident = UsbManager.DeviceData.FrameIdent;
        FRM_read(&UsbManager.Fubs.FRM_read_Fub);
          
        if (UsbManager.Fubs.FRM_read_Fub.status != ERR_FUB_BUSY)
        {
          // data received
          if (UsbManager.Fubs.FRM_read_Fub.status == ERR_OK)
          {
            UsbManager.Statistic.FrmRead++;
            if (UsbManager.ReadIdx < sizeof(ReadData))
            {
              // copy new data to the receive buffer
              brsmemcpy((UDINT) &ReadData[UsbManager.ReadIdx], UsbManager.Fubs.FRM_read_Fub.buffer, UsbManager.Fubs.FRM_read_Fub.buflng);
              UsbManager.ReadIdx = UsbManager.ReadIdx + UsbManager.Fubs.FRM_read_Fub.buflng;
              
              // loop through the characters and search for 16#28 and 16#A8 = end of data stream
              if (ReadData[UsbManager.ReadIdx-2] == HID_RETURN)
              {
                UsbManager.DeviceData.WaitTranslate = 0;
                UsbManager.Steps.Step.Nr = USB_STEPS_WAIT_TD;
              }
            }
   
            // after reading the data: release reading buffer
            UsbManager.Fubs.FRM_rbuf_Fub.enable = TRUE;
            UsbManager.Fubs.FRM_rbuf_Fub.ident = UsbManager.DeviceData.FrameIdent;
            UsbManager.Fubs.FRM_rbuf_Fub.buffer = UsbManager.Fubs.FRM_read_Fub.buffer;
            
            FRM_rbuf(&UsbManager.Fubs.FRM_rbuf_Fub);
            if (UsbManager.Fubs.FRM_rbuf_Fub.status != ERR_OK)
            {
              UsbManager.Err.ErrStep = USB_STEPS_READ;
              UsbManager.Err.Status = UsbManager.Fubs.FRM_rbuf_Fub.status;
              
              UsbManager.Steps.Step.Nr = USB_STEPS_ERR;
            }
          }
          else if (UsbManager.Fubs.FRM_read_Fub.status == frmERR_NOINPUT)
          {
            UsbManager.Statistic.FrmReadNoData = UsbManager.Statistic.FrmReadNoData + 1; // statistic
          }
        }
        
        break;
      }
    
    case USB_STEPS_WAIT_TD: 
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_WAIT_TD");
        
        UsbManager.DeviceData.WaitTranslate++;
        if (UsbManager.DeviceData.WaitTranslate > 10)
        {
          UsbManager.Steps.Step.Nr = USB_STEPS_TRANSLATE_DATA;
        }
        break;
      }
    
    
    case USB_STEPS_TRANSLATE_DATA:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_TRANSLATE_DATA");
        
        // parse the raw data from the barcode scanner:
        // translate the hid codes to the ascii codes
        // at the moment some "standard" table is configured (=> Documentation/USB_HID_Table.pdf)
        UsbManager.Status.ParseResult = ParseScannerCode((AsciiHidTable_Type*) &AsciiHidTableStandard, (USINT*) &ReadData, UsbManager.ReadIdx,(AsciiHidInternal_Type*) &AsciiHidInternal);

        // some data available
        if (UsbManager.Status.ParseResult != 0)
        {
          UsbManager.Statistic.TranslateOk++;
        
          // copy some results for the watch window
          UsbManager.Steps.Step.Nr = USB_STEPS_COPY_DATA;
        }
        else
        {
          UsbManager.Statistic.TranslateNok++;
          UsbManager.Steps.Step.Nr = USB_STEPS_RESET_DATA;
        }
        break;
      }
    
    // clear some data (arrays, index,...)
    case USB_STEPS_RESET_DATA:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_RESET_DATA");
        
        // reset some result vars - just for the watch window:
        brsmemset((UDINT) &ResultData, 0, sizeof(ResultData));
        brsmemset((UDINT) &ReadData, 0, sizeof(ReadData));
        
        //internal reset
        UsbManager.DeviceData.WaitTranslate = 0;
        UsbManager.ReadIdx = 0;
          
        // go back and wait for new data
        UsbManager.Steps.Step.Nr = USB_STEPS_READ;
        break;
      }
    
    // copy the translated data to some output variable
    case USB_STEPS_COPY_DATA:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_COPY_DATA");
        
        // copy result - just for the watch window:
        brsmemset((UDINT) &ResultData, 0, sizeof(ResultData));
        brsmemcpy((UDINT) &ResultData, (UDINT) &AsciiHidInternal.Result, sizeof(ResultData));
        brsmemset((UDINT) &ReadData, 0, sizeof(ReadData));
        
        //internal reset
        UsbManager.DeviceData.WaitTranslate = 0;
        UsbManager.ReadIdx = 0;
          
        // go back and wait for new data
        UsbManager.Steps.Step.Nr = USB_STEPS_READ;
        break;
      }
    
    // close the open dvframe connection 
    case USB_STEPS_SHUTDOWN:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_SHUTDOWN");
        
        UsbManager.Fubs.FRM_close_Fub.enable = TRUE;
        UsbManager.Fubs.FRM_close_Fub.ident = UsbManager.DeviceData.FrameIdent;
        FRM_close(&UsbManager.Fubs.FRM_close_Fub);
        
        if (UsbManager.Fubs.FRM_close_Fub.status != ERR_FUB_BUSY)
        {
          if (UsbManager.Fubs.FRM_close_Fub.status == ERR_OK)
          {
            UsbManager.Statistic.FrmClose++;
            UsbManager.DeviceData.FrameIdent = 0;
            UsbManager.Steps.Step.Nr = USB_STEPS_WAIT;
          }
          else
          {
            UsbManager.Steps.Step.Nr = USB_STEPS_ERR;
          }
        }
        break;
      }
    
    // error step: at the moment 
    case USB_STEPS_ERR:
      {
        brsstrcpy((UDINT) &UsbManager.Steps.Step.Text, (UDINT) "USB_STEPS_ERR");
        
        // back to start
        UsbManager.Steps.Step.Nr = USB_STEPS_WAIT;
        
        break;
      }
  }
  
  
}


// Exit part of the task
void _EXIT ProgramExit(void)
{
  // nothing...
}


// set the HID Codes and the corresponding Ascii codes:
// the codes are from file "Documentation/USB_HID_Table.pdf"
// => the HID Code comes from the reader
// => depending on the code it is translated to the upper or lower case sign
void SetHIDStandardTable(AsciiHidTable_Type* pTable)
{
  brsstrcpy((UDINT) &pTable->Sign,  (UDINT) "Standard");
  
  // set codes: numbers 0 - 9 and special characters
  pTable->Codes[0].HidCode = 0x27;  //0
  pTable->Codes[0].SmallChar = 48;
  pTable->Codes[0].LargeChar = 41;
  
  pTable->Codes[1].HidCode  = 0x1E; //1
  pTable->Codes[1].SmallChar = 49;
  pTable->Codes[1].LargeChar = 33;
  
  pTable->Codes[2].HidCode  = 0x1F; //2
  pTable->Codes[2].SmallChar = 50;
  pTable->Codes[2].LargeChar = 64;
  
  pTable->Codes[3].HidCode = 0x20; //3
  pTable->Codes[3].SmallChar = 51;
  pTable->Codes[3].LargeChar = 35;
  
  pTable->Codes[4].HidCode = 0x21; //4
  pTable->Codes[4].SmallChar = 52;
  pTable->Codes[4].LargeChar = 36;
  
  pTable->Codes[5].HidCode = 0x22; //5
  pTable->Codes[5].SmallChar = 53;
  pTable->Codes[5].LargeChar = 36;
  
  pTable->Codes[6].HidCode = 0x23; //6
  pTable->Codes[6].SmallChar = 54;
  pTable->Codes[6].LargeChar = 37;
  
  pTable->Codes[7].HidCode = 0x24; //7
  pTable->Codes[7].SmallChar = 55;
  pTable->Codes[7].LargeChar = 94;
  
  pTable->Codes[8].HidCode = 0x25; //8
  pTable->Codes[8].SmallChar = 56;
  pTable->Codes[8].LargeChar = 42;
  
  pTable->Codes[9].HidCode = 0x26; //9
  pTable->Codes[9].SmallChar = 57;
  pTable->Codes[9].LargeChar = 40;
  
  //letters: A-Z, a-z
  pTable->Codes[10].HidCode = 0x04; //a
  pTable->Codes[10].SmallChar = 65;
  pTable->Codes[10].LargeChar = 97;
  
  pTable->Codes[11].HidCode = 0x05; //b
  pTable->Codes[11].SmallChar = 66;
  pTable->Codes[11].LargeChar = 98;
  
  pTable->Codes[12].HidCode = 0x06; // c
  pTable->Codes[12].SmallChar = 67;
  pTable->Codes[12].LargeChar = 99;
  
  pTable->Codes[13].HidCode = 0x07; //d
  pTable->Codes[13].SmallChar = 68;
  pTable->Codes[13].LargeChar = 101;
  
  pTable->Codes[14].HidCode = 0x08; //e
  pTable->Codes[14].SmallChar = 69;
  pTable->Codes[14].LargeChar = 102;
  
  pTable->Codes[15].HidCode = 0x09; //f
  pTable->Codes[15].SmallChar = 70;
  pTable->Codes[15].LargeChar = 103;
  
  pTable->Codes[16].HidCode = 0x0A; // g
  pTable->Codes[16].SmallChar = 71;
  pTable->Codes[16].LargeChar = 104;

  pTable->Codes[17].HidCode = 0x0B; // h
  pTable->Codes[17].SmallChar = 72;
  pTable->Codes[17].LargeChar = 105;
  
  pTable->Codes[18].HidCode = 0x0C; // i
  pTable->Codes[18].SmallChar = 73;
  pTable->Codes[18].LargeChar = 106;
  
  pTable->Codes[19].HidCode = 0x0D; // j
  pTable->Codes[19].SmallChar = 74;
  pTable->Codes[19].LargeChar = 107;
  
  pTable->Codes[20].HidCode = 0x0E; // k
  pTable->Codes[20].SmallChar = 75;
  pTable->Codes[20].LargeChar = 108;
  
  pTable->Codes[21].HidCode = 0x0F; // l
  pTable->Codes[21].SmallChar = 76;
  pTable->Codes[21].LargeChar = 109;
  
  pTable->Codes[22].HidCode = 0x10; // m
  pTable->Codes[22].SmallChar = 77;
  pTable->Codes[22].LargeChar = 110;
  
  pTable->Codes[23].HidCode = 0x11; // n
  pTable->Codes[23].SmallChar = 78;
  pTable->Codes[23].LargeChar = 111;
  
  pTable->Codes[24].HidCode = 0x12; // o
  pTable->Codes[24].SmallChar = 79;
  pTable->Codes[24].LargeChar = 112;
  
  pTable->Codes[25].HidCode = 0x13; // p
  pTable->Codes[25].SmallChar = 80;
  pTable->Codes[25].LargeChar = 113;

  pTable->Codes[26].HidCode = 0x14; // q
  pTable->Codes[26].SmallChar = 81;
  pTable->Codes[26].LargeChar = 114;

  pTable->Codes[27].HidCode = 0x15; // r
  pTable->Codes[27].SmallChar = 82;
  pTable->Codes[27].LargeChar = 115;
  
  pTable->Codes[28].HidCode = 0x16; // s
  pTable->Codes[28].SmallChar = 83;
  pTable->Codes[28].LargeChar = 116;
  
  pTable->Codes[29].HidCode = 0x17; // t
  pTable->Codes[29].SmallChar = 84;
  pTable->Codes[29].LargeChar = 117;
  
  pTable->Codes[30].HidCode = 0x18; // u
  pTable->Codes[30].SmallChar = 85;
  pTable->Codes[30].LargeChar = 118;
  
  pTable->Codes[31].HidCode = 0x19; // v
  pTable->Codes[31].SmallChar = 86;
  pTable->Codes[31].LargeChar = 119;
  
  pTable->Codes[32].HidCode = 0x1A; // w
  pTable->Codes[32].SmallChar = 87;
  pTable->Codes[32].LargeChar = 120;
  
  pTable->Codes[33].HidCode = 0x1B; // x
  pTable->Codes[33].SmallChar = 88;
  pTable->Codes[33].LargeChar = 121;
  
  pTable->Codes[34].HidCode = 0x1C; // y
  pTable->Codes[34].SmallChar = 89;
  pTable->Codes[34].LargeChar = 122;
  
  pTable->Codes[35].HidCode = 0x1D; // z
  pTable->Codes[35].SmallChar = 90;
  pTable->Codes[35].LargeChar = 123;
  
  pTable->Codes[36].HidCode = 0x2D; // -_
  pTable->Codes[36].SmallChar = 45;
  pTable->Codes[36].LargeChar = 95;

  pTable->Codes[37].HidCode = 0x2E; // =+
  pTable->Codes[37].SmallChar = 61;
  pTable->Codes[37].LargeChar = 43;
  
}


// parse the code from the usb interface and build the result string
UINT ParseScannerCode(AsciiHidTable_Type* pTable, USINT* pStartData, USINT DataLen, AsciiHidInternal_Type* pHidInternal)
{
  // check for upper/lower
  // check for characters
  USINT* pWorkData;
  
  // set field with markers for high/low to unchanged
  brsmemset((UDINT) &pHidInternal->HighLow, ASCII_HID_STATE_UNCHANGED, sizeof(pHidInternal->HighLow));
  pHidInternal->HighLow[0] = ASCII_HID_STATE_LOWER;
  pHidInternal->FoundLast = ASCII_HID_STATE_LOWER;
  
  // clear last data
  brsmemset((UDINT) &pHidInternal->Result, 0, sizeof(pHidInternal->Result));
  
  // - - - first check: which characters should be used (small, large):
  for (pHidInternal->i = 0, pWorkData = pStartData; pHidInternal->i < DataLen; pHidInternal->i++, pWorkData++)
  {
    if (*pWorkData == HID_KEY_HIGHER)
    {
      pHidInternal->FoundLast = ASCII_HID_STATE_HIGHER;
      pHidInternal->HighLow[pHidInternal->i] = ASCII_HID_STATE_HIGHER;
    }
    else if (*pWorkData == HID_KEY_LOWER)
    {
      pHidInternal->FoundLast = ASCII_HID_STATE_LOWER;
      pHidInternal->HighLow[pHidInternal->i] = ASCII_HID_STATE_LOWER;
    }
    else
    {
      pHidInternal->HighLow[pHidInternal->i] = pHidInternal->FoundLast;
    }
  }
  
  // result to the output
  brsmemset((UDINT) &pHidInternal->Result, 0, sizeof(pHidInternal->Result));
  
  // - - - get characters 
  pHidInternal->AsciiCnt = 0;
  for (pHidInternal->i = 0, pWorkData = pStartData; pHidInternal->i < DataLen; pHidInternal->i++, pWorkData++)
  {
    if (GetAsciiCodeFromTable(pTable, *pWorkData, pHidInternal->FoundLast, (USINT*) &pHidInternal->ResultChar) == 0)
    {
      pHidInternal->Result[pHidInternal->AsciiCnt] = pHidInternal->ResultChar;
      pHidInternal->AsciiCnt++;
    }
  }
 
  return pHidInternal->AsciiCnt;
}


// look after the code from the barcode scanner in the internal table
UINT GetAsciiCodeFromTable(AsciiHidTable_Type* pTable, USINT CheckForCode, ASCII_HID_STATE_enum LowHighValue,  USINT* pResult)
{
  USINT i;
  
  for (i=0; i<HID_ARR_LEN; i++)
  {
    if (pTable->Codes[i].HidCode != 0)
    {
      if (pTable->Codes[i].HidCode == CheckForCode)
      {
        if (LowHighValue == ASCII_HID_STATE_HIGHER)
        {
          *pResult = pTable->Codes[i].LargeChar;
        }
        else if (LowHighValue == ASCII_HID_STATE_LOWER)
        {
          *pResult = pTable->Codes[i].SmallChar;
        }
        else
        {
          // wrong Value
          *pResult = 0;
          return 2;
        }
        // found some values
        return 0;
      }
    }
  }
  // no value found
  return 1;
}


