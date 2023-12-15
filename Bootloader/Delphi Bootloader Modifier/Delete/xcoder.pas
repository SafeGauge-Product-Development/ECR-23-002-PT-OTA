unit xcoder;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, MMSystem, StdCtrls, ExtCtrls,
  Menus, VaClasses, VaComm;

type
  TForm1 = class(TForm)
    ProgramButton: TButton;
    Memo2: TMemo;
    Timer1: TTimer;
    VaComm1: TVaComm;
    linkStatusLed: TShape;
    blueToothStatusLed: TShape;
    StaticText1: TStaticText;
    StaticText2: TStaticText;
	procedure FormCreate(Sender: TObject);
  procedure ProgramButtonClick(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure VaComm1RxChar(Sender: TObject; Count: Integer);
  private

  public
	{ Public declarations }
  end;
const
  //---------------------------------------------------------------
  // Sequence of stages
  //---------------------------------------------------------------
  SEQ_START =                                         0;
  SEQ_COMMS_SETUP =                                   1;
  SEQ_ANALYSE_COMMS_RESPONSE =                        2;
  SEQ_SEND_LINK_RESPONSE_COMMAND =                    3;
  SEQ_WAITING_FOR_LINK_RESPONSE =                     4;
  SEQ_WAITING_FOR_HH_RESPONSE =                       5;
  SEQ_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS =  7;
  SEQ_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED =             8;
  SEQ_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE =         9;
  SEQ_RECEIVED_FLASH_BLOCK_DATA_RESPONSE =            10;
  SEQ_FINISHED =                                      11;
  SEQ_WRITE_EEPROM =                                  12;
  SEQ_WRITE_EEPROM_RESPONSE_WAIT =                    13;
  SEQ_WRITE_EEPROM_RESPONSE_GOOD =                    14;
  SEQ_PROBLEM =                                       15;
  SEQ_END =                                           16;
  //---------------------------------------------------------------
  // 256K Device
  //---------------------------------------------------------------
  //CONST_FLASH_SPACE = 175103;
  //CONST_NUM_BLOCKS = 170;
  //CONST_DATA_BYTE_SPACE = 527705;
  //---------------------------------------------------------------
  // 1024K Device
  //---------------------------------------------------------------
  CONST_FLASH_SPACE = 704511;       // single partition full space
  CONST_NUM_BLOCKS = 171;           //343
  CONST_DATA_BYTE_SPACE = 2123167;
  //---------------------------------------------------------------
  PKT_IS_HH_READY =              $F0;
  PKT_HH_RESPONSE =              $F1;
  PKT_BLOCK_PROGRAM_INFO =       $F2;
  PKT_BLOCK_DATA =               $F3;
  PKT_PROGRAMMING_DONE =         $F4;
  PKT_PROGRAM_EEPROM =           $F5;
  PKT_PROGRAM_EEPROM_RESPONSE =  $F6;
  //---------------------------------------------------------------
  START_HEADER =                      $80;
  STOP_HEADER =                       $81;
  TEST_CONNECTION_HEADER =            $82;
  NO_CONNECTION_HEADER =              $83;
  CONNECTION_ESTABLISHED_HEADER =     $84;
  //---------------------------------------------------------------
  NOT_CONNECTED =                     0;
  CONNECTED =                         1;
  //---------------------------------------------------------------

type
  //dataArray = array [0..1535] of integer;
  dataArray = array [0..3071] of integer;
  writeMatrix = array[0..CONST_NUM_BLOCKS] of dataArray;

var
  Form1: TForm1;
  writeSpaceMatrix: writeMatrix;
  checksum1: array[0..CONST_NUM_BLOCKS] of Byte;
  checksum2: array[0..CONST_NUM_BLOCKS] of Byte;
  checksum3: array[0..CONST_NUM_BLOCKS] of Byte;
  checksum4: array[0..CONST_NUM_BLOCKS] of Byte;
  root_directory: String;
  data1: String;
  data2: String;
  OutFile1: TextFile;
  OutFile2: TextFile;
  OutFile3: TextFile;
  data_bytes_in_string: array[0..255] of String;
  data_bytes: array[0..255] of Integer;
  flash_space: array[0..CONST_FLASH_SPACE] of Longword;
  flash_space_written: array[0..CONST_FLASH_SPACE] of Integer;
  flash_blocks_written: array[0..CONST_NUM_BLOCKS] of Integer;
  security: array[0..255] of byte;
  seq: Integer;
  packet: array[0..19] of Byte;
  seconds: Integer;
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
  osRevision: Integer;

  numbers: array[0..15] of String;

implementation

{$R *.DFM}

  //---------------------------------------------------------------
  // PKT_IS_HH_READY : Delphi->HH
  // Is the HH in Programming mode? - a timeout will be needed
  //---
  // data[0] = $F0
  // data[1] = osRevision, this is "0" for the first OS
  // data[2..19] = 0 , ie filler
  //---------------------------------------------------------------
  // PKT_HH_RESPONSE : HH->Delphi
  // If the HH is ready it sends this packet
  //----
  // data[0] = $F1
  // data[1..19] = 0 , ie filler
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
            RS232_output(packet[n] shr 4);    // most significant nibble
            RS232_output(packet[n] and $0F);  // least significant nibble
         end;
   RS232_output($81);
end;

procedure create_array_of_data(block: Integer);
var
Sysex: string;
n: Integer;
checksum: Integer;
//data: array[0..1023] of Integer;
data: array[0..2047] of Integer;
data_A: Integer;
data_B: Integer;
begin
   //---
   //for n := 0 to 1023 do data[n] := flash_space[n + (block * 1024)];
   for n := 0 to 2047 do data[n] := flash_space[n + (block * 2048)];
   for n := 0 to 1023 do   // 511
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
   for n := 0 to 3071 do writeSpaceMatrix[block][n] := security[writeSpaceMatrix[block][n]];
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

Function HexConvert(c: Char) : Integer;
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

procedure analysis_of_hex_file();
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
   AssignFile(input_file, 'MainB.hex');
   //AssignFile(input_file, 'C:\Users\Indus\MPLABXProjects\HandHeld Rev1.03 21st May 2020\HandHeld.X\dist\default\production\HandHeld.X.production.hex');
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
                     //if address = $401800 then Form1.memo5.Lines.Add('FBOOT detected & ignored');
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
      for n := 0 to CONST_NUM_BLOCKS do
      begin
         flash_blocks_written[n] := 0;

         //for m := 0 to 1023 do
         //   begin
         //      if flash_space_written[(n * 1024) + m] = 1 then flash_blocks_written[n] := 1;
         //   end;

         for m := 0 to 2048 do
            begin
               if flash_space_written[(n * 2048) + m] = 1 then flash_blocks_written[n] := 1;
            end;

         //if flash_blocks_written[n] = 1 then Form1.Memo2.Lines.Add(IntToStr(n));
      end;
      //-----------------------------------------------------------------------------------------------------
      // Process the data_bytes_space so it can be written to the file
      //-----------------------------------------------------------------------------------------------------
      //for n := 0 to CONST_DATA_BYTE_SPACE do data_bytes_space[n] := 69; // clear the entire space
      for n := 0 to CONST_NUM_BLOCKS do
         begin
            if flash_blocks_written[n] = 1 then create_array_of_data(n);
         end;
      //-----------------------------------------------------------------------------------------------------
      //Form1.Memo2.Lines.Add('Done!!!');
      //-----------------------------------------------------------------------------------------------------

      //====================================
      blk := 171;
      //Form1.Memo5.Lines.Add('Block = ' + IntToStr(flash_blocks_written[blk]));
      for m := 0 to 255 do
         begin
            sn := '';
            for n := 0 to 11 do
               begin
                  sn := sn + ' ' + IntToHex(writeSpaceMatrix[blk][n + (m * 12)], 2);
               end;
            //Form1.Memo5.Lines.Add(IntToSTr(m) + '    Address = ' + IntToHex($400000 + (blk * 2048) + (m * 8), 4) + '    ' + sn);
         end;
      //====================================
      //Form1.Memo5.Lines.Add(IntToSTr(writeSpaceMatrix[blk][384]));












      // Summary at this point...

      // flash_blocks_written[0..CONST_NUM_BLOCKS] if = 1 then writeSpaceMatrix data needs to be written
      // writeSpaceMatrix[0..CONST_NUM_BLOCKS][0..1535] data to written over Bluetooth
      // checksum1,2,3,4[0..CONST_NUM_BLOCKS] checksum data needed to be written






      //-----------------------------------------------------------------------------------------------------
      // may need the below later...
      //-----------------------------------------------------------------------------------------------------
      // Write to the final output file an array of the blocks that need to be sent, ie blocks with data - FOR DELPHI
      //-----------------------------------------------------------------------------------------------------
      //GetDir(0, root_directory);
      //AssignFile(OutFile1, root_directory + '\' + 'delphi_sysex_' + Form1.RevisionId.Text + '.txt');
      //Rewrite(OutFile1);
      //---
      //Writeln(OutFile1, 'sysex_blocks_' + Form1.RevisionId.Text + ': array[0..687] of Integer = ');  // Set Revision Code here
      //Writeln(OutFile1, '(');
      //---
      //for n := 0 to (CONST_NUM_BLOCKS - 1) do Writeln(OutFile1, IntToStr(flash_blocks_written[n]) + ',');
      //Writeln(OutFile1, IntToStr(flash_blocks_written[CONST_NUM_BLOCKS]));
      //---
      //Writeln(OutFile1, ');');
      //---
      //Writeln(OutFile1, '');
      //-----------------------------------------------------------------------------------------------------
      // Write to the final output file an array of the blocks of data to be sent - FOR DELPHI
      //-----------------------------------------------------------------------------------------------------
      //Writeln(OutFile1, 'sysex_data_' + Form1.RevisionId.Text + ': array[0..2123167] of Integer = ');  // Set Revision Code here
      //Writeln(OutFile1, '(');
      //---
      //for n := 0 to (CONST_DATA_BYTE_SPACE - 1) do Writeln(OutFile1, IntToStr(data_bytes_space[n]) + ',')
      //Writeln(OutFile1, IntToStr(data_bytes_space[CONST_DATA_BYTE_SPACE]));
      //---
      //Writeln(OutFile1, ');');
      //---
      //CloseFile(OutFile1);
      //---
      //Form1.Memo1.Lines.Add('Finished ' + Form1.RevisionId.Text + ' Delphi');
      //-----------------------------------------------------------------------------------------------------
end;
{
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
begin
   RS232_output($80);
      for n := 0 to 19 do
         begin
            RS232_output(packet[n] shr 4);    // most significant nibble
            RS232_output(packet[n] and $0F);  // least significant nibble
         end;
   RS232_output($81);
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
   VaComm1.PortNum := 4;
   VaComm1.Baudrate := br57600;
   VaComm1.Open;
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
   for n := 0 to 19 do packet[n] := n;
   sendPacket();
end;

procedure TForm1.VaComm1RxChar(Sender: TObject; Count: Integer);
var
   i: Integer;
   data: string;
begin
   data := VaComm1.ReadText;
   for i := 1 to Length(data) do
      begin
         Memo1.Lines.Add(IntToStr(ord(data[i])));
      end;
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
   Memo1.Clear;
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
   RS232_output($82);
end;
}

procedure DataBlockSend(block: Integer);
var
n: Integer;
begin
   //---------------------------------------------
   //Form1.Memo4.Lines.add(IntToStr(block));
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
   for n := 7 to 19 do packet[n] := 0;
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
   //---
   packetId := 170;
   packet[0] := PKT_BLOCK_DATA;
   packet[1] := packetId;
   for n := 0 to 11 do packet[n + 2] := writeSpaceMatrix[block][n + (packetId * 18)];
   for n := 12 to 17 do packet[n + 2] := 0;
   sendPacket();
   //---------------------------------------------
end;

procedure TForm1.ProgramButtonClick(Sender: TObject);
var
n: Integer;
begin
   //---
   analysis_of_hex_file();
   ProgramButton.Enabled := false;
   seq := SEQ_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
   blockNumber := 0;
   //---
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
n: Integer;
i: Integer;
begin
  case seq of
      //------------------------------------------------------------
      SEQ_START:
         begin
            Try
               VaComm1.Open;
               commsStatus := CONNECTED;
            Except
               commsStatus := NOT_CONNECTED;
            end;
            seq := SEQ_ANALYSE_COMMS_RESPONSE;
         end;
      //------------------------------------------------------------
      SEQ_ANALYSE_COMMS_RESPONSE:
         begin
            if commsStatus = CONNECTED then
               begin
                  seq := SEQ_SEND_LINK_RESPONSE_COMMAND;
               end
            else
               begin
                  seq := SEQ_START;
               end;
         end;
      //------------------------------------------------------------
      SEQ_SEND_LINK_RESPONSE_COMMAND:
         begin
            RS232_output(TEST_CONNECTION_HEADER);
            seq := SEQ_WAITING_FOR_LINK_RESPONSE;
         end;
      //------------------------------------------------------------
      SEQ_WAITING_FOR_LINK_RESPONSE:
         begin
            //---
            if linkStatus = CONNECTED then
               linkStatusLed.Brush.Color := clLime
            else
               linkStatusLed.Brush.Color := clGray;
            //---
            if blueToothStatus = CONNECTED then
               blueToothStatusLed.Brush.Color := clAqua
            else
               blueToothStatusLed.Brush.Color := clGray;
            //---
            if (linkStatus = CONNECTED) and (blueToothStatus = CONNECTED) then
               begin
                  seq := SEQ_WAITING_FOR_HH_RESPONSE;
                  //---
                  packet[0] := PKT_IS_HH_READY;
                  for n := 1 to 19 do packet[n] := 0;
                  sendPacket();
                  //---
               end
            else
               begin
                  seq := SEQ_SEND_LINK_RESPONSE_COMMAND;
               end;
            //---
         end;
      //------------------------------------------------------------
      SEQ_WAITING_FOR_HH_RESPONSE:
         begin
            // wait here for HH response
            ProgramButton.Enabled := true;
            seq := SEQ_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS;
         end;
      //------------------------------------------------------------
      SEQ_WAITING_FOR_TRANSFER_BLOCK_DATA_BUTTON_PRESS:
         begin
            // waiting for button press to start loading code
         end;
      //------------------------------------------------------------
      SEQ_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED:
         begin
            if blockNumber = (CONST_NUM_BLOCKS + 1) then
               begin
                  //-----------------------------------------
                  // We have exhausted all the blocks
                  //-----------------------------------------
                  seq := SEQ_FINISHED;
                  //-----------------------------------------
               end
            else
               begin
                  //-----------------------------------------
                  // keep looking for blocks of data
                  //-----------------------------------------
                  for n := blockNumber to CONST_NUM_BLOCKS do
                     begin
                        if flash_blocks_written[n] = 1 then break;
                     end;
                  Form1.Memo2.Lines.Add('Programming Block... ' + IntToStr(n));
                  DataBlockSend(n);
                  seq := SEQ_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE;
                  blockNumber := n + 1;
                  //-----------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ_WAITING_FOR_FLASH_BLOCK_DATA_RESPONSE:
         begin
            //Form1.Memo2.Lines.Add('Here');
         end;
      //------------------------------------------------------------
      SEQ_RECEIVED_FLASH_BLOCK_DATA_RESPONSE:
         begin
            // status for transferred block data integrity... packetBytes[1] = data , packetBytes[2] = flash verification
            if ((packetBytes[1] = 1) and (packetBytes[2] = 1)) then
               begin
                  //---------------------------------------------
                  // Data is good
                  //---------------------------------------------
                  seq := SEQ_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                  numberOfWriteAttempts := 0;
                  //---------------------------------------------
               end
            else
               begin
                  //---------------------------------------------
                  // this is an exception for the last block as the verification always fails due to it being in the flash configuration section
                  //---------------------------------------------
                  if blockNumber = (CONST_NUM_BLOCKS + 1) then // configuration section, we don't test for errors
                     begin
                        if packetBytes[1] = 1 then
                           begin
                              seq := SEQ_WRITE_EEPROM;
                              numberOfWriteAttempts := 0;
                           end
                        else
                           begin
                              seq := SEQ_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                              numberOfWriteAttempts := numberOfWriteAttempts + 1;
                              Form1.Memo2.Lines.Add('Programming Attempts... ' + IntToStr(numberOfWriteAttempts));
                              blockNumber := blockNumber - 1;
                              if numberOfWriteAttempts > 2 then seq := SEQ_PROBLEM;
                           end;
                     end
                  else
                     begin
                        //---------------------------------------------
                        // Data is bad, lets try again
                        //---------------------------------------------
                        seq := SEQ_SEND_BLOCKS_OF_DATA_TO_BE_FLASHED;
                        numberOfWriteAttempts := numberOfWriteAttempts + 1;
                        Form1.Memo2.Lines.Add('Programming Attempts... ' + IntToStr(numberOfWriteAttempts));
                        blockNumber := blockNumber - 1;
                        if numberOfWriteAttempts > 2 then seq := SEQ_PROBLEM;
                        //---------------------------------------------
                     end;
                  //---------------------------------------------
               end;
         end;
      //------------------------------------------------------------
      SEQ_FINISHED:
         begin
            seq := SEQ_WRITE_EEPROM;
         end;
      //------------------------------------------------------------
      SEQ_WRITE_EEPROM:
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
            seq := SEQ_WRITE_EEPROM_RESPONSE_WAIT;
            //---------------------------------------------
         end;
      //------------------------------------------------------------
      SEQ_WRITE_EEPROM_RESPONSE_WAIT:
         begin
         end;
      //------------------------------------------------------------
      SEQ_WRITE_EEPROM_RESPONSE_GOOD:
         begin
            Form1.Memo2.Lines.Add('Finished');
            Form1.Memo2.Lines.Add('Success!');
            seq := SEQ_END;
         end;
      //------------------------------------------------------------
      SEQ_PROBLEM:
         begin
            Form1.Memo2.Lines.Add('There is a problem!');
            Form1.Memo2.Lines.Add('Reboot Windows App...');
            Form1.Memo2.Lines.Add('Reboot Handheld...');
            Form1.Memo2.Lines.Add('Try again...');
            seq := SEQ_END;
         end;
      //------------------------------------------------------------
      SEQ_END:
         begin
         end;
      //------------------------------------------------------------
   end;
   //---
   seconds := seconds + 1;
   //---
end;

procedure TForm1.VaComm1RxChar(Sender: TObject; Count: Integer);
var
   i: Integer;
   s: string;
   data: Byte;
   N: Integer;
begin
   s := VaComm1.ReadText;
   for i := 1 to Length(s) do
      begin
         //---
         data := ord(s[i]);
         //memo3.Lines.add(intToStr(data));
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
                  CONNECTION_ESTABLISHED_HEADER:
                     begin
                        linkStatus := CONNECTED;
                        blueToothStatus := CONNECTED;
                     end;
                  //------------------------------------------------------------
                  NO_CONNECTION_HEADER:
                     begin
                        linkStatus := CONNECTED;
                        blueToothStatus := NOT_CONNECTED;
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
                           PKT_HH_RESPONSE:
                              begin
                                 osRevision := packetBytes[1];
                                 seq := SEQ_WAITING_FOR_HH_RESPONSE;
                              end;
                           //------------------------------------------------------------
                           PKT_PROGRAMMING_DONE:
                              begin
                                 seq := SEQ_RECEIVED_FLASH_BLOCK_DATA_RESPONSE;
                              end;
                           //------------------------------------------------------------
                           PKT_PROGRAM_EEPROM_RESPONSE:
                              begin
                                 if (packetBytes[1] = 0) and (packetBytes[2] = ord('L')) and (packetBytes[3] = ord('u')) and (packetBytes[4] = ord('k')) and(packetBytes[5] = ord('e')) then
                                    begin
                                       seq := SEQ_WRITE_EEPROM_RESPONSE_GOOD;
                                    end
                                 else
                                    begin
                                       seq := SEQ_PROBLEM;
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
               end;
            end;
      end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
   //----------------------------------------------------------------------
   seq := SEQ_START;
   seconds := 0;
   linkStatus := NOT_CONNECTED;
   linkStatusLed.Brush.Color := clGray;
   blueToothStatusLed.Brush.Color := clGray;
   commsStatus := NOT_CONNECTED;
   VaComm1.PortNum := 128;
   VaComm1.Baudrate := br57600;
   blueToothStatus := NOT_CONNECTED;
   numberOfWriteAttempts := 0;
   osRevision := $FFFF; // note only a number between 0 and 255 is valid, $FFFF tells us we have not received an OS number yet
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
end;

end.

