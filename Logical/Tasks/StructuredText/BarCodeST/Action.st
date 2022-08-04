
ACTION SetHIDStandardTable_Action: 
  
  brsstrcpy(ADR(AsciiHidTableStandard.Sign),  ADR("Standard"));
  
  // set codes: numbers 0 - 9 and special characters
  AsciiHidTableStandard.Codes[0].HidCode := 16#27;  //0
  AsciiHidTableStandard.Codes[0].SmallChar := 48;
  AsciiHidTableStandard.Codes[0].LargeChar := 41;
  
  AsciiHidTableStandard.Codes[1].HidCode  := 16#1E; //1
  AsciiHidTableStandard.Codes[1].SmallChar := 49;
  AsciiHidTableStandard.Codes[1].LargeChar := 33;
  
  AsciiHidTableStandard.Codes[2].HidCode  := 16#1F; //2
  AsciiHidTableStandard.Codes[2].SmallChar := 50;
  AsciiHidTableStandard.Codes[2].LargeChar := 64;
  
  AsciiHidTableStandard.Codes[3].HidCode := 16#20; //3
  AsciiHidTableStandard.Codes[3].SmallChar := 51;
  AsciiHidTableStandard.Codes[3].LargeChar := 35;
  
  AsciiHidTableStandard.Codes[4].HidCode := 16#21; //4
  AsciiHidTableStandard.Codes[4].SmallChar := 52;
  AsciiHidTableStandard.Codes[4].LargeChar := 36;
  
  AsciiHidTableStandard.Codes[5].HidCode := 16#22; //5
  AsciiHidTableStandard.Codes[5].SmallChar := 53;
  AsciiHidTableStandard.Codes[5].LargeChar := 36;
  
  AsciiHidTableStandard.Codes[6].HidCode := 16#23; //6
  AsciiHidTableStandard.Codes[6].SmallChar := 54;
  AsciiHidTableStandard.Codes[6].LargeChar := 37;
  
  AsciiHidTableStandard.Codes[7].HidCode := 16#24; //7
  AsciiHidTableStandard.Codes[7].SmallChar := 55;
  AsciiHidTableStandard.Codes[7].LargeChar := 94;
  
  AsciiHidTableStandard.Codes[8].HidCode := 16#25; //8
  AsciiHidTableStandard.Codes[8].SmallChar := 56;
  AsciiHidTableStandard.Codes[8].LargeChar := 42;
  
  AsciiHidTableStandard.Codes[9].HidCode := 16#26; //9
  AsciiHidTableStandard.Codes[9].SmallChar := 57;
  AsciiHidTableStandard.Codes[9].LargeChar := 40;
  
  //letters: A-Z, a-z
  AsciiHidTableStandard.Codes[10].HidCode := 16#04; //a
  AsciiHidTableStandard.Codes[10].SmallChar := 65;
  AsciiHidTableStandard.Codes[10].LargeChar := 97;
  
  AsciiHidTableStandard.Codes[11].HidCode := 16#05; //b
  AsciiHidTableStandard.Codes[11].SmallChar := 66;
  AsciiHidTableStandard.Codes[11].LargeChar := 98;
  
  AsciiHidTableStandard.Codes[12].HidCode := 16#06; // c
  AsciiHidTableStandard.Codes[12].SmallChar := 67;
  AsciiHidTableStandard.Codes[12].LargeChar := 99;
  
  AsciiHidTableStandard.Codes[13].HidCode := 16#07; //d
  AsciiHidTableStandard.Codes[13].SmallChar := 68;
  AsciiHidTableStandard.Codes[13].LargeChar := 101;
  
  AsciiHidTableStandard.Codes[14].HidCode := 16#08; //e
  AsciiHidTableStandard.Codes[14].SmallChar := 69;
  AsciiHidTableStandard.Codes[14].LargeChar := 102;
  
  AsciiHidTableStandard.Codes[15].HidCode := 16#09; //f
  AsciiHidTableStandard.Codes[15].SmallChar := 70;
  AsciiHidTableStandard.Codes[15].LargeChar := 103;
  
  AsciiHidTableStandard.Codes[16].HidCode := 16#0A; // g
  AsciiHidTableStandard.Codes[16].SmallChar := 71;
  AsciiHidTableStandard.Codes[16].LargeChar := 104;

  AsciiHidTableStandard.Codes[17].HidCode := 16#0B; // h
  AsciiHidTableStandard.Codes[17].SmallChar := 72;
  AsciiHidTableStandard.Codes[17].LargeChar := 105;
  
  AsciiHidTableStandard.Codes[18].HidCode := 16#0C; // i
  AsciiHidTableStandard.Codes[18].SmallChar := 73;
  AsciiHidTableStandard.Codes[18].LargeChar := 106;
  
  AsciiHidTableStandard.Codes[19].HidCode := 16#0D; // j
  AsciiHidTableStandard.Codes[19].SmallChar := 74;
  AsciiHidTableStandard.Codes[19].LargeChar := 107;
  
  AsciiHidTableStandard.Codes[20].HidCode := 16#0E; // k
  AsciiHidTableStandard.Codes[20].SmallChar := 75;
  AsciiHidTableStandard.Codes[20].LargeChar := 108;
  
  AsciiHidTableStandard.Codes[21].HidCode := 16#0F; // l
  AsciiHidTableStandard.Codes[21].SmallChar := 76;
  AsciiHidTableStandard.Codes[21].LargeChar := 109;
  
  AsciiHidTableStandard.Codes[22].HidCode := 16#10; // m
  AsciiHidTableStandard.Codes[22].SmallChar := 77;
  AsciiHidTableStandard.Codes[22].LargeChar := 110;
  
  AsciiHidTableStandard.Codes[23].HidCode := 16#11; // n
  AsciiHidTableStandard.Codes[23].SmallChar := 78;
  AsciiHidTableStandard.Codes[23].LargeChar := 111;
  
  AsciiHidTableStandard.Codes[24].HidCode := 16#12; // o
  AsciiHidTableStandard.Codes[24].SmallChar := 79;
  AsciiHidTableStandard.Codes[24].LargeChar := 112;
  
  AsciiHidTableStandard.Codes[25].HidCode := 16#13; // p
  AsciiHidTableStandard.Codes[25].SmallChar := 80;
  AsciiHidTableStandard.Codes[25].LargeChar := 113;

  AsciiHidTableStandard.Codes[26].HidCode := 16#14; // q
  AsciiHidTableStandard.Codes[26].SmallChar := 81;
  AsciiHidTableStandard.Codes[26].LargeChar := 114;

  AsciiHidTableStandard.Codes[27].HidCode := 16#15; // r
  AsciiHidTableStandard.Codes[27].SmallChar := 82;
  AsciiHidTableStandard.Codes[27].LargeChar := 115;
  
  AsciiHidTableStandard.Codes[28].HidCode := 16#16; // s
  AsciiHidTableStandard.Codes[28].SmallChar := 83;
  AsciiHidTableStandard.Codes[28].LargeChar := 116;
  
  AsciiHidTableStandard.Codes[29].HidCode := 16#17; // t
  AsciiHidTableStandard.Codes[29].SmallChar := 84;
  AsciiHidTableStandard.Codes[29].LargeChar := 117;
  
  AsciiHidTableStandard.Codes[30].HidCode := 16#18; // u
  AsciiHidTableStandard.Codes[30].SmallChar := 85;
  AsciiHidTableStandard.Codes[30].LargeChar := 118;
  
  AsciiHidTableStandard.Codes[31].HidCode := 16#19; // v
  AsciiHidTableStandard.Codes[31].SmallChar := 86;
  AsciiHidTableStandard.Codes[31].LargeChar := 119;
  
  AsciiHidTableStandard.Codes[32].HidCode := 16#1A; // w
  AsciiHidTableStandard.Codes[32].SmallChar := 87;
  AsciiHidTableStandard.Codes[32].LargeChar := 120;
  
  AsciiHidTableStandard.Codes[33].HidCode := 16#1B; // x
  AsciiHidTableStandard.Codes[33].SmallChar := 88;
  AsciiHidTableStandard.Codes[33].LargeChar := 121;
  
  AsciiHidTableStandard.Codes[34].HidCode := 16#1C; // y
  AsciiHidTableStandard.Codes[34].SmallChar := 89;
  AsciiHidTableStandard.Codes[34].LargeChar := 122;
  
  AsciiHidTableStandard.Codes[35].HidCode := 16#1D; // z
  AsciiHidTableStandard.Codes[35].SmallChar := 90;
  AsciiHidTableStandard.Codes[35].LargeChar := 123;
  
  AsciiHidTableStandard.Codes[36].HidCode := 16#2D; // -_
  AsciiHidTableStandard.Codes[36].SmallChar := 45;
  AsciiHidTableStandard.Codes[36].LargeChar := 95;

  AsciiHidTableStandard.Codes[37].HidCode := 16#2E; // :=+
  AsciiHidTableStandard.Codes[37].SmallChar := 61;
  AsciiHidTableStandard.Codes[37].LargeChar := 43;
END_ACTION

//UsbManager.Status.ParseResult = ParseScannerCode((AsciiHidTable_Type*) &AsciiHidTableStandard, (USINT*) &ReadData, UsbManager.ReadIdx,(AsciiHidInternal_Type*) &AsciiHidInternal);

ACTION ParseScannerCode_ACTION:
  
  // check for upper/lower
  // check for characters
  //USINT* pWorkData;
  
  // set field with markers for high/low to unchanged
  brsmemset(ADR(AsciiHidInternal.HighLow), 0, SIZEOF(AsciiHidInternal.HighLow));
  AsciiHidInternal.HighLow[0] := ASCII_HID_STATE_LOWER;
  AsciiHidInternal.FoundLast := ASCII_HID_STATE_LOWER;
  
  // clear last data
  brsmemset(ADR(AsciiHidInternal.Result), 0, SIZEOF(AsciiHidInternal.Result));
  EndFor := UINT_TO_USINT(UsbManager.ReadIdx) -1;
  
  FOR AsciiHidInternal.i := 0 TO EndFor DO
    pWorkData ACCESS (ADR(ReadData) + AsciiHidInternal.i);
    IF pWorkData = HID_KEY_HIGHER THEN
      AsciiHidInternal.FoundLast := ASCII_HID_STATE_HIGHER;
      AsciiHidInternal.HighLow[AsciiHidInternal.i] := ASCII_HID_STATE_HIGHER;
    ELSIF pWorkData = HID_KEY_LOWER THEN
      AsciiHidInternal.FoundLast := ASCII_HID_STATE_LOWER;
      AsciiHidInternal.HighLow[AsciiHidInternal.i] := ASCII_HID_STATE_LOWER;
    ELSE
      AsciiHidInternal.HighLow[AsciiHidInternal.i] := AsciiHidInternal.FoundLast;
    END_IF
  END_FOR  
  
  // result to the output
  brsmemset(ADR(AsciiHidInternal.Result), 0, SIZEOF(AsciiHidInternal.Result));

  // - - - get characters 
  FOR AsciiHidInternal.i := 0 TO EndFor DO
    pWorkData ACCESS (ADR(ReadData) + AsciiHidInternal.i);
    
    j := 0;
    ValuesFound := FALSE;
    ExitRepeat := FALSE;
    REPEAT
    
      IF AsciiHidTableStandard.Codes[j].HidCode <> 0 THEN
        IF (AsciiHidTableStandard.Codes[j].HidCode = pWorkData) THEN
          IF AsciiHidInternal.FoundLast = ASCII_HID_STATE_HIGHER THEN
            AsciiHidInternal.ResultChar := AsciiHidTableStandard.Codes[j].LargeChar;
          ELSIF AsciiHidInternal.FoundLast = ASCII_HID_STATE_LOWER THEN
            AsciiHidInternal.ResultChar := AsciiHidTableStandard.Codes[j].SmallChar;
          ELSE
            // wrong Value
            AsciiHidInternal.ResultChar := 0;
          END_IF
          ValuesFound := TRUE;
        END_IF  
      END_IF
      
      IF (j >= HID_ARR_TABLE_LEN) OR (ValuesFound = TRUE)  THEN
        ExitRepeat := TRUE;
      END_IF 
      j := j +1;
    UNTIL
      ExitRepeat = TRUE
    END_REPEAT;
    
    IF ValuesFound = TRUE THEN
      AsciiHidInternal.Result[AsciiHidInternal.AsciiCnt] := AsciiHidInternal.ResultChar;
      AsciiHidInternal.AsciiCnt := AsciiHidInternal.AsciiCnt + 1 ;
    END_IF
    
 END_FOR
    UsbManager.Status.ParseResult := AsciiHidInternal.AsciiCnt;
    
END_ACTION