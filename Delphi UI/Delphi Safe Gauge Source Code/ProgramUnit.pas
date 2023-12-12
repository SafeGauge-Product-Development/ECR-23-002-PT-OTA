unit ProgramUnit;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, MMSystem, StdCtrls, ExtCtrls,
  Menus, VaClasses, VaComm, ExtDlgs;

type
  TForm1 = class(TForm)
    ProgramButton: TButton;
    Memo: TMemo;
    Timer1: TTimer;
    VaComm1: TVaComm;                                                                                       
    linkStatusLed: TShape;
    blueToothStatusLed: TShape;
    StaticText1: TStaticText;
    StaticText2: TStaticText;
    DataSendTimer: TTimer;
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    SensorIdString: TEdit;
    UpLoadData: TButton;
    IdOSLed: TShape;
    Label1: TLabel;
    MULTITOOL_led: TShape;
    PT20_led: TShape;
    PT25_led: TShape;
    PT100_led: TShape;
    PT300_led: TShape;
    PT600_led: TShape;
    DI10_led: TShape;
    TM1000_led: TShape;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    TS1000_led: TShape;
    LD30_led: TShape;
    UNKNOWN_led: TShape;
    Label8: TLabel;
    Label12: TLabel;
    Label7: TLabel;
    cpuDisplay: TLabel;
    CPU: TLabel;
    FileSelectCombo: TComboBox;
    OpenPictureDialog1: TOpenPictureDialog;
    BlueTimer: TTimer;
    Label13: TLabel;
    CodeRevision: TLabel;
    ResetDongleTimer: TTimer;
    Label14: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure ProgramButtonClick(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure VaComm1RxChar(Sender: TObject; Count: Integer);
    procedure RadioButton1Click(Sender: TObject);
    procedure RadioButton2Click(Sender: TObject);
    procedure UpLoadDataClick(Sender: TObject);
    procedure FileSelectComboChange(Sender: TObject);
    procedure ResetDongleClick(Sender: TObject);
    procedure BlueTimerTimer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure ResetDongleTimerTimer(Sender: TObject);

  private

  public
	{ Public declarations }
  end;
const
  //---------------------------------------------------------------
  // Sequence of stages
  //---------------------------------------------------------------
  SEQ1_START =                                         0;
  SEQ1_COMMS_SETUP =                                   1;
  SEQ1_SEND_DONGLE_WRITE_BACK_COMMAND =                20;
  SEQ1_WAITING_FOR_DONGLE_WRITE_BACK =                 21;
  SEQ1_RECEIVED_DONGLE_WRITE_BACK =                    22;
  SEQ1_WAITING_TO_SEND_GATT_DATA =                     23;
  SEQ1_ANALYSE_COMMS_RESPONSE =                        2;
  SEQ1_SEND_LINK_RESPONSE_COMMAND =                    3;
  SEQ1_WAITING_FOR_LINK_RESPONSE =                     4;
  SEQ1_SEND_HH_RESPONSE_COMMAND =                      5;
  SEQ1_WAITING_FOR_HH_RESPONSE =                       6;
  //---
  SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS =  7;
  SEQ2_SEND_BLOCKS_0_DATA_TO_BE_FLASHED =              8;
  SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED =             9;
  SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE =         10;
  SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE =            11;
  SEQ2_FINISHED =                                      12;
  SEQ2_WRITE_EEPROM =                                  13;
  SEQ2_WRITE_EEPROM_RESPONSE_WAIT =                    14;
  SEQ2_WRITE_EEPROM_RESPONSE_GOOD =                    15;
  SEQ2_PROBLEM =                                       16;
  SEQ2_END =                                           17;
  SEQ2_READY_FOR_NEXT_DEVICE =                         18;
  SEQ1_PAUSE =                                         19;
  SEQ2_PAUSE =                                         19;
  //---------------------------------------------------------------
  // 256K Device
  //---------------------------------------------------------------
  //CONST_FLASH_SPACE = 175103;
  //CONST_NUM_BLOCKS = 170;
  //CONST_DATA_BYTE_SPACE = 527705;
  //---------------------------------------------------------------
  // 1024K Device
  //---------------------------------------------------------------
  CONST_FLASH_SPACE = 704511;                       // single partition full space
  CONST_NUM_BLOCKS_PIC24_1024K = 171;               //343
  CONST_NUM_BLOCKS_DSPIC33_32K = 5;
  CONST_NUM_BLOCKS_PIC18_64K = 1023;
  CONST_DATA_BYTE_SPACE = 2123167;
  //---------------------------------------------------------------
  PKT_IS_HH_READY =                    $F0;
  PKT_HH_RESPONSE =                    $F1;
  PKT_BLOCK_PROGRAM_INFO =             $F2;
  PKT_BLOCK_DATA =                     $F3;
  PKT_PROGRAMMING_DONE =               $F4;
  PKT_PROGRAM_EEPROM =                 $F5;
  PKT_PROGRAM_EEPROM_RESPONSE =        $F6;
  PKT_READ_MEMORY_16 =                 $F7;
  PKT_MEMORY_16_READ =                 $F8;
  //---------------------------------------------------------------
  START_HEADER =                       $80;
  STOP_HEADER =                        $81;
  TEST_CONNECTION_HEADER =             $82;
  NO_CONNECTION_HEADER =               $83;
  CONNECTION_ESTABLISHED_HEADER =      $84;
  REBOOT_DONGLE =                      $85;
  GATT_DATA_HEADER =                   $87;
  RUN_BLUETOOTH =                      $88;
  BLE_STATES_HEADER =                  $8A;
  TRACE_HEADER =                       $8B;
  GETGATT_HEADER =                     $8C;
  START_BLUETOOTH =                    $8E;
  DISCONNECT_BLUETOOTH =               $8F;
  RESET_FLAG_0 =                       $90;
  RESET_FLAG_1 =                       $91;
  DONGLE_WRITE_BACK_HEADER =           $92;
  ENABLE_BT_SEQ =                      $93;
  BT_HAS_DISCONNECTED =                $94;
  DISCONNECTION_ERROR =                $95;
  //---------------------------------------------------------------
  NOT_CONNECTED =                      0;
  CONNECTED =                          1;
  //---------------------------------------------------------------
  FLAG_RESET = 0;
  FLAG_SET = 1;
  NO = 0;
  YES = 1;
  //---------------------------------------------------------------
  MULTITOOL_ID =           0;
  PT600_ID =               1;
  PT25_ID =                2;
  PT100_ID =               3;
  PT300_ID =               4;
  PT20_ID =                7;
  DI10_ID =                11;      // Dial Indicator
  TM1000_ID =              21;      // Tachometer
  TS1000_ID =              31;      // Temperature Sensor
  LD30_ID =                51;      // Laser Displacement
  //---------------------------------------------------------------
  MULTITOOL_OS_REVISION =  1;
  PT600_OS_REVISION =      1;
  PT25_OS_REVISION =       1;
  PT100_OS_REVISION =      1;
  PT300_OS_REVISION =      1;
  PT1000_OS_REVISION =     1;
  PT20_OS_REVISION =       1;
  DI10_OS_REVISION =       1;
  TM1000_OS_REVISION =     1;
  TS1000_OS_REVISION =     1;
  LD30_OS_REVISION =       1;
  //---------------------------------------------------------------
  PIC24_1024K =            0;
  DSPIC33_32K =            1;
  PIC18LF46K40 =           2;
  //---------------------------------------------------------------
  MULTITOOL_CPU =          PIC24_1024K;
  PT600_CPU =              PIC18LF46K40;
  PT25_CPU =               PIC18LF46K40;
  PT100_CPU =              PIC18LF46K40;
  PT300_CPU =              PIC18LF46K40;
  PT1000_CPU =             PIC18LF46K40;
  PT20_CPU =               PIC18LF46K40;
  DI10_CPU =               DSPIC33_32K;
  TM1000_CPU =             DSPIC33_32K;
  TS1000_CPU =             DSPIC33_32K;
  LD30_CPU =               DSPIC33_32K;
  //---------------------------------------------------------------

type
  dataArray = array [0..3071] of integer;
  writeMatrix = array[0..CONST_NUM_BLOCKS_PIC18_64K] of dataArray;

var
  Form1: TForm1;
  writeSpaceMatrix: writeMatrix;
  checksum1: array[0..1023] of Byte;
  checksum2: array[0..1023] of Byte;
  checksum3: array[0..1023] of Byte;
  checksum4: array[0..1023] of Byte;
  root_directory: String;
  data1: String;
  data2: String;
  OutFile1: TextFile;
  OutFile2: TextFile;
  OutFile3: TextFile;
  data_bytes_in_string: array[0..255] of String;
  data_bytes: array[0..255] of Integer;
  //---
  flash_space: array[0..1000000] of Longword;
  flash_space_written: array[0..1000000] of Integer;
  flash_blocks_written: array[0..1023] of Integer;
  //---
  security: array[0..255] of byte;
  seq1: Integer;
  seq2: Integer;
  packet: array[0..19] of Byte;
  linkStatus: Integer;
  commsStatus: Integer;
  blueToothStatus: Integer;
  header: Byte;
  packetByteCount: Integer;
  packetNibbles: array[0..39] of Integer;
  packetBytes: array[0..19] of Integer;
  blockNumber: Integer;
  packetId: Integer;
  numberOfWriteAttempts: Integer;

  productId: Integer;
  osRevision: Integer;
  cpuId: Integer;

  numbers: array[0..15] of String;

  block_id: Integer;
  packet_count: Integer;
  packet_returned_flag: Integer;
  current_block: Integer;

  blueToothOSstatus: Integer;
  trace_count: Integer;
  seqStow1: Integer;
  seqStow2: Integer;
  id_count: Integer;
  id_array: array[0..1023] of byte;
  deviceHasResponded: Integer;

  multitoolServiceUUID: string;
  multitoolCharacteristicUUID: string;
  sensorServiceUUID: string;
  sensorCharacteristicUUID: string;

  fileNameToProcess: string;
  directoryPath: string;
  fileToAnalyse: string;

  displayData: Integer;
  blueToothTimerFlag: Integer;

  //-----------------------------------------------------------
  longAddress: Longword;
  data_bytes_log: array[0..255] of byte;
  user_space: array[0..15] of byte;
  user_space_written: array[0..15] of byte;
  configuration_space: array[0..15] of byte;
  configuration_space_written: array[0..15] of byte;
  //-----------------------------------------------------------
  ModifiedFileString: String;
  normalCodeRevisionNumber: Integer;
  testAddress: Integer;
  configuration_readback: array[0..15] of byte;
  flash_readback: array[0..65535] of byte;
  //-----------------------------------------------------------

implementation

{$R *.DFM}

  //---------------------------------------------------------------
  // PKT_IS_HH_READY : Delphi->HH
  // Is the HH in Programming mode? - a timeout will be needed
  //---
  // data[0] = $F0
  // data[1..19] = 0 , ie filler
  //---------------------------------------------------------------
  // PKT_HH_RESPONSE : HH->Delphi
  // If the HH is ready it sends this packet
  //----
  // data[0] = $F1
  // data[1] = osRevision, this is "0" for the first OS
  // data[2] = product id, we are going to use 0 for a Multitool Handheld
  // data[2..19] = 0 , ie filler
  //---------------------------------------------------------------
  // PKT_BLOCK_PROGRAM_INFO : Delphi->HH
  // Block and checksum info needed
  //---
  // data[0] = $F2
  // data[1] = BLOCK MSB
  // data[2] = BLOCK LSB
  // data[3] = CHECKSUM1
  // data[4] = CHECKSUM2
  // data[5] = CHECKSUM3
  // data[6] = CHECKSUM4
  // data[7..19] = 0 , ie filler
  //---------------------------------------------------------------
  // PKT_BLOCK_DATA : Delphi->HH
  // We are sending the data packets to the HH
  //---
  // data[0] = $F3
  // data[1] = packet_id can be 0 to 170, sent one after another
  // data[2..19] = data, 18 bytes of data for packets 0..169, packet 170 has 12 bytes
  //---------------------------------------------------------------
  // PKT_PROGRAMMING_DONE : HH->Delphi
  // Is the HH in Programming mode? - a timeout will be needed
  //---
  // data[0] = $F4
  // data[1] = data transfer status, 0 = problem with data downloaded ie missed packet/packets , 1 = success in data download
  // data[2] = flash programming status, 0 = flash programming/verification fail, 1 = successful flash programming
  // data[3..19] = 0 , ie filler
  //---------------------------------------------------------------

procedure displayFlashMemory();
var
n: Integer;
i: Integer;
address: Integer;
data: Integer;
s: string;
begin
  //---
  address := 0;
  //---
  for n := 0 to 4095 do
    begin
      s := '0x' + IntToHex(address, 4) + ':     ';
      for i := 0 to 7 do
        begin
          if (flash_space_written[(n * 16) + (2 * i) + 0] = 1) or (flash_space_written[(n * 16) + (2 * i) + 1] = 1) then
            s := s + IntToHex(flash_space[(n * 16) + (2 * i) + 1], 2) + IntToHex(flash_space[(n * 16) + (2 * i) + 0], 2) + '   '
          else
            s := s + '....   ';
        end;
      //Form1.memo2.lines.add(s);
      address := address + 16;
    end;
  //---
end;

procedure displayFlashMemory2();
var
n: Integer;
i: Integer;
address: Integer;
data: Integer;
s: string;
begin
  //---
  address := 0;
  //---
  for n := 0 to 4095 do
    begin
      s := '0x' + IntToHex(address, 4) + ':     ';
      for i := 0 to 7 do
        begin
          if (flash_space_written[(n * 16) + (2 * i) + 0] = 1) or (flash_space_written[(n * 16) + (2 * i) + 1] = 1) then
            s := s + IntToHex(flash_space[(n * 16) + (2 * i) + 1], 2) + IntToHex(flash_space[(n * 16) + (2 * i) + 0], 2) + '   '
          else
            s := s + '....   ';
        end;
      //Form1.memo4.lines.add(s);
      address := address + 16;
    end;
  //---
end;

procedure displayUserMemory();
var
i: Integer;
address: Integer;
data: Integer;
s: string;
begin
  //---
  address := $200000;
  //---
  s := '0x' + IntToHex(address, 4) + ':   ';
  for i := 0 to 7 do
    begin
      if (user_space_written[(2 * i) + 0] = 1) or (user_space_written[(2 * i) + 1] = 1) then
        s := s + IntToHex(user_space[(2 * i) + 1], 2) + IntToHex(user_space[(2 * i) + 0], 2) + '   '
      else
        s := s + '....   ';
    end;
    
  //Form1.memo2.lines.add(s);
  //---
end;

procedure displayConfigurationMemory();
var
i: Integer;
address: Integer;
data: Integer;
s: string;
begin
  //---
  address := $300000;
  //---
  s := '0x' + IntToHex(address, 4) + ':   ';
  for i := 0 to 7 do
    begin
      if (configuration_space_written[(2 * i) + 0] = 1) or (configuration_space_written[(2 * i) + 1] = 1) then
        s := s + IntToHex(configuration_space[(2 * i) + 1], 2) + IntToHex(configuration_space[(2 * i) + 0], 2) + '   '
      else
        s := s + '....   ';
    end;

  //Form1.memo2.lines.add(s);
  //---
end;

procedure create_array_of_data_PIC24_33(block: Integer);
var
Sysex: string;
n: Integer;
checksum: Integer;
data: array[0..2047] of Integer;
data_A: Integer;
data_B: Integer;
begin
   //---
   for n := 0 to 2047 do data[n] := flash_space[n + (block * 2048)];
   for n := 0 to 1023 do
      begin
         data_A := data[n * 2];
         data_B := data[(n * 2) + 1];
         writeSpaceMatrix[block][(n * 3) + 0] := data_A and $FF;
         writeSpaceMatrix[block][(n * 3) + 1] := (data_A shr 8) and $FF;
         writeSpaceMatrix[block][(n * 3) + 2] := data_B and $FF;
      end;
   //---
   checksum := 0;
   //--------------------------------------------------------------------------------------
   // Secure the data
   //--------------------------------------------------------------------------------------
   //for n := 0 to 3071 do writeSpaceMatrix[block][n] := security[writeSpaceMatrix[block][n]];
   //--------------------------------------------------------------------------------------
   // Calculate the checksums of the secured data
   //--------------------------------------------------------------------------------------
   for n := 0 to 3071 do checksum := checksum + writeSpaceMatrix[block][n];
   //--------------------------------------------------------------------------------------
   checksum1[block] := (checksum shr 24) and $FF;
   checksum2[block] := (checksum shr 16) and $FF;
   checksum3[block] := (checksum shr 8) and $FF;
   checksum4[block] := checksum and $FF;
   //--------------------------------------------------------------------------------------
end;

procedure create_array_of_data_PIC18(block: Integer);
var
n: Integer;
checksum: Integer;
data: array[0..63] of Integer;
data_A: Integer;
data_B: Integer;
s: String;
begin
   //---
   s := 'block ' + IntToHex(block, 4) + ': ';
   for n := 0 to 63 do
      begin
        writeSpaceMatrix[block][n] := flash_space[n + (block * 64)];
        s := s + IntToHex(writeSpaceMatrix[block][n], 2) + ' ';
      end;
   //Form1.Memo4.Lines.Add(s);
   //---
   checksum := 0;
   //--------------------------------------------------------------------------------------
   // Secure the data
   //--------------------------------------------------------------------------------------
   for n := 0 to 3071 do writeSpaceMatrix[block][n] := security[writeSpaceMatrix[block][n]];
   //--------------------------------------------------------------------------------------
   // Calculate the checksums of the secured data
   //--------------------------------------------------------------------------------------
   for n := 0 to 63 do checksum := checksum + writeSpaceMatrix[block][n];
   //--------------------------------------------------------------------------------------
   checksum1[block] := (checksum shr 8) and $FF;
   checksum2[block] := checksum and $FF;
   //--------------------------------------------------------------------------------------
end;

procedure RS232_output(sdata: Byte);
var
c: Char;
begin
   c := char(sdata);
   Form1.VaComm1.WriteChar(c);
end;

procedure sendPacket();
var
c: Char;
n: Integer;
begin
    RS232_output($80);
    for n := 0 to 19 do
      begin
        RS232_output(packet[n] shr 4);
        RS232_output(packet[n] and $0F);
      end;
    RS232_output($81);
end;

procedure send_packet_PKT_IS_HH_READY();
var
n: Integer;
begin
   packet[0] := PKT_IS_HH_READY;
   for n := 1 to 19 do packet[n] := 0;
   sendPacket();
end;

function HexConvert(c: Char) : Integer;
var
   r: Integer;
begin
   r := 0;
   if c = '0' then r := 0;
   if c = '1' then r := 1;
   if c = '2' then r := 2;
   if c = '3' then r := 3;
   if c = '4' then r := 4;
   if c = '5' then r := 5;
   if c = '6' then r := 6;
   if c = '7' then r := 7;
   if c = '8' then r := 8;
   if c = '9' then r := 9;
   if ((c = 'A') or (c = 'a')) then r := 10;
   if ((c = 'B') or (c = 'b')) then r := 11;
   if ((c = 'C') or (c = 'c')) then r := 12;
   if ((c = 'D') or (c = 'd')) then r := 13;
   if ((c = 'E') or (c = 'e')) then r := 14;
   if ((c = 'F') or (c = 'f')) then r := 15;
   result := r;
end;

procedure analysis_of_hex_file_PIC24_1024K();
   // Summary at this point...
   // flash_blocks_written[0..CONST_NUM_BLOCKS] if = 1 then writeSpaceMatrix data needs to be written
   // writeSpaceMatrix[0..CONST_NUM_BLOCKS][0..1535] data to written over Bluetooth
   // checksum1,2,3,4[0..CONST_NUM_BLOCKS] checksum data needed to be writte
var
   input_file: TextFile;
   s: String;
   sn: String;
   byte_count: Integer;
   four_byte_address: Longword;
   record_type: Integer;
   n: Integer;
   m: Integer;
   data_point: Integer;
   extended_address: Longword;
   address: Longword;
   data: Integer;
   start_address: Longword;
   blk: Integer;
begin
   //-----------------------------------------------------------------------------------------------------
   // Analysis of HexFile and put data into an array "flash_space[address]" also "flash_space_written[address] = 1 if data was written into flash_space"
   //-----------------------------------------------------------------------------------------------------
   for n := 0 to CONST_FLASH_SPACE do flash_space[n] := $FFFF;
   for n := 0 to CONST_FLASH_SPACE do flash_space_written[n] := 0;
   extended_address := 0;
   //-----------------------------------------------------------------------------------------------------
   AssignFile(input_file, fileToAnalyse);
   Reset(input_file);
   while not Eof(input_file) do
      begin
         Readln(input_file, s);
         byte_count := (HexConvert(s[2]) * 16) + HexConvert(s[3]);
         four_byte_address := (HexConvert(s[4]) * 4096) + (HexConvert(s[5]) * 256) + (HexConvert(s[6]) * 16) + HexConvert(s[7]);
         four_byte_address := four_byte_address div 2;
         record_type := (HexConvert(s[8]) * 16) + HexConvert(s[9]);
         //---
         //====================================
         sn:= '';
         //====================================
         data_point := 10;
         for n := 0 to (byte_count - 1) do
            begin
                data_bytes_in_string[n] := s[data_point] + s[data_point + 1];
                //====================================
                sn:= sn + data_bytes_in_string[n] + ' ';
                //====================================
                data_bytes[n] := (HexConvert(s[data_point]) * 16) + HexConvert(s[data_point + 1]);
                data_point := data_point + 2;
            end;
         //---
         if record_type = 0 then
            begin
               //---
               data_point := 0;
               start_address := (extended_address * 32768) + four_byte_address - $400000;
               for n := 0 to ((byte_count div 4) - 1) do
                  begin
                     //---
                     address := (extended_address * 32768) + four_byte_address + (n * 2);
                     address := address - $400000;
                     //---
                     if address <= CONST_FLASH_SPACE then
                        begin
                           data := (data_bytes[data_point + 0] * 1)
                                 + (data_bytes[data_point + 1] * 256)
                                 + (data_bytes[data_point + 2] * 65536)
                                 + (data_bytes[data_point + 3] * 16777216);
                           flash_space[address] := data and $FFFF;
                           if flash_space[address] <> $FFFF then flash_space_written[address] := 1;
                           flash_space[address + 1] := data div 65536;
                           if flash_space[address + 1] <> $FFFF then flash_space_written[address] := 1;
                        end;
                     //---
                     data_point := data_point + 4;
                     //---
                  end;
                  //---
                  //====================================
                  //Form1.memo4.Lines.Add('type = ' + IntToStr(record_type) + '    address = ' + IntToStr(start_address) + '    bytes = ' + IntToStr(byte_count) + '    data = ' + sn);
                  //====================================
            end;
         //---
         if record_type = 4 then
            begin
               extended_address := (HexConvert(s[10]) * 4096) + (HexConvert(s[11]) * 256) +(HexConvert(s[12]) * 16) +(HexConvert(s[13]) * 1);
               //====================================
               //Form1.memo4.Lines.Add('type = ' + IntToStr(record_type) + '    extended address = ' + IntToStr(extended_address));
               //====================================
            end;
         //---
      end;
      Close(input_file);
      //-----------------------------------------------------------------------------------------------------
      // Find the blocks which have data
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to CONST_NUM_BLOCKS_PIC24_1024K do
      begin
         flash_blocks_written[n] := 0;
         for m := 0 to 2048 do
            begin
               if flash_space_written[(n * 2048) + m] = 1 then flash_blocks_written[n] := 1;
            end;
      end;
      //-----------------------------------------------------------------------------------------------------
      // Process the data_bytes_space so it can be written to the file
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to CONST_NUM_BLOCKS_PIC24_1024K do
         begin
            if flash_blocks_written[n] = 1 then create_array_of_data_PIC24_33(n);
         end;
      //-----------------------------------------------------------------------------------------------------
end;

procedure analysis_of_hex_file_DSPIC33_32K();
   // Summary at this point...
   // flash_blocks_written[0..CONST_NUM_BLOCKS] if = 1 then writeSpaceMatrix data needs to be written
   // writeSpaceMatrix[0..CONST_NUM_BLOCKS][0..1535] data to written over Bluetooth
   // checksum1,2,3,4[0..CONST_NUM_BLOCKS] checksum data needed to be writte
var
   input_file: TextFile;
   s: String;
   sn: String;
   byte_count: Integer;
   four_byte_address: Longword;
   record_type: Integer;
   n: Integer;
   m: Integer;
   data_point: Integer;
   extended_address: Longword;
   address: Longword;
   data: Integer;
   start_address: Longword;
   blk: Integer;
begin
   //-----------------------------------------------------------------------------------------------------
   // Analysis of HexFile and put data into an array "flash_space[address]" also "flash_space_written[address] = 1 if data was written into flash_space"
   //-----------------------------------------------------------------------------------------------------
   for n := 0 to CONST_FLASH_SPACE do flash_space[n] := $FFFF;
   for n := 0 to CONST_FLASH_SPACE do flash_space_written[n] := 0;
   extended_address := 0;
   //-----------------------------------------------------------------------------------------------------
   AssignFile(input_file, fileToAnalyse);
   Reset(input_file);
   while not Eof(input_file) do
      begin
         Readln(input_file, s);
         byte_count := (HexConvert(s[2]) * 16) + HexConvert(s[3]);
         four_byte_address := (HexConvert(s[4]) * 4096) + (HexConvert(s[5]) * 256) + (HexConvert(s[6]) * 16) + HexConvert(s[7]);
         four_byte_address := four_byte_address div 2;
         record_type := (HexConvert(s[8]) * 16) + HexConvert(s[9]);
         //---
         //====================================
         sn:= '';
         //====================================
         data_point := 10;
         for n := 0 to (byte_count - 1) do
            begin
                data_bytes_in_string[n] := s[data_point] + s[data_point + 1];
                //====================================
                sn:= sn + data_bytes_in_string[n] + ' ';
                //====================================
                data_bytes[n] := (HexConvert(s[data_point]) * 16) + HexConvert(s[data_point + 1]);
                data_point := data_point + 2;
            end;
         //---
         if record_type = 0 then
            begin
               //---
               data_point := 0;
               start_address := (extended_address * 32768) + four_byte_address - $400000;
               for n := 0 to ((byte_count div 4) - 1) do
                  begin
                     //---
                     address := (extended_address * 32768) + four_byte_address + (n * 2);
                     address := address - $400000;
                     //---
                     if address <= CONST_FLASH_SPACE then
                        begin
                           data := (data_bytes[data_point + 0] * 1)
                                 + (data_bytes[data_point + 1] * 256)
                                 + (data_bytes[data_point + 2] * 65536)
                                 + (data_bytes[data_point + 3] * 16777216);
                           flash_space[address] := data and $FFFF;
                           if flash_space[address] <> $FFFF then flash_space_written[address] := 1;
                           flash_space[address + 1] := data div 65536;
                           if flash_space[address + 1] <> $FFFF then flash_space_written[address] := 1;
                        end;
                     //---
                     data_point := data_point + 4;
                     //---
                  end;
                  //---
                  //====================================
                  //Form1.memo4.Lines.Add('type = ' + IntToStr(record_type) + '    address = ' + IntToHex(start_address, 8) + '    bytes = ' + IntToStr(byte_count) + '    data = ' + sn);
                  //====================================
            end;
         //---
         if record_type = 4 then
            begin
               extended_address := (HexConvert(s[10]) * 4096) + (HexConvert(s[11]) * 256) +(HexConvert(s[12]) * 16) +(HexConvert(s[13]) * 1);
               //====================================
               //Form1.memo4.Lines.Add('type = ' + IntToStr(record_type) + '    extended address = ' + IntToHex(extended_address, 8));
               //====================================
            end;
         //---
      end;
      Close(input_file);
      //-----------------------------------------------------------------------------------------------------
      // Find the blocks which have data
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to CONST_NUM_BLOCKS_DSPIC33_32K do
      begin
         flash_blocks_written[n] := 0;
         for m := 0 to 2048 do
            begin
               if flash_space_written[(n * 2048) + m] = 1 then flash_blocks_written[n] := 1;
            end;
         //if flash_blocks_written[n] = 1 then Form1.memo4.Lines.Add('block = ' + IntToStr(n));
      end;
      //-----------------------------------------------------------------------------------------------------
      // Process the data_bytes_space so it can be written to the file
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to CONST_NUM_BLOCKS_DSPIC33_32K do
         begin
            if flash_blocks_written[n] = 1 then create_array_of_data_PIC24_33(n);
         end;
      //-----------------------------------------------------------------------------------------------------
end;

procedure analysis_of_hex_file_PIC18_64K();
var
   input_file: TextFile;
   s: String;
   byte_count: Integer;
   record_type: Integer;
   n: Integer;
   m: Integer;
   data_point: Integer;
   extended_address: Longword;
   point: Integer;
   launchAddress: Integer;
   interruptAddress: Integer;
begin
   //---
   for n := 0 to 65536 do flash_space[n] := $FF;
   for n := 0 to 65536 do flash_space_written[n] := 0;
   //---
   for n := 0 to 15 do configuration_space[n] := $FF;
   for n := 0 to 15 do configuration_space_written[n] := 0;
   //---
   for n := 0 to 15 do user_space[n] := $FF;
   for n := 0 to 15 do user_space_written[n] := 0;
   //---
   extended_address := 0;
   AssignFile(input_file, directoryPath + Form1.FileSelectCombo.Text);
   //AssignFile(input_file, 'H:\Safe Gauge Work\PT_Series_Firmware.X\dist\default\production\PT_Series_Firmware.X.production.hex');
   Reset(input_file);
   while not Eof(input_file) do
      begin
         Readln(input_file, s);
         record_type := (HexConvert(s[8]) * 16) + HexConvert(s[9]);
         //------------------------------------------------------------------------------------------------------------
         if record_type = 0 then
         //------------------------------------------------------------------------------------------------------------
         begin
            //---
            byte_count := (HexConvert(s[2]) * 16) + HexConvert(s[3]);
            longAddress := (extended_address * 65536) + (HexConvert(s[4]) * 4096) + (HexConvert(s[5]) * 256) + (HexConvert(s[6]) * 16) + HexConvert(s[7]);
            //---
            data_point := 10;
            for n := 0 to 255 do data_bytes_log[n] := 0;
            for n := 0 to (byte_count - 1) do
                begin
                    data_bytes[n] := (HexConvert(s[data_point]) * 16) + HexConvert(s[data_point + 1]);
                    data_bytes_log[n] := 1;
                    data_point := data_point + 2;
                end;
            //-----------------------------------------------------------------
            // Flash Space
            //-----------------------------------------------------------------
            if longAddress < $10000 then
              begin
                for n := 0 to (byte_count - 1) do
                  begin
                    flash_space[longAddress] := data_bytes[n];
                    flash_space_written[longAddress] := 1;
                    longAddress := longAddress + 1;
                  end;
              end;
            //-----------------------------------------------------------------
            // Configuration Space
            //-----------------------------------------------------------------
            if (longAddress >= $300000) and (longAddress < $30000C) then
              begin
                for n := 0 to (byte_count - 1) do
                  begin
                    point := longAddress - $300000 + n;
                    configuration_space[point] := data_bytes[n];
                    configuration_space_written[point] := 1;
                  end;
              end;
            //-----------------------------------------------------------------
            // User Space
            //-----------------------------------------------------------------
            if (longAddress >= $200000) and (longAddress < $200010) then
              begin
                for n := 0 to (byte_count - 1) do
                  begin
                    point := longAddress - $200000 + n;
                    user_space[point] := data_bytes[n];
                    user_space_written[point] := 1;
                  end;
              end;
            //-----------------------------------------------------------------
         end;
         //------------------------------------------------------------------------------------------------------------
         if record_type = 4 then
         //------------------------------------------------------------------------------------------------------------
         begin
            extended_address := (HexConvert(s[10]) * 4096) + (HexConvert(s[11]) * 256) +(HexConvert(s[12]) * 16) +(HexConvert(s[13]) * 1);
         end;
         //------------------------------------------------------------------------------------------------------------
      end;
      Close(input_file);
      //---
      //displayFlashMemory();
      //-----------------------------------------------------------------------------------------------------
      // Process the data_bytes_space so it can be written to the file
      //-----------------------------------------------------------------------------------------------------
      launchAddress := $0022;
      flash_space[$4000] := flash_space[launchAddress + 0];
      flash_space_written[$4000] := 1;
      flash_space[$4001] := flash_space[launchAddress + 1];
      flash_space_written[$4001] := 1;
      flash_space[$4002] := flash_space[launchAddress + 2];
      flash_space_written[$4002] := 1;
      flash_space[$4003] := flash_space[launchAddress + 3];
      flash_space_written[$4003] := 1;
      //---
      interruptAddress := $0008;
      for n := 0 to 25 do
         begin
            flash_space[$4004 + n] := flash_space[interruptAddress + n];
            flash_space_written[$4004 + n] := 1;
         end;
      //-----------------------------------------------------------------------------------------------------
      // Clear the space $0000 to $3FFF and the blocks written
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to $3FFF do flash_space[n] := $FF;
      for n := 0 to $3FFF do flash_space_written[n] := 0;
      //-----------------------------------------------------------------------------------------------------
      // this is to protect the flash space $4040 to $4FFF which is reserved for calibration history, etc.
      //-----------------------------------------------------------------------------------------------------
      for n := $4040 to $4FFF do flash_space_written[n] := 0;
      //-----------------------------------------------------------------------------------------------------
      // Find the blocks which have data
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to CONST_NUM_BLOCKS_PIC18_64K do
      begin
         flash_blocks_written[n] := 0;
         for m := 0 to 63 do
            begin
               if flash_space_written[(n * 64) + m] = 1 then flash_blocks_written[n] := 1;
            end;
         //if flash_blocks_written[n] = 1 then Form1.memo4.Lines.Add('block = ' + IntToHex(n, 4));
      end;
      //-----------------------------------------------------------------------------------------------------
      for n := 0 to CONST_NUM_BLOCKS_PIC18_64K do
         begin
            if flash_blocks_written[n] = 1 then create_array_of_data_PIC18(n); //need to calculate on flash_blocks_written
         end;
      //-----------------------------------------------------------------------------------------------------
      //displayFlashMemory2();
      //-----------------------------------------------------------------------------------------------------
end;

procedure StartDataBlockSend_PIC_24_33(block: Integer);
var
n: Integer;
begin
   //---------------------------------------------
   packet[0] := PKT_BLOCK_PROGRAM_INFO;
   //---
   packet[1] := block shr 8;
   packet[2] := block and $FF;
   //---
   packet[3] := checksum1[block];
   packet[4] := checksum2[block];
   packet[5] := checksum3[block];
   packet[6] := checksum4[block];
   //---
   for n := 7 to 19 do packet[n] := 0;  // can use this to identify the product
   //---
   sendPacket();
   //---------------------------------------------
   for packetId := 0 to 169 do
      begin
         packet[0] := PKT_BLOCK_DATA;
         packet[1] := packetId;
         for n := 0 to 17 do packet[n + 2] := writeSpaceMatrix[block][n + (packetId * 18)];
         sendPacket();
      end;
   //---------------------------------------------
   packetId := 170;
   packet[0] := PKT_BLOCK_DATA;
   packet[1] := packetId;
   for n := 0 to 11 do packet[n + 2] := writeSpaceMatrix[block][n + (packetId * 18)];
   for n := 12 to 17 do packet[n + 2] := 0;
   sendPacket();
   //---------------------------------------------
end;

procedure StartDataBlockSend_PIC18(block: Integer);
var
n: Integer;
begin
   //---------------------------------------------
   packet[0] := PKT_BLOCK_PROGRAM_INFO;
   //---
   packet[1] := block shr 8;
   packet[2] := block and $FF;
   //---
   packet[3] := checksum1[block];
   packet[4] := checksum2[block];
   //---
   for n := 5 to 19 do packet[n] := 0;
   //---
   sendPacket();
   //---------------------------------------------
   for packetId := 0 to 3 do
      begin
         packet[0] := PKT_BLOCK_DATA;
         packet[1] := packetId;
         for n := 0 to 15 do packet[n + 2] := writeSpaceMatrix[block][n + (packetId * 16)];
         packet[18] := 0;
         packet[19] := 0;
         sendPacket();
      end;
   //---------------------------------------------
end;

procedure FileSorting();
var
n: Integer;
begin
   //---
   fileToAnalyse := directoryPath + Form1.FileSelectCombo.Items[Form1.FileSelectCombo.ItemIndex];
   //---
   case productId of
      //-------------------------------------------------------
      MULTITOOL_ID:
         begin
            analysis_of_hex_file_PIC24_1024K();
         end;
      //-------------------------------------------------------
      PT600_ID:
         begin
            analysis_of_hex_file_PIC18_64K();
         end;
      //-------------------------------------------------------
      PT25_ID:
         begin

         end;
      //-------------------------------------------------------
      PT100_ID:
         begin

         end;
      //-------------------------------------------------------
      PT300_ID:
         begin

         end;
      //-------------------------------------------------------
      PT20_ID:
         begin

         end;
      //-------------------------------------------------------
      DI10_ID:
         begin

         end;
      //-------------------------------------------------------
      TM1000_ID:
         begin
            analysis_of_hex_file_DSPIC33_32K();
         end;
      //-------------------------------------------------------
      TS1000_ID:
         begin

         end;
      //-------------------------------------------------------
      LD30_ID:
         begin

         end;
      //-------------------------------------------------------
    end;
   //---
   Form1.ProgramButton.Enabled := false;
   seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
   blockNumber := 0;
   //---
end;

procedure TForm1.ProgramButtonClick(Sender: TObject);
begin
   FileSorting();
end;

procedure displayCpu();
begin
   case cpuId of
      PIC24_1024K: Form1.cpuDisplay.Caption := 'PIC24_1024K';
      DSPIC33_32K: Form1.cpuDisplay.Caption := 'DSPIC33_32K';
      PIC18LF46K40: Form1.cpuDisplay.Caption := 'PIC18LF46K40';
   else
      Form1.cpuDisplay.Caption := '--------';
   end;
end;

procedure ledsToSilver();
begin
   Form1.blueToothStatusLed.brush.Color := clSilver;
   Form1.MULTITOOL_led.brush.Color := clSilver;
   Form1.PT20_led.brush.Color := clSilver;
   Form1.PT25_led.brush.Color := clSilver;
   Form1.PT100_led.brush.Color := clSilver;
   Form1.PT300_led.brush.Color := clSilver;
   Form1.PT600_led.brush.Color := clSilver;
   Form1.DI10_led.brush.Color := clSilver;
   Form1.TM1000_led.brush.Color := clSilver;
   Form1.TS1000_led.brush.Color := clSilver;
   Form1.LD30_led.brush.Color := clSilver;
   Form1.IdOSLed.brush.Color := clSilver;
   Form1.cpuDisplay.Caption := '--------';
end;

procedure comboBoxFileDir(Path: string; FileList: TStrings);
var
  searchResult: TSearchRec;
begin
   //---
   Form1.FileSelectCombo.Clear;
   Form1.FileSelectCombo.Text := 'No File Selected';
   //---
   if FindFirst(Path + '*.hex', faAnyFile, searchResult) = 0 then
      begin
         repeat
            if (searchResult.Attr <> faDirectory) then FileList.Add(searchResult.Name);
               until FindNext(searchResult) <> 0;
         SysUtils.FindClose(searchResult);
      end;
   //---
end;

procedure loadHexFile(id: Integer);
begin
   //---
   Form1.FileSelectCombo.Enabled := true;
   //---
   case productId of
      //-------------------------------------------------------
      MULTITOOL_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\MULTITOOL\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---
         end;
      //-------------------------------------------------------
      PT600_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\PT600\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      PT25_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\PT25\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      PT100_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\PT100\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      PT300_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\PT300\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      PT20_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\PT20\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      DI10_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\DI10\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      TM1000_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\TM1000\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      TS1000_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\TS1000\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
      LD30_ID:
         begin
            //---
            directoryPath := GetCurrentDir + '\Hex Files\LD30\';
            comboBoxFileDir(directoryPath, Form1.FileSelectCombo.Items);
            //---

         end;
      //-------------------------------------------------------
    end;
end;

procedure sortIds();
begin
   case productId of
      //-------------------------------------------------------
      MULTITOOL_ID:
         begin
            Form1.MULTITOOL_led.Brush.color := clLime;
            displayCpu();
            if osRevision = MULTITOOL_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(MULTITOOL_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      PT600_ID:
         begin
            Form1.PT600_led.Brush.color := clLime;
            displayCpu();
            if osRevision = PT600_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(PT600_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      PT25_ID:
         begin
            Form1.PT25_led.Brush.color := clLime;
            displayCpu();
            if osRevision = PT25_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(PT25_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      PT100_ID:
         begin
            Form1.PT100_led.Brush.color := clLime;
            displayCpu();
            if osRevision = PT100_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(PT100_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      PT300_ID:
         begin
            Form1.PT300_led.Brush.color := clLime;
            displayCpu();
            if osRevision = PT300_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(PT300_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      PT20_ID:
         begin
            Form1.PT20_led.Brush.color := clLime;
            displayCpu();
            if osRevision = PT20_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(PT20_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      DI10_ID:
         begin
            Form1.DI10_led.Brush.color := clLime;
            displayCpu();
            if osRevision = DI10_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(DI10_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      TM1000_ID:
         begin
            Form1.TM1000_led.Brush.color := clLime;
            displayCpu();
            if osRevision = TM1000_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(TM1000_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      TS1000_ID:
         begin
            Form1.TS1000_led.Brush.color := clLime;
            displayCpu();
            if osRevision = TS1000_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(TS1000_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      LD30_ID:
         begin
            Form1.LD30_led.Brush.color := clLime;
            displayCpu();
            if osRevision = LD30_OS_REVISION then
               begin
                  Form1.IdOSLed.Brush.color := clLime;
                  seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
                  loadHexFile(LD30_ID);
               end
            else
               begin
                  Form1.IdOSLed.Brush.color := clRed;
               end;
         end;
      //-------------------------------------------------------
      else
         begin
            Form1.UNKNOWN_led.Brush.color := clLime;
            Form1.IdOSLed.Brush.color := clSilver;
            displayCpu();
         end;
      //-------------------------------------------------------
   end;
end;

procedure programCommon();
var
n: Integer;
i: Integer;
begin
   //---
   if seqStow1 <> seq1 then
      begin
         seqStow1 := seq1;
      end;
   //---
   case seq1 of
      //------------------------------------------------------------
      SEQ1_START:
         begin
            Try
               begin
                  Form1.VaComm1.Open;
                  commsStatus := CONNECTED;
                  Form1.linkStatusLed.Brush.Color := clLime;
                  seq1 := SEQ1_SEND_DONGLE_WRITE_BACK_COMMAND;
               end
            Except
               begin
                  commsStatus := NOT_CONNECTED;
                  Form1.linkStatusLed.Brush.Color := clSilver;
               end;
            end;
         end;
      //------------------------------------------------------------
      SEQ1_SEND_DONGLE_WRITE_BACK_COMMAND:
         begin
            RS232_output(DONGLE_WRITE_BACK_HEADER);
            seq1 := SEQ1_WAITING_FOR_DONGLE_WRITE_BACK;
         end;
      //------------------------------------------------------------
      SEQ1_WAITING_FOR_DONGLE_WRITE_BACK:
         begin
            seq1 := SEQ1_SEND_DONGLE_WRITE_BACK_COMMAND;
         end;
      //------------------------------------------------------------
      SEQ1_RECEIVED_DONGLE_WRITE_BACK:
         begin
            seq1 := SEQ1_SEND_LINK_RESPONSE_COMMAND;
         end;
      //------------------------------------------------------------
      SEQ1_SEND_LINK_RESPONSE_COMMAND:
         begin
            RS232_output(TEST_CONNECTION_HEADER);
            seq1 := SEQ1_WAITING_FOR_LINK_RESPONSE;
         end;
      //------------------------------------------------------------
      SEQ1_WAITING_FOR_LINK_RESPONSE:
         begin
            //---
            if blueToothStatus = CONNECTED then
               begin
                  Form1.blueToothStatusLed.Brush.Color := clAqua;
                  seq1 := SEQ1_SEND_HH_RESPONSE_COMMAND;
               end
            else
               begin
                  Form1.blueToothStatusLed.Brush.Color := clSilver;
                  ledsToSilver();
                  seq1 := SEQ1_SEND_LINK_RESPONSE_COMMAND;
               end;
            //---
         end;
      //------------------------------------------------------------
      SEQ1_SEND_HH_RESPONSE_COMMAND:
         begin
            send_packet_PKT_IS_HH_READY();
            seq1 := SEQ1_WAITING_FOR_HH_RESPONSE;
         end;
      //------------------------------------------------------------
      SEQ1_WAITING_FOR_HH_RESPONSE:
         begin
            if deviceHasResponded = YES then
               begin
                  Form1.blueToothStatusLed.Brush.Color := clLime;
                  sortIds();
                  seq1 := SEQ1_PAUSE;
               end
            else
               begin
                  seq1 := SEQ1_SEND_LINK_RESPONSE_COMMAND;
               end;
         end;
      //------------------------------------------------------------
      SEQ1_PAUSE:
         begin
            // doing nothing
         end;
      //------------------------------------------------------------
   end;
end;

procedure program_PIC24_1024K();
var
n: Integer;
i: Integer;
begin
   //---
   if seqStow2 <> seq2 then
      begin
         seqStow2 := seq2;
      end;
   //---
   case seq2 of
      //------------------------------------------------------------
      SEQ2_PAUSE:
         begin
            // doing nothing
         end;
      //------------------------------------------------------------
      SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS:
         begin
            // waiting for button press to start loading code
         end;
      //------------------------------------------------------------
      SEQ2_SEND_BLOCKS_0_DATA_TO_BE_FLASHED:
        begin
          blockNumber := 0;
          Form1.Timer1.Interval := 500;
          seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
        end;
      //------------------------------------------------------------
      SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED:
         begin
            if blockNumber = (CONST_NUM_BLOCKS_PIC24_1024K + 1) then
               begin
                  //-----------------------------------------
                  // We have exhausted all the blocks
                  //-----------------------------------------
                  seq2 := SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE;
                  //-----------------------------------------
               end
            else
               begin
                  //-----------------------------------------
                  // keep looking for blocks of data
                  //-----------------------------------------
                  for n := blockNumber to CONST_NUM_BLOCKS_PIC24_1024K do
                     begin
                        if flash_blocks_written[n] = 1 then break;
                     end;
                  Form1.Memo.Lines.Add('Programming Block... ' + IntToStr(n));
                  current_block := n;
                  StartDataBlockSend_PIC_24_33(current_block);
                  seq2 := SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE;
                  blockNumber := n + 1;
                  //-----------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE:
         begin
            // status for transferred block data integrity... packetBytes[1] = data , packetBytes[2] = flash verification
            if ((packetBytes[1] = 1) and (packetBytes[2] = 1)) then
               begin
                  //---------------------------------------------
                  // Data is good
                  //---------------------------------------------
                  seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                  numberOfWriteAttempts := 0;
                  //---------------------------------------------
               end
            else
               begin
                  //---------------------------------------------
                  // this is an exception for the last block as the verification always fails due to it being in the flash configuration section
                  //---------------------------------------------
                  if blockNumber = (CONST_NUM_BLOCKS_PIC24_1024K + 1) then // configuration section, we don't test for errors
                     begin
                        if packetBytes[1] = 1 then
                           begin
                              seq2 := SEQ2_WRITE_EEPROM;
                              numberOfWriteAttempts := 0;
                           end
                        else
                           begin
                              seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                              numberOfWriteAttempts := numberOfWriteAttempts + 1;
                              Form1.Memo.Lines.Add('Programming Attempts... ' + IntToStr(numberOfWriteAttempts));
                              blockNumber := blockNumber - 1;
                              if numberOfWriteAttempts > 2 then seq2 := SEQ2_PROBLEM;
                           end;
                     end
                  else
                     begin
                        //---------------------------------------------
                        // Data is bad, lets try again
                        //---------------------------------------------
                        seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                        numberOfWriteAttempts := numberOfWriteAttempts + 1;
                        Form1.Memo.Lines.Add('Programming Attempts... ' + IntToStr(numberOfWriteAttempts));
                        blockNumber := blockNumber - 1;
                        if numberOfWriteAttempts > 2 then seq2 := SEQ2_PROBLEM;
                        //---------------------------------------------
                     end;
                  //---------------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ2_FINISHED:
         begin
            seq2 := SEQ2_WRITE_EEPROM;
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM:
         begin
            //---------------------------------------------
            packet[0] := PKT_PROGRAM_EEPROM;
            //---
            packet[1] := ord('L');
            packet[2] := ord('u');
            packet[3] := ord('k');
            packet[4] := ord('e');
            //---
            for n := 5 to 19 do packet[n] := 0;
            //---
            sendPacket();
            //---------------------------------------------
            seq2 := SEQ2_WRITE_EEPROM_RESPONSE_WAIT;
            //---------------------------------------------
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM_RESPONSE_WAIT:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM_RESPONSE_GOOD:
         begin
            Form1.Memo.Lines.Add('Finished');
            Form1.Memo.Lines.Add('Success!');
            Form1.Memo.Lines.Add('Turn Off Device');
            Form1.Memo.Lines.Add('Ready for next Device');
            RS232_output(DISCONNECT_BLUETOOTH);

            ledsToSilver();

            seq2 := SEQ2_READY_FOR_NEXT_DEVICE;
         end;
      //------------------------------------------------------------
      SEQ2_PROBLEM:
         begin
            //Form1.ResetDongle.Enabled := true;
            Form1.Memo.Lines.Add('There is a problem!');
            Form1.Memo.Lines.Add('Press Dongle Reset');
            Form1.Memo.Lines.Add('Reboot Windows App...');
            Form1.Memo.Lines.Add('Reboot Device...');
            Form1.Memo.Lines.Add('Try again...');
            seq2 := SEQ2_END;
         end;
      //------------------------------------------------------------
      SEQ2_END:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_READY_FOR_NEXT_DEVICE:
         begin
         end;
      //------------------------------------------------------------
   end;
end;

procedure program_DSPIC33_32K();
var
n: Integer;
i: Integer;
begin
   //---
   if seqStow2 <> seq2 then
      begin
         seqStow2 := seq2;
      end;
   //---
   case seq2 of
      //------------------------------------------------------------
      SEQ2_PAUSE:
         begin
            // doing nothing
         end;
      //------------------------------------------------------------
      SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS:
         begin
            // waiting for button press to start loading code
         end;
      //------------------------------------------------------------
      SEQ2_SEND_BLOCKS_0_DATA_TO_BE_FLASHED:
        begin
          blockNumber := 0;
          Form1.Timer1.Interval := 500;
          seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
        end;
      //------------------------------------------------------------
      SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED:
         begin
            if blockNumber = (CONST_NUM_BLOCKS_DSPIC33_32K + 1) then
               begin
                  //-----------------------------------------
                  // We have exhausted all the blocks
                  //-----------------------------------------
                  seq2 := SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE;
                  //-----------------------------------------
               end
            else
               begin
                  //-----------------------------------------
                  // keep looking for blocks of data
                  //-----------------------------------------
                  for n := blockNumber to CONST_NUM_BLOCKS_DSPIC33_32K do
                     begin
                        if flash_blocks_written[n] = 1 then break;
                     end;
                  Form1.Memo.Lines.Add('Programming Block... ' + IntToStr(n));
                  current_block := n;
                  StartDataBlockSend_PIC_24_33(current_block);
                  seq2 := SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE;
                  blockNumber := n + 1;
                  //-----------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE:
         begin
            // status for transferred block data integrity... packetBytes[1] = data , packetBytes[2] = flash verification
            //Form1.Memo4.Lines.Add('Block Message = ' + IntToStr(blockNumber)); ////////////////////////////////////////////////
            if ((packetBytes[1] = 1) and (packetBytes[2] = 1)) then
               begin
                  //---------------------------------------------
                  // Data is good
                  //---------------------------------------------
                  seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                  numberOfWriteAttempts := 0;
                  //---------------------------------------------
                  if blockNumber = (CONST_NUM_BLOCKS_DSPIC33_32K + 1) then
                     begin
                        seq2 := SEQ2_WRITE_EEPROM;
                     end;
                  //---------------------------------------------
               end
            else
               begin
                  //---------------------------------------------
                  // Data is bad, lets try again
                  //---------------------------------------------
                  seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                  numberOfWriteAttempts := numberOfWriteAttempts + 1;
                  Form1.Memo.Lines.Add('Programming Attempts... ' + IntToStr(numberOfWriteAttempts));
                  blockNumber := blockNumber - 1;
                  if numberOfWriteAttempts > 2 then seq2 := SEQ2_PROBLEM;
                  //---------------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ2_FINISHED:
         begin
            seq2 := SEQ2_WRITE_EEPROM;
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM:
         begin
            //---------------------------------------------
            packet[0] := PKT_PROGRAM_EEPROM;
            //---
            packet[1] := ord('L');
            packet[2] := ord('u');
            packet[3] := ord('k');
            packet[4] := ord('e');
            //---
            for n := 5 to 19 do packet[n] := 0;
            //---
            sendPacket();
            //Form1.memo4.Lines.Add('send Luke');
            //---------------------------------------------
            seq2 := SEQ2_WRITE_EEPROM_RESPONSE_WAIT;
            //---------------------------------------------
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM_RESPONSE_WAIT:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM_RESPONSE_GOOD:
         begin
            Form1.Memo.Lines.Add('Finished');
            Form1.Memo.Lines.Add('Success!');
            Form1.Memo.Lines.Add('Turn Off Device');
            Form1.Memo.Lines.Add('Ready for next Device');
            RS232_output(DISCONNECT_BLUETOOTH);

            ledsToSilver();

            seq2 := SEQ2_READY_FOR_NEXT_DEVICE;
         end;
      //------------------------------------------------------------
      SEQ2_PROBLEM:
         begin
            //Form1.ResetDongle.Enabled := true;
            Form1.Memo.Lines.Add('There is a problem!');
            Form1.Memo.Lines.Add('Press Dongle Reset');
            Form1.Memo.Lines.Add('Reboot Windows App...');
            Form1.Memo.Lines.Add('Reboot Devicel...');
            Form1.Memo.Lines.Add('Try again...');
            seq2 := SEQ2_END;
         end;
      //------------------------------------------------------------
      SEQ2_END:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_READY_FOR_NEXT_DEVICE:
         begin
         end;
      //------------------------------------------------------------
   end;
end;

procedure program_PIC18_64K();
var
n: Integer;
i: Integer;
begin
   //---
   if seqStow2 <> seq2 then
      begin
         seqStow2 := seq2;
      end;
   //---
   case seq2 of
      //------------------------------------------------------------
      SEQ2_PAUSE:
         begin
            // doing nothing
         end;
      //------------------------------------------------------------
      SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS:
         begin
            // waiting for button press to start loading code
         end;
      //------------------------------------------------------------
      SEQ2_SEND_BLOCKS_0_DATA_TO_BE_FLASHED:
        begin
          blockNumber := 0;
          Form1.Timer1.Interval := 500;
          seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
        end;
      //------------------------------------------------------------
      SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED:
         begin
          //-----------------------------------------
          // keep looking for blocks of data
          //-----------------------------------------
          for n := blockNumber to CONST_NUM_BLOCKS_PIC18_64K do
            begin
              if flash_blocks_written[n] = 1 then
                break;
            end;
          //---
          if flash_blocks_written[n] = 1 then
            begin
              // Send data to be programmed
              Form1.Memo.Lines.Add('Programming Block... ' + IntToStr(n));
              current_block := n;
              StartDataBlockSend_PIC18(current_block);
              seq2 := SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE;
              blockNumber := n + 1;
            end
          else
            begin
              // have exhausted all the blocks to be programmed
              Form1.Memo.Lines.Add('End it here !!!');
              seq2 := SEQ2_WRITE_EEPROM;
            end;
          //-----------------------------------------
         end;
      //------------------------------------------------------------
      SEQ2_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE:
         begin
            // status for transferred block data integrity... packetBytes[1] = data , packetBytes[2] = flash verification
            if ((packetBytes[1] = 1) and (packetBytes[2] = 1)) then
               begin
                  //---------------------------------------------
                  // Data is good
                  //---------------------------------------------
                  seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                  numberOfWriteAttempts := 0;
                  //---------------------------------------------
               end
            else
               begin
                  //---------------------------------------------
                  // Data is bad, lets try again
                  //---------------------------------------------
                  seq2 := SEQ2_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                  numberOfWriteAttempts := numberOfWriteAttempts + 1;
                  Form1.Memo.Lines.Add('Programming Attempts... ' + IntToStr(numberOfWriteAttempts));
                  blockNumber := blockNumber - 1;
                  if numberOfWriteAttempts > 2 then seq2 := SEQ2_PROBLEM;
                  //---------------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ2_FINISHED:
         begin
            seq2 := SEQ2_WRITE_EEPROM;
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM:
         begin
            //---------------------------------------------
            packet[0] := PKT_PROGRAM_EEPROM;
            //---
            packet[1] := ord('L');
            packet[2] := ord('u');
            packet[3] := ord('k');
            packet[4] := ord('e');
            //---
            for n := 5 to 19 do packet[n] := 0;
            //---
            sendPacket();
            //---------------------------------------------
            seq2 := SEQ2_WRITE_EEPROM_RESPONSE_WAIT;
            //---------------------------------------------
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM_RESPONSE_WAIT:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_WRITE_EEPROM_RESPONSE_GOOD:
         begin
            Form1.Memo.Lines.Add('Finished');
            Form1.Memo.Lines.Add('Success!');
            Form1.Memo.Lines.Add('Turn Off Device');
            Form1.Memo.Lines.Add('Ready for next Device');
            RS232_output(DISCONNECT_BLUETOOTH);
            seq2 := SEQ2_READY_FOR_NEXT_DEVICE;
            Form1.ResetDongleTimer.Enabled := true;
            ledsToSilver();
            Form1.UpLoadData.Enabled := true;  ////////////////////////////////////////////////////
         end;
      //------------------------------------------------------------
      SEQ2_PROBLEM:
         begin
            Form1.ResetDongleTimer.Enabled := true;
            Form1.Memo.Lines.Add('There is a problem!');
            //Form1.Memo.Lines.Add('Press Dongle Reset');
            Form1.Memo.Lines.Add('Reboot Windows App...');
            Form1.Memo.Lines.Add('Reboot Devicel...');
            Form1.Memo.Lines.Add('Try again...');
            seq2 := SEQ2_END;
         end;
      //------------------------------------------------------------
      SEQ2_END:
         begin
         end;
      //------------------------------------------------------------
      SEQ2_READY_FOR_NEXT_DEVICE:
         begin
         end;
      //------------------------------------------------------------
   end;
end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
   programCommon();
   case productId of
      //-------------------------------------------------------
      MULTITOOL_ID:
         begin
            program_PIC24_1024K();
         end;
      //-------------------------------------------------------
      PT600_ID:
         begin
            program_PIC18_64K();
         end;
      //-------------------------------------------------------
      PT25_ID:
         begin
         end;
      //-------------------------------------------------------
      PT100_ID:
         begin
         end;
      //-------------------------------------------------------
      PT300_ID:
         begin
         end;
      //-------------------------------------------------------
      PT20_ID:
         begin
         end;
      //-------------------------------------------------------
      DI10_ID:
         begin
         end;
      //-------------------------------------------------------
      TM1000_ID:
         begin
            program_DSPIC33_32K();
         end;
      //-------------------------------------------------------
      TS1000_ID:
         begin
         end;
      //-------------------------------------------------------
      LD30_ID:
         begin
         end;
      //-------------------------------------------------------
    end;
end;

procedure TForm1.VaComm1RxChar(Sender: TObject; Count: Integer);
var
   i: Integer;
   s: string;
   data: Byte;
   n: Integer;
   s1: string;
begin
   s := VaComm1.ReadText;
   for i := 1 to Length(s) do
      begin
         //---
         data := ord(s[i]);
         //---
         if (data and $80) = $80 then
            begin
               //-----------------------------------
               // Header
               //-----------------------------------
               header := data;
               //-----------------------------------
               case header of
                   //------------------------------------------------------------
                   BT_HAS_DISCONNECTED:  //////////////////////////////////////////////////////////////////////////////////
                     begin
                        Form1.UpLoadData.Enabled := true;
                        blueToothStatus := NOT_CONNECTED;
                        ProgramButton.Enabled := false;
                        Form1.FileSelectCombo.Enabled := false;
                        ledsToSilver();
                     end;
                  //------------------------------------------------------------
                  DONGLE_WRITE_BACK_HEADER:
                     begin
                        seq1 := SEQ1_RECEIVED_DONGLE_WRITE_BACK;
                     end;
                  //------------------------------------------------------------
                  DISCONNECTION_ERROR:
                     begin
                     end;
                  //------------------------------------------------------------
                  CONNECTION_ESTABLISHED_HEADER:
                     begin
                        linkStatus := CONNECTED;
                        blueToothStatus := CONNECTED;
                     end;
                  //------------------------------------------------------------
                  NO_CONNECTION_HEADER:
                     begin
                        blueToothStatus := NOT_CONNECTED;
                        ProgramButton.Enabled := false;
                        Form1.FileSelectCombo.Enabled := false;
                        ledsToSilver();
                     end;
                  //------------------------------------------------------------
                  GETGATT_HEADER:
                    begin
                      id_count := 0;
                    end;
                  //------------------------------------------------------------
                  START_HEADER:
                     begin
                        packetByteCount := 0;
                     end;
                  //------------------------------------------------------------
                  STOP_HEADER:
                     begin
                        //------------------------------------------------------------
                        // we have a packet
                        //------------------------------------------------------------
                        for n := 0 to 19 do packetBytes[n] := (packetNibbles[n * 2] * 16) + packetNibbles[(n * 2) + 1];
                        //------------------------------------------------------------
                         case packetBytes[0] of
                           //------------------------------------------------------------
                           PKT_BLOCK_PROGRAM_INFO:
                              begin
                              end;
                           //------------------------------------------------------------
                           PKT_BLOCK_DATA:
                              begin
                              end;
                           //------------------------------------------------------------
                           PKT_HH_RESPONSE:
                              begin
                                 osRevision := packetBytes[1];
                                 productId := packetBytes[2];
                                 cpuId := packetBytes[3];
                                 normalCodeRevisionNumber := packetBytes[4] + (256 * packetBytes[5]);
                                 CodeRevision.Caption := IntToStr(normalCodeRevisionNumber);
                                 deviceHasResponded := YES;
                              end;
                           //------------------------------------------------------------
                           PKT_PROGRAMMING_DONE:
                              begin
                                //---
                                case cpuId of
                                //-------------------------------------------------------
                                DSPIC33_32K:
                                  begin
                                    if blockNumber = 172 then
                                      seq2 := SEQ2_FINISHED
                                    else
                                      seq2 := SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE;
                                  end;
                                //-------------------------------------------------------
                                PIC24_1024K:
                                  begin
                                    if blockNumber = 172 then
                                      seq2 := SEQ2_FINISHED
                                    else
                                      seq2 := SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE;
                                  end;
                                //-------------------------------------------------------
                                PIC18LF46K40:
                                  begin
                                    seq2 := SEQ2_RECEIVED_FLASH_BLOCK_DATA_RESPONSE;
                                  end;
                                //-------------------------------------------------------
                              end;
                              //---
                           end;
                           //------------------------------------------------------------
                           PKT_PROGRAM_EEPROM_RESPONSE:
                              begin
                                 if (packetBytes[1] = 0) and (packetBytes[2] = ord('L')) and (packetBytes[3] = ord('u')) and (packetBytes[4] = ord('k')) and(packetBytes[5] = ord('e')) then
                                    begin
                                       seq2 := SEQ2_WRITE_EEPROM_RESPONSE_GOOD;
                                    end
                                 else
                                    begin
                                       seq2 := SEQ2_PROBLEM;
                                    end;
                              end;
                           //------------------------------------------------------------
                         end;
                        //------------------------------------------------------------
                     end;
                  //------------------------------------------------------------
               end;
            end
         else
            begin
               case header of
               //------------------------------------------------------------
               NO_CONNECTION_HEADER:
                  begin
                  end;
               //------------------------------------------------------------
               CONNECTION_ESTABLISHED_HEADER:
                  begin
                  end;
               //------------------------------------------------------------
               START_HEADER:
                  begin
                     packetNibbles[packetByteCount] := data;
                     packetByteCount := packetByteCount + 1;
                  end;
               //------------------------------------------------------------
               BLE_STATES_HEADER:
                  begin
                  end;
               //------------------------------------------------------------
               TRACE_HEADER:
                  begin
                    //Form1.Memo4.Lines.Add(IntToStr(trace_count) + ' value = ' + IntToStr(data));
                    trace_count := trace_count + 1;
                  end;
               //------------------------------------------------------------
               GETGATT_HEADER:
                  begin
                    //---
                    id_array[id_count] := data;
                    //---
                    if id_count = 63 then
                    begin
                      //---
                      s1 := '';
                      for n := 0 to 15 do s1 := s1 + IntToHex((16 * id_array[n * 2]) + id_array[(n * 2) + 1], 2) + ', ';
                      //---
                      s1 := '';
                      for n := 16 to 31 do s1 := s1 + IntToHex((16 * id_array[n * 2]) + id_array[(n * 2) + 1], 2) + ', ';
                      //---
                    end;
                    //---
                    id_count := id_count + 1;
                    //---
                  end;
               //------------------------------------------------------------
               end;
            end;
      end;
end;

procedure TForm1.FormCreate(Sender: TObject);
var
n: Integer;
m: Integer;
begin
   //----------------------------------------------------------------------
   security[0] := 213;
   security[1] := 102;
   security[2] := 167;
   security[3] := 127;
   security[4] := 253;
   security[5] := 42;
   security[6] := 151;
   security[7] := 169;
   security[8] := 160;
   security[9] := 191;
   security[10] := 50;
   security[11] := 136;
   security[12] := 156;
   security[13] := 83;
   security[14] := 173;
   security[15] := 207;
   security[16] := 252;
   security[17] := 223;
   security[18] := 21;
   security[19] := 150;
   security[20] := 68;
   security[21] := 172;
   security[22] := 37;
   security[23] := 7;
   security[24] := 233;
   security[25] := 94;
   security[26] := 103;
   security[27] := 183;
   security[28] := 192;
   security[29] := 40;
   security[30] := 176;
   security[31] := 73;
   security[32] := 226;
   security[33] := 115;
   security[34] := 110;
   security[35] := 221;
   security[36] := 130;
   security[37] := 135;
   security[38] := 9;
   security[39] := 11;
   security[40] := 236;
   security[41] := 124;
   security[42] := 35;
   security[43] := 111;
   security[44] := 8;
   security[45] := 41;
   security[46] := 152;
   security[47] := 181;
   security[48] := 227;
   security[49] := 178;
   security[50] := 238;
   security[51] := 61;
   security[52] := 28;
   security[53] := 16;
   security[54] := 231;
   security[55] := 92;
   security[56] := 179;
   security[57] := 153;
   security[58] := 198;
   security[59] := 229;
   security[60] := 148;
   security[61] := 0;
   security[62] := 58;
   security[63] := 180;
   security[64] := 254;
   security[65] := 23;
   security[66] := 129;
   security[67] := 218;
   security[68] := 137;
   security[69] := 64;
   security[70] := 12;
   security[71] := 185;
   security[72] := 74;
   security[73] := 55;
   security[74] := 138;
   security[75] := 251;
   security[76] := 13;
   security[77] := 184;
   security[78] := 118;
   security[79] := 186;
   security[80] := 29;
   security[81] := 199;
   security[82] := 72;
   security[83] := 89;
   security[84] := 26;
   security[85] := 170;
   security[86] := 36;
   security[87] := 25;
   security[88] := 131;
   security[89] := 95;
   security[90] := 197;
   security[91] := 32;
   security[92] := 133;
   security[93] := 44;
   security[94] := 88;
   security[95] := 128;
   security[96] := 114;
   security[97] := 79;
   security[98] := 243;
   security[99] := 45;
   security[100] := 201;
   security[101] := 204;
   security[102] := 120;
   security[103] := 75;
   security[104] := 98;
   security[105] := 49;
   security[106] := 162;
   security[107] := 108;
   security[108] := 86;
   security[109] := 145;
   security[110] := 161;
   security[111] := 14;
   security[112] := 139;
   security[113] := 93;
   security[114] := 80;
   security[115] := 90;
   security[116] := 182;
   security[117] := 210;
   security[118] := 196;
   security[119] := 228;
   security[120] := 132;
   security[121] := 48;
   security[122] := 54;
   security[123] := 125;
   security[124] := 39;
   security[125] := 248;
   security[126] := 10;
   security[127] := 97;
   security[128] := 217;
   security[129] := 209;
   security[130] := 187;
   security[131] := 168;
   security[132] := 174;
   security[133] := 76;
   security[134] := 202;
   security[135] := 149;
   security[136] := 126;
   security[137] := 242;
   security[138] := 57;
   security[139] := 62;
   security[140] := 122;
   security[141] := 2;
   security[142] := 78;
   security[143] := 3;
   security[144] := 141;
   security[145] := 109;
   security[146] := 6;
   security[147] := 154;
   security[148] := 164;
   security[149] := 66;
   security[150] := 18;
   security[151] := 134;
   security[152] := 189;
   security[153] := 30;
   security[154] := 20;
   security[155] := 4;
   security[156] := 214;
   security[157] := 70;
   security[158] := 22;
   security[159] := 101;
   security[160] := 171;
   security[161] := 77;
   security[162] := 244;
   security[163] := 163;
   security[164] := 107;
   security[165] := 71;
   security[166] := 87;
   security[167] := 123;
   security[168] := 82;
   security[169] := 208;
   security[170] := 220;
   security[171] := 241;
   security[172] := 52;
   security[173] := 216;
   security[174] := 224;
   security[175] := 17;
   security[176] := 225;
   security[177] := 81;
   security[178] := 38;
   security[179] := 119;
   security[180] := 239;
   security[181] := 116;
   security[182] := 96;
   security[183] := 65;
   security[184] := 250;
   security[185] := 177;
   security[186] := 59;
   security[187] := 46;
   security[188] := 247;
   security[189] := 53;
   security[190] := 246;
   security[191] := 211;
   security[192] := 47;
   security[193] := 190;
   security[194] := 232;
   security[195] := 212;
   security[196] := 205;
   security[197] := 34;
   security[198] := 158;
   security[199] := 155;
   security[200] := 100;
   security[201] := 222;
   security[202] := 56;
   security[203] := 230;
   security[204] := 245;
   security[205] := 203;
   security[206] := 31;
   security[207] := 234;
   security[208] := 106;
   security[209] := 240;
   security[210] := 175;
   security[211] := 33;
   security[212] := 143;
   security[213] := 140;
   security[214] := 237;
   security[215] := 27;
   security[216] := 255;
   security[217] := 200;
   security[218] := 147;
   security[219] := 215;
   security[220] := 1;
   security[221] := 159;
   security[222] := 69;
   security[223] := 15;
   security[224] := 99;
   security[225] := 165;
   security[226] := 219;
   security[227] := 51;
   security[228] := 112;
   security[229] := 117;
   security[230] := 146;
   security[231] := 60;
   security[232] := 206;
   security[233] := 104;
   security[234] := 188;
   security[235] := 195;
   security[236] := 142;
   security[237] := 5;
   security[238] := 121;
   security[239] := 91;
   security[240] := 105;
   security[241] := 194;
   security[242] := 67;
   security[243] := 43;
   security[244] := 19;
   security[245] := 63;
   security[246] := 193;
   security[247] := 113;
   security[248] := 249;
   security[249] := 85;
   security[250] := 157;
   security[251] := 84;
   security[252] := 235;
   security[253] := 144;
   security[254] := 166;
   security[255] := 24;
   //----------------------------------------------------------------------
   seq1 := SEQ1_START;
   seq2 := SEQ2_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
   linkStatus := NOT_CONNECTED;
   blueToothOSstatus := NOT_CONNECTED;
   linkStatusLed.Brush.Color := clSilver;
   blueToothStatusLed.Brush.Color := clSilver;
   IdOSLed.Brush.Color := clSilver;
   commsStatus := NOT_CONNECTED;
   //VaComm1.Baudrate := br4800;
   VaComm1.Baudrate := br9600;
   //VaComm1.Baudrate := br19200;
   VaComm1.PortNum := 127;
   blueToothStatus := NOT_CONNECTED;
   numberOfWriteAttempts := 0;
   osRevision := $FFFF; // note only a number between 0 and 255 is valid, $FFFF tells us we have not received an OS number yet
   cpuId := $FFFF;
   trace_count := 0;
   seqStow1 := 255;
   seqStow2 := 255;
   deviceHasResponded := NO;
   displayData := YES;
   //----------------------------------------------------------------------
   multitoolServiceUUID :=        'aedd13875f45437989d0239b20c65970';
   multitoolCharacteristicUUID := 'ce603408058049c0a4bcb5d56bd75f09';
   sensorServiceUUID :=           '477b695efb984cfc9c89539326';
   sensorCharacteristicUUID :=    '8de9b8c20fff4009b2f7da9748730c57';
   //----------------------------------------------------------------------
   blueToothTimerFlag := 0;
   normalCodeRevisionNumber := $FFFF;
   //----------------------------------------------------------------------
end;

procedure TForm1.RadioButton1Click(Sender: TObject);
begin
  seq1 := SEQ1_SEND_LINK_RESPONSE_COMMAND;
  blueToothStatus := NOT_CONNECTED;
  SensorIdString.Visible := false;
end;

procedure TForm1.RadioButton2Click(Sender: TObject);
begin
  SensorIdString.Visible := true;
end;

procedure TForm1.UpLoadDataClick(Sender: TObject);
var
  s1: String;
  s2: String;
  value: byte;
  n: Integer;
begin
  //---
  UpLoadData.Enabled := false;
  ledsToSilver();
  blueToothStatus := NOT_CONNECTED;
  //-------------------------------------------------------
  // look at radio buttons, construct the strings to upload to the dongle, and start to connect.
  //-------------------------------------------------------
  if RadioButton1.Checked = true then
    begin
    // connect to a Multitool
    s1 := multitoolServiceUUID;
    s2 := multitoolCharacteristicUUID;
    end;
  //-------------------------------------------------------
  if RadioButton2.Checked = true then
    begin
    // connect to a Sensor
    s1 := sensorServiceUUID + SensorIdString.Text;
    s2 := sensorCharacteristicUUID;
    end;
  //-------------------------------------------------------
  RS232_output(GATT_DATA_HEADER);
  for n := 0 to 15 do
    begin
      value := StrToInt('$' + s1[(n * 2) + 1] + s1[(n * 2) + 2]);
      RS232_output(value shr 4);
      RS232_output(value and $0F);
    end;
  for n := 0 to 15 do
    begin
      value := StrToInt('$' + s2[(n * 2) + 1] + s2[(n * 2) + 2]);
      RS232_output(value shr 4);
      RS232_output(value and $0F);
    end;
  //-------------------------------------------------------
  seq1 := SEQ1_SEND_LINK_RESPONSE_COMMAND;
  //-------------------------------------------------------
  ledsToSilver();
  //-------------------------------------------------------
end;

procedure TForm1.FileSelectComboChange(Sender: TObject);
begin
   Form1.ProgramButton.Enabled := true;
end;

procedure TForm1.ResetDongleClick(Sender: TObject);
begin
  RS232_output(REBOOT_DONGLE);
end;

procedure TForm1.BlueTimerTimer(Sender: TObject);
begin
  blueToothTimerFlag := 1;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
   analysis_of_hex_file_PIC18_64K();
end;


procedure TForm1.ResetDongleTimerTimer(Sender: TObject);
begin
  ResetDongleTimer.Enabled :=False;
  RS232_output(REBOOT_DONGLE);
end;

end.

