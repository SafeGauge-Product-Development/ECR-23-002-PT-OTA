unit ProgramUnit;

interface

uses
  SysUtils, WinTypes, WinProcs, Messages, Classes, Graphics, Controls,
  Forms, Dialogs, MMSystem, StdCtrls, ExtCtrls,
  Menus, ExtDlgs;

type
  TForm1 = class(TForm)
    Memo4: TMemo;
    Memo2: TMemo;
    ConvertFile: TButton;
    Memo1: TMemo;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    procedure ConvertFileClick(Sender: TObject);

  private

  public
	{ Public declarations }
  end;

var
  Form1: TForm1;
  data_bytes: array[0..255] of Integer;
  flash_space: array[0..1000000] of Longword;
  flash_space_written: array[0..1000000] of Integer;
  longAddress: Longword;
  data_bytes_log: array[0..255] of byte;
  user_space: array[0..15] of byte;
  user_space_written: array[0..15] of byte;
  configuration_space: array[0..15] of byte;
  configuration_space_written: array[0..15] of byte;
  ModifiedFileString: String;

implementation

{$R *.DFM}


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
      Form1.memo2.lines.add(s);
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
      Form1.memo1.lines.add(s);
      address := address + 16;
    end;
  //---
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



procedure AddDataToFile(block16:integer);
var
s: String;
n: Integer;
address: Integer;
checksum: Byte;
cs: String;
begin
   //---
   s := ':10';
   checksum := $10;
   //---
   address := (block16 * 16) and $00FF;
   checksum := checksum + address;
   //---
   address := ((block16 * 16) shr 8) and $00FF;
   checksum := checksum + address;
   //---
   s := s + IntToHex(block16 * 16, 4) + '00';
   checksum := checksum + 0;
   //---
   for n := 0 to 15 do s := s + IntToHex(flash_space[(block16 * 16) + n], 2);
   for n := 0 to 15 do checksum := checksum + flash_space[(block16 * 16) + n];
   checksum := (not checksum) + 1;
   cs := IntToHex(checksum, 2);
   s := s + cs;
   //---
   Form1.Memo4.Lines.Add(s);
   //---
end;


procedure isThereDataInParttialBlocks();
var
   n: Integer;
   m: Integer;
   partialBlockWritten: Integer;
begin
   //---
   for n := 0 to 4095 do
      begin
         partialBlockWritten := 0;
         for m := 0 to 15 do
            begin
               if flash_space_written[(n * 16) + m] = 1 then partialBlockWritten := 1;
            end;
         if partialBlockWritten = 1 then AddDataToFile(n);
      end;
end;

procedure analysis_of_hex_file_PIC18_64K_Modified();
var
   input_file: TextFile;
   s: String;
   lString: String;
   byte_count: Integer;
   record_type: Integer;
   n: Integer;
   m: Integer;
   data_point: Integer;
   extended_address: Longword;
   point: Integer;
   launchAddress: Integer;
   linesCount1: Integer;
   linesCount2: Integer;
   accumulate: Integer;
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
   AssignFile(input_file, 'C:\Users\scott\OneDrive - Safe Gauge\Desktop\OTA\BootLoader Rev1.X-20230919T231146Z-001\BootLoader Rev1.X\dist\default\production\BootLoader_Rev1.X.production.hex');
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
      displayFlashMemory();
      //-----------------------------------------------------------------------------------------------------
      // Bootloader modification code below
      //-----------------------------------------------------------------------------------------------------
      // This is the code mplab generated as viewed through the program memory window.
      // its origin is 0008 - ie the interrupt vector code for the bootloader, see "Original Bootloader interrupt code" .
      // this code is actually in the bootloader file this Delphi is analysing.

      // Note that if the bootloader code is modified we need to monitor this to make sure things are not getting screwed up.

      // we are going to move this code, ie the interrupt handler, so we will have two branches that can be used.
      // 1. The bootloader interrupt
      // 2. The normal interrupt, ie what is running when the sensoer is running "normally", ie not in bootloader mode.
      //
      // A File Register location in the PIC18 has been reserved in the Bootloader and normal code with the same address (0x000).
      // It is made persistent so the compiler can not zero this value on a hardware or watchdog reset.

      // __persistent unsigned char normalOrBoot __at(0x0000);  // this is located in data memory ie File Registers

      // When an interrupt is received the program counter vectors to address 0x0008.
      // As we now have two possible interrupt routines to run we need to test the File Register "normalOrBoot" and jump to the appropriate code.
      // Note that this File Register has a values BOOT = 0 , NORMAL = 1.

      // So we need to introduce the code below at memory location 0x0008 to jump to the appropriate code, ie "Interrupt Jump Code"

      //    50 00          MOVF $00,w        ; boots = 0 ; normal = 1
      //    E1 02          BNZ   normal
      //          boots:
      //    EF 13          GOTO 0x26         ; new address for the bootloader interrupt
      //    F0 00
      //          normal:
      //    EF 02          GOTO 0x4004       ; new address for the normal interrupt
      //    F0 20

      // Array of data to be written starting at address 0x0008 is as follows...

      // flash_space[8]  = $00
      // flash_space[9]  = $50
      // flash_space[10] = $02
      // flash_space[11] = $E1
      // flash_space[12] = $13
      // flash_space[13] = $EF
      // flash_space[14] = $00
      // flash_space[15] = $F0
      // flash_space[16] = $02
      // flash_space[17] = $EF
      // flash_space[18] = $20
      // flash_space[19] = $F0

      // note we need to move the original data first so the interrupt code we are about to move is not written over

      // Note that in the bootloader code we have reserved memory using the code...
      // const unsigned int constReserveMemory[64] __at(0x0026) = {0xFFFF, 0xFFFF, 0xFFFF,........
      // This reserves 128 bytes of code where the interrupt code will be relocated.
      //-----------------------------------------------------------------------------------------------------
      //  Original Bootloader interrupt code
      //-----------------------------------------------------------------------------------------------------
      //    [$0008]          8258    _my_isr   BSF btemp, 1, ACCESS
      //    [$000A]          CFD9              MOVFF FSR2, 0x2E
      //    [$000C]          F02E              NOP
      //    [$000E]          CFDA              MOVFF FSR2H, 0x2F
      //    [$0010]          F02F              NOP
      //    [$0012]          EC18              CALL 0x2E30, 0
      //    [$0014]          F017              NOP
      //    [$0016]          C02F              MOVFF 0x2F, FSR2H
      //    [$0018]          FFDA              NOP
      //    [$001A]          C02E              MOVFF 0x2E, FSR2
      //    [$001C]          FFD9              NOP
      //    [$001E]          9258              BCF btemp, 1, ACCESS
      //    [$0020]          0011              RETFIE 1

      // So we move the interrupt code first...

      for n := 0 to 25 do flash_space[n + $0026] := flash_space[n + $0008];

      // then we can load the new "Interrupt Jump Code"

      flash_space[8]  := $00;
      flash_space[9]  := $50;
      flash_space[10] := $02;
      flash_space[11] := $E1;
      flash_space[12] := $13;
      flash_space[13] := $EF;
      flash_space[14] := $00;
      flash_space[15] := $F0;
      flash_space[16] := $02;
      flash_space[17] := $EF;
      flash_space[18] := $20;
      flash_space[19] := $F0;

      //-----------------------------------------------------------------------------------------------------
      // Once we have manipulated the code
      // we create a new Modified Hex File,
      // this is the bootloader hex file we program the PIC18 chips with using the mplab IPE
      // Note that the "Convert File" button must be pressed to adjust and then re-write the hex file.
      //-----------------------------------------------------------------------------------------------------
      ModifiedFileString := '';
      Form1.Memo4.Clear();
      isThereDataInParttialBlocks();
      //---
      linesCount1 := 0;
      Reset(input_file);
      while not Eof(input_file) do
        begin
          Readln(input_file, lString);
          linesCount1 := linesCount1 + 1;
        end;
      //---
      linesCount2 := 0;
      Reset(input_file);
      while not Eof(input_file) do
        begin
          Readln(input_file, lString);
          if linesCount2 = (linesCount1 - 5)  then Form1.Memo4.Lines.Add(lString);
          if linesCount2 = (linesCount1 - 4)  then Form1.Memo4.Lines.Add(lString);
          if linesCount2 = (linesCount1 - 3)  then Form1.Memo4.Lines.Add(lString);
          if linesCount2 = (linesCount1 - 2)  then Form1.Memo4.Lines.Add(lString);
          if linesCount2 = (linesCount1 - 1)  then Form1.Memo4.Lines.Add(lString);
          //---
          linesCount2 := linesCount2 + 1;
          //---
        end;
      //---
      Form1.Memo4.Lines.SaveToFile('C:\Users\scott\OneDrive - Safe Gauge\Desktop\OTA\BootLoader Rev1.X-20230919T231146Z-001\BootLoader Rev1.X\dist\default\production\BootLoader_Rev1.X.production.Modified.hex');
      //---
      displayFlashMemory2();
      //---
      accumulate := 0;
      for n := 0 to $3FFF do
         begin
            accumulate := accumulate + flash_space[n];
         end;
      //-----------------------------------------------------------------------------------------------------
end;

procedure TForm1.ConvertFileClick(Sender: TObject);
begin
  analysis_of_hex_file_PIC18_64K_Modified();
end;

end.

