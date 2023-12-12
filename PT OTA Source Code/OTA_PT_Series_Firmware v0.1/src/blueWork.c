//---------------------------------------------
#include <xc.h>
#include "apitypes.h"
#include "blueStates.h"
#include "const.h"
//---------------------------------------------
struct ble_header
{
    uint8  type_hilen;
    uint8  lolen;
    uint8  cls;
    uint8  command;
};
//---------------------------------------------
void ledColour(unsigned char colour);
//---------------------------------------------
volatile extern uint8            blueState;
volatile extern uint16           blueTimer;
volatile extern uint8            bleReady;
volatile extern uint8            calibrationPointsLSB[8];
volatile extern uint8            calibrationPointsMSB[8];
extern unsigned char             ads1120Config0;
extern unsigned char             ads1120Config1;
extern unsigned char             ads1120Config2;
extern unsigned char             ads1120Config3;
extern volatile uint16           solidColourTimer;
extern volatile uint8            solidColour;
extern volatile uint16           periodTimer;
extern volatile uint8            periodFlag;
extern volatile uint8            batteryVoltage;

extern __persistent uint16       batteryTimeCharging;
extern __persistent uint16       batteryTimeSleeping;
extern __persistent uint16       batteryTimeRunning;
//---------------------------------------------
uint8 dataSet[20]  = 
{
   0, 1, 2, 3, 4, 5, 6, 7,
   8, 9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19
};
uint8                            payload[256];  //holds payload of message
uint8                            datasIn[20];
uint8                            datasOut[20];
uint8                            bleJustReset = YES;
//--------------------------------------------
extern uint8                     sensorLsb;
extern uint8                     sensorMsb;
extern uint8                     autoCalibrationLsb;
extern uint8                     autoCalibrationMsb;
const uint8                      calibrationSecurityCheck[20] = 
{
   0, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 98, 99, 100, 101, 102, 103, 102 ,100, 99, 98, 0xFF
};
//---------------------------------------------------
void delay(void)
{
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();  
}
//---------------------------------------------------
void shootSoftSpiData8(unsigned char data)
{
   unsigned char n;
   unsigned char shift;
   
   shift = 0b10000000;
   for (n = 0 ; n < 8 ; n++)
   {
      delay();
      if ((data & shift) == shift)
         S_DIN = 1;
      else
         S_DIN = 0;
     delay();
     S_SCLK = 1;
     shift = (unsigned char)(shift >> 1);
     delay();
     S_SCLK = 0;
     delay();
   }
   delay();
   S_DIN = 0; // leave data out line low
   delay();
}
//--------------------------------------------
void waitForDrdy(void)
{
   while (S_DRDY == 1)
   {
   }
}
//--------------------------------------------
unsigned char readSoftSpiData8(void)
{
   unsigned char n, build, shift;
   
   delay();
   S_DIN = 0; // set data out line low while reading data
   build = 0;
   shift = 0b10000000;
   for (n = 0 ; n < 8 ; n++)
   {
      delay();
      S_SCLK = 1;
      delay();
      if (S_DOUT == 1)
         build = (unsigned char)(build | shift);
      shift = (unsigned char)(shift >> 1);
      delay();
      S_SCLK = 0;
      delay();
   }
   //---
   return build;
   //---
}
//--------------------------------------------
void readDataSensor(unsigned char data)
{
   unsigned char msb, lsb;

   //---
   delay();
   S_CS = 0;
   delay();
   shootSoftSpiData8(data);
   waitForDrdy();
   sensorMsb = readSoftSpiData8();
   sensorLsb = readSoftSpiData8(); 
   delay();     
   S_CS = 1;
   delay();
   //---
}
//--------------------------------------------
void setupADS1120(void)
{
   //---
   unsigned int d;
   //---
   S_DIN = 0;
   S_CS = 0;
   //---
   shootSoftSpiData8(ADC_RESET);
   //---
   for (d = 0 ; d < 500 ; d++)
   {
   }
   //---
   shootSoftSpiData8(ADC_WREG | 0b00000011); // we are about to write for bytes into the 4 registers
   shootSoftSpiData8(ADS1120_CONFIG_0);
   shootSoftSpiData8(ADS1120_CONFIG_1);
   shootSoftSpiData8(ADS1120_CONFIG_2);
   shootSoftSpiData8(ADS1120_CONFIG_3);
   //---
   S_CS = 1;
   //---
}
//--------------------------------------------
void readAllADS1120configRegisters(void)
{
   unsigned int d;

   //---
   S_DIN = 0;
   S_CS = 0;
   //---
   for (d = 0 ; d < 500 ; d++)
   {
   }
   //---
   shootSoftSpiData8(RREG | 0b00000011);
   //---
   ads1120Config0 = readSoftSpiData8();
   ads1120Config1 = readSoftSpiData8();
   ads1120Config2 = readSoftSpiData8();
   ads1120Config3 = readSoftSpiData8();
   //---
   S_CS = 1;
   //---
}
//--------------------------------------------
void ledColour(unsigned char colour)
{
   //---
   if ((colour & RED) == RED)
      LATBbits.LATB1 = 1;
   else
      LATBbits.LATB1 = 0;
   //---
   if ((colour & GREEN) == GREEN)
      LATBbits.LATB2 = 1;
   else
      LATBbits.LATB2 = 0;
   //---
   if ((colour & BLUE) == BLUE)
      LATBbits.LATB3 = 1;
   else
      LATBbits.LATB3 = 0;
   //---
}
//---------------------------------------------------
uint8 ReadEEByte(uint16 address)
{
   //---
   NVMCON1bits.NVMREG0 = 0;
   NVMCON1bits.NVMREG1 = 0;
   //---
   NVMADRL = (uint8)(address & 0x00FF);
   NVMADRH = (uint8)((address >> 2) & 0x0003);
   //---
   NVMCON1bits.RD = 1;
   //---
   return NVMDAT;
   //--- 
}
//--------------------------------------------
void WriteEEByte(uint16 address, uint8 data)
{
   //---
   NVMCON1bits.NVMREG0 = 0;
   NVMCON1bits.NVMREG1 = 0;
   //---
   NVMADRL = (uint8)(address & 0x00FF);
   NVMADRH = (uint8)((address >> 2) & 0x0003);
   //---
   NVMDAT = data;
   //---
   NVMCON1bits.WREN = 1;
   //---
   INTCONbits.GIE = 0;
   //---
   NVMCON2 = 0x55;
   NVMCON2 = 0xAA;
   //---
   NVMCON1bits.WR = 1;
   while (NVMCON1bits.WR == 1)
   {
   }
   //---
   INTCONbits.GIE = 1;
   //---
   NVMCON1bits.WREN = 0;
   //---
}
//--------------------------------------------
void readInCalibrationPoints(void)
{
   unsigned int n;
   
   for (n = 0 ; n < 8 ; n++)
   {
      calibrationPointsLSB[n] = ReadEEByte(n);
      calibrationPointsMSB[n] = ReadEEByte(n + 8);
   }
}
//--------------------------------------------
void bleReset(unsigned char reset)
{
   if (reset == 1)
      LATDbits.LATD2 = 1;
   else
      LATDbits.LATD2 = 0;
}
//--------------------------------------------
void mcuRtsHigh(void)
{
   LATCbits.LATC4 = 1;
}
//--------------------------------------------
void mcuRtsLow(void)
{
   LATCbits.LATC4 = 0;
}
//--------------------------------------------
unsigned char mcuCts(void)
{
   if ((PORTC & 0b00100000) == 0b00100000)
      return YES;
   else
      return NO;
}
//--------------------------------------------
void shootUart1Data(unsigned char data)
{
   //--
   while(TXSTA1bits.TRMT == 0)
   {
      ClrWdt();
   }
   while(mcuCts() == YES)   
   {
     ClrWdt();
   }
   //--
   TXREG1 = data;
   //---
}
//---------------------------------------------
void calibrationStow(unsigned char id)
{
   ledColour(BLACK);
   WriteEEByte((uint16)(id), sensorLsb);
   WriteEEByte((uint16)(id + 8), (uint8)sensorMsb);
   calibrationPointsLSB[id] = sensorLsb;
   calibrationPointsMSB[id] = sensorMsb;
   blueTimer = 0;
   solidColourTimer = 100; // 5 seconds
   solidColour = id;
}
//---------------------------------------------
uint8 messageMatch(uint8 hilen, uint8 cls, uint8 command)
{
   uint8 result;
   uint8 n;
   
   //---
   result = BLE_NO_MATCH;
   //---
   //ble_rsp_attributes_write ,0x00, 0x02, 0x00
   if ((hilen == 0x00) && (cls == 0x02) && (command == 0x00))
   {
      result = BLE_RSP_ATTRIBUTES_WRITE;
      bleReady = READY;
      return result;
   }
   //---
   //ble_rsp_gap_set_mode
   if ((hilen == 0x00) && (cls == 0x06) && (command == 0x01))
   {
      result = BLE_RSP_GAP_SET_MODE;
      blueState = BLE_WAITING_FOR_CONNECTION;
      bleReady = READY;
      blueTimer = 0;
      return result;
   }
   //---
   //ble_evt_system_protocol_error ,0x80,0x00,0x06
   if ((hilen == 0x80) && (cls == 0x00) && (command == 0x06))  
   {
      result = BLE_EVT_SYSTEM_PROTOCOL_ERROR;
      bleReady = READY;
      return result;
   }
   //---
   //ble_evt_system_boot ,0x80,0x00,0x00
   if ((hilen == 0x80) && (cls == 0x00) && (command == 0x00)) 
   {
      result = BLE_EVT_SYSTEM_BOOT;
      blueState = BLE_START_ADVERTISING;
      return result;
   }
   //---
   //ble_evt_attributes_value ,0x80,0x02,0x00
   if ((hilen == 0x80) && (cls == 0x02) && (command == 0x00)) 
   {
      //------------------------------------
      result = BLE_EVT_ATTRIBUTES_VALUE;
      for (n = 0 ; n < 20 ; n++)
         datasIn[n] = payload[(uint16)(7 + n)];
      //------------------------------------
      for (n = 1 ; n < 20 ; n++)
      {
         if (datasIn[n] != calibrationSecurityCheck[n])
            return BLE_NO_MATCH;    
      }
      //------------------------------------
      switch (datasIn[0])
      {
         //------------------------------------
         case 0xF1:
         {
            // Store Calibration Point 1 in EEPROM
            calibrationStow(0);
            break;  
         }
         //------------------------------------
         case 0xF2:
         {
            // Store Calibration Point 2 in EEPROM
            calibrationStow(1);
            break;  
         }
         //------------------------------------
         case 0xF3:
         {
            // Store Calibration Point 3 in EEPROM
            calibrationStow(2);
            break;  
         }
         //------------------------------------
         case 0xF4:
         {
            // Store Calibration Point 4 in EEPROM
            calibrationStow(3);
            break;  
         }
         //------------------------------------
         case 0xF5:
         {
            // Store Calibration Point 5 in EEPROM
            calibrationStow(4);
            break;  
         }
         //------------------------------------
         case 0xF6:
         {
            // Store Calibration Point 6 in EEPROM
            calibrationStow(5);
            break;  
         }
         //------------------------------------
         case 0xF7:
         {
            // Store Calibration Point 7 in EEPROM
            calibrationStow(6);
            break;  
         }
         //------------------------------------
         case 0xF8:
         {
            // Store Calibration Point 8 in EEPROM
            calibrationStow(7);
            break;  
         }
         //------------------------------------
         default:
         {  
            break;  
         }
         //------------------------------------       
      }
      return result;
      //------------------------------------
   }
   //---
   //ble_evt_attributes_status ,0x80,0x02,0x02
   if ((hilen == 0x80) && (cls == 0x02) && (command == 0x02))
   {
      result = BLE_EVT_ATTRIBUTES_STATUS;
      blueState = BLE_IS_CONNECTED;
      return result;
   }
   //---
   //ble_evt_connection_status ,0x80,0x03,0x00
   if ((hilen == 0x80) && (cls == 0x03) && (command == 0x00))   
   {
      result = BLE_EVT_CONNECTION_STATUS;
      blueState = BLE_WAITING_NOTIFICATION_REPLY;
      blueTimer = 0;
      return result;
   }
   //---
   //ble_evt_connection_disconnected ,0x80,0x03,0x04
   if ((hilen == 0x80) && (cls == 0x03) && (command == 0x04)) 
   {
      result = BLE_EVT_CONNECTION_DISCONNECTED;
      blueState = BLE_DISCONNECTED;
      return result;
   }
   //---
   return result;
   //---
}    
//---------------------------------------------
void XReceiveBTMessage (uint8 BTdata)
{
    static uint16       byteCount = 0;
    static struct       ble_header  BTHeader;      //holds header of message
    static uint16       pLength;
    static uint8        receivedMessage;
   
   //--------------------------------------------
   // this removes unwanted "0" at power up
   //--------------------------------------------
   if (bleJustReset == YES)
   {
      if (BTdata == 0x80)
         bleJustReset = NO;
      else
         return;
   }
   //--- 1st BYTE ---
   if (byteCount == 0)
   {
      BTHeader.type_hilen = BTdata;
      ++byteCount;
      return;
   }
   //--- 2nd BYTE ---
   if (byteCount == 1)
   {
      BTHeader.lolen = BTdata;   
      ++byteCount;
      return;
   }   
   //--- 3rd BYTE ---
   if (byteCount == 2)
   {
      BTHeader.cls = BTdata; 
      ++byteCount;
      return;
   }
   //--- 4th BYTE ---
   if (byteCount == 3)
   {
      //---
      BTHeader.command = BTdata;
      //---
      // Calculate data length
      //---
      pLength = (uint16)BTHeader.lolen;
      //---
      if (pLength == 0) // we have the entire message now as there are no data bytes
      {
         //---
         receivedMessage = messageMatch(BTHeader.type_hilen, BTHeader.cls, BTHeader.command);
         byteCount = 0x00;
         //---
      }
      else
         ++byteCount;
      return;
   }
   //--- PAYLOAD BYTES ---
   if (byteCount >= 4)
   {
      // this is an acceptable packet length
      if (byteCount == (pLength + 3))
      {
         //---
         // we have received all the data bytes now
         payload[byteCount - 4] = BTdata;
         //---
         receivedMessage = messageMatch(BTHeader.type_hilen, BTHeader.cls, BTHeader.command);
         byteCount = 0x00;
         return;
         //---
      }
      else
      {
         //---
         // still loading in data
         //---
         payload[byteCount - 4] = BTdata;
         //++byteCount;
         if (++byteCount >= 255)
            byteCount = 255;
         return;
         //---
      }
   }
}
//---------------------------------------------
void Xble_cmd_attributes_write(void)
{
   uint8 n;
   
   shootUart1Data(0);      
   shootUart1Data(24);     
   shootUart1Data(2);      
   shootUart1Data(0);      
   
   shootUart1Data(XDATA);  // handle low endian byte
   shootUart1Data(0);      // handle high endian byte
   shootUart1Data(0);      // data offset
   shootUart1Data(20);      // data bytes to send
   
   for (n = 0 ; n < 20 ; n++) // dataSet bytes
      shootUart1Data(datasOut[n]);
}
//---------------------------------------------
void Xble_cmd_gap_set_mode(void)
{
   shootUart1Data(0);
   shootUart1Data(2);
   shootUart1Data(6);
   shootUart1Data(1);
   shootUart1Data(2);
   shootUart1Data(2);
}
//---------------------------------------------
void XBLE_state_manage(void)
{ 
   unsigned char n;
   static uint8 value = 0;
   static uint8 countId = 0;
   static uint16 dataId = 0;
   static uint8 historyLsb[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   static uint8 historyMsb[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   
   switch(blueState)
   { 
      //-----------------------------------------------------------------------
      case BLE_IN_RESET://
      {
         if (blueTimer > BLUETIMER_0d5_SECOND)
            blueState = BLE_START;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_START://
      {
         bleJustReset = YES;
         LATDbits.LATD2 = 1; // take BLE121LR out of reset
         bleReset(1);
         // command.c response to this hardware reset is "ble_evt_system_boot"
         blueTimer = 0;
         blueState = BLE_WAITING_FOR_RESET_CALL_BACK;
         break;  
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_RESET_CALL_BACK://
      { 
         if (blueTimer > BLUETIMER_1_SECOND)
         {      
            blueState = BLE_IN_RESET;
            blueTimer = 0;
            LATDbits.LATD2 = 0; // put BLE121LR into reset
            bleReset(0);
         }
         break;
      } 
      //-----------------------------------------------------------------------
      case BLE_START_ADVERTISING: // id = 3
      {
         Xble_cmd_gap_set_mode();
         blueState = BLE_WAITING_ADVERTISING_REPLY;
         blueTimer = 0;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_ADVERTISING_REPLY: // id = 4
      {
         if (blueTimer > BLUETIMER_0d5_SECOND)
            blueState = BLE_DISCONNECTED;
         break;
      }   
      //-----------------------------------------------------------------------
      case BLE_WAITING_FOR_CONNECTION: // id = 5
      {
         if (blueTimer > BLUETIMER_5_SECOND)
            blueState = BLE_DISCONNECTED;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_WAITING_NOTIFICATION_REPLY: // id = 6
      {
         if (blueTimer > BLUETIMER_5_SECOND)
            blueState = BLE_DISCONNECTED;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_IS_CONNECTED: // id = 7
      {
         //----------------------------------------
         if (periodFlag == 1)
         { 
            periodFlag = 0;
            //---
            readDataSensor(STARTSYNC);
            //---
            historyLsb[7] = historyLsb[6];
            historyMsb[7] = historyMsb[6];
            historyLsb[6] = historyLsb[5];
            historyMsb[6] = historyMsb[5];
            historyLsb[5] = historyLsb[4];
            historyMsb[5] = historyMsb[4];            
            historyLsb[4] = historyLsb[3];
            historyMsb[4] = historyMsb[3];
            historyLsb[3] = historyLsb[2];
            historyMsb[3] = historyMsb[2];
            historyLsb[2] = historyLsb[1];
            historyMsb[2] = historyMsb[1];
            historyLsb[1] = historyLsb[0]; 
            historyMsb[1] = historyMsb[0];
            //---
            historyLsb[0] = sensorLsb;
            historyMsb[0] = sensorMsb;
            //---
            countId++;
            //---
            if (bleReady == READY)
            {
               //---
               blueTimer = 0;
               //---
               datasOut[0] = PRODUCT_ID;
               //---
               datasOut[1] = historyMsb[0];
               datasOut[2] = historyLsb[0];
               datasOut[3] = historyMsb[1];
               datasOut[4] = historyLsb[1];           
               datasOut[5] = historyMsb[2];
               datasOut[6] = historyLsb[2];            
               datasOut[7] = historyMsb[3];
               datasOut[8] = historyLsb[3];            
               datasOut[9] = historyMsb[4];
               datasOut[10] = historyLsb[4];
               datasOut[11] = historyMsb[5];
               datasOut[12] = historyLsb[5];
               datasOut[13] = historyMsb[6];
               datasOut[14] = historyLsb[6];
               datasOut[15] = historyMsb[7];
               datasOut[16] = historyLsb[7];
               //---
               datasOut[17] = dataId;
               //---
               switch(dataId)
               {
                  //----------------------
                  case 8:
                  {
                     datasOut[18] = autoCalibrationMsb;
                     datasOut[19] = autoCalibrationLsb;
                     break;
                  }
                  //----------------------
                  case 9:
                  {
                     datasOut[18] = batteryVoltage;
                     datasOut[19] = SOFTWARE_REVISION;
                     break;
                  }
                  //----------------------
                  case 10:
                  {
                     //datasOut[18] = (uint8)(batteryTimeCharging >> 8);
                     //datasOut[19] = (uint8)(batteryTimeCharging & 0x00FF);
                     datasOut[18] = 0;
                     datasOut[19] = 0;
                     break;
                  }
                  //----------------------
                  case 11:
                  {
//                     datasOut[18] = (uint8)(batteryTimeSleeping >> 8);
//                     datasOut[19] = (uint8)(batteryTimeSleeping & 0x00FF);
                     datasOut[18] = 0;
                     datasOut[19] = 0;
                     break;
                  }
                  //----------------------
                  case 12:
                  {
//                     datasOut[18] = (uint8)(batteryTimeRunning >> 8);
//                     datasOut[19] = (uint8)(batteryTimeRunning & 0x00FF);
                     datasOut[18] = 0;
                     datasOut[19] = 0;
                     break;
                  }
                  //----------------------
                  default:
                  {
                     datasOut[18] = calibrationPointsMSB[dataId];
                     datasOut[19] = calibrationPointsLSB[dataId];
                     break;
                  }
                  //----------------------
               }
               //---
               if (++dataId >= 13)
                  dataId = 0;
               //---
               Xble_cmd_attributes_write();
               //---
               bleReady = NOT_READY;
               //---
            }
         }
         //----------------------------------------
         if (blueTimer > BLUETIMER_5_SECOND)
            blueState = BLE_DISCONNECTED;
         //----------------------------------------
         break;
         //----------------------------------------
      }
      //-----------------------------------------------------------------------
      case BLE_DISCONNECTED://
      {
         LATDbits.LATD2 = 0; // put BLE121LR into reset
         bleReset(0);
         blueState = BLE_RESET_TIMEOUT;
         blueTimer = 0;
         break;
      }
      //-----------------------------------------------------------------------
      case BLE_RESET_TIMEOUT://
      {
         // we are giving the BLE121LR module a few milliseconds in reset
         if (blueTimer > 1)
            blueState = BLE_START;
         break;
      }
      //-----------------------------------------------------------------------
      default:
      {
         // this would be an error condition as would be entering an undefined state.
         break;
      }
      //-----------------------------------------------------------------------
   }
}
