//---------------------------------------------
// 5-08-2023  5:14PM
//---------------------------------------------
// FSEC
#pragma config BWRP = ON                  // Boot Segment Write-Protect bit was OFF
#pragma config BSS = DISABLED             // Boot Segment Code-Protect Level bits was DISABLED / HIGH
#pragma config BSEN = 1                   // Boot Segment Control bit 1 = no boot segment defined
#pragma config GWRP = OFF                 // General Segment Write-Protect bit
#pragma config GSS = DISABLED             // General Segment Code-Protect Level bits  ///// must be this
#pragma config CWRP = OFF                 // Configuration Segment Write-Protect bit
#pragma config CSS = DISABLED             // Configuration Segment Code-Protect Level bits DISABLED / HIGH
#pragma config AIVTDIS = OFF              // Alternate Interrupt Vector Table bit

// FBSLIM
#pragma config BSLIM = 0x1FFF           // Boot Segment Flash Page Address Limit bits
//#pragma config BSLIM = 0x1FF7             // Boot Segment Flash Page Address Limit bits  invert the bits then multiply by 128 for end of boot segement address

// FOSCSEL
#pragma config FNOSC = FRCPLL
#pragma config PLLMODE = PLL4X
#pragma config IESO = OFF

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFCN = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config SOSCSEL = OFF             // SOSC Power Selection Configuration bits (SOSC is used in crystal (SOSCI/SOSCO) mode)
#pragma config PLLSS = PLL_PRI          // PLL Secondary Selection Configuration bit (PLL is fed by the Primary oscillator)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration bit (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler bits (1:32,768)
#pragma config FWPSA = PR128            // Watchdog Timer Prescaler bit (1:128)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bits (WDT Enabled)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config WDTCMX = WDTCLK          // WDT MUX Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)

// FPOR
#pragma config BOREN = OFF               // Brown Out Enable bit (Brown Out Enable Bit)
#pragma config LPCFG = OFF              // Low power regulator control (No Retention Sleep)
#pragma config DNVPEN = ENABLE          // Downside Voltage Protection Enable bit (Downside protection enabled using ZPBOR when BOR is inactive)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FDEVOPT1
#pragma config ALTCMPI = DISABLE        // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF            // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config SOSCHP = ON              // SOSC High Power Enable bit (valid only when SOSCSEL = 1 (Enable SOSC high power mode (default))
#pragma config ALTI2C1 = ALTI2CDIS       // Alternate I2C pin Location (SDA1 and SCL1 on RB9 and RB8)
//---------------------------------------------
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <dsp.h>
#include "apitypes.h"
#include "const.h"
#include "MCU_Layer.h"
#include "blueStates.h"
#include "blueWork.h"
//---------------------------------------------
#define _ISR_PSV __attribute__((__interrupt__, __auto_psv__))
//---------------------------------------------
typedef struct
{  
   uint8    connection;
   uint8    flags;
   bd_addr  address;
   uint8    AddressType;
   uint16   connectionInterval;
   uint16   timeout;
   uint16   latency;
   uint8    bonding;  
    
} CONNECTION;
//---------------------------------------------
typedef struct
{
    uint8      connection;
    uint16     start;
    uint16     end;
    uint8      uuidLength;
    uint8      uuid[16];

} BLE_SERVICE;
//---------------------------------------------
// Global Variables
//---------------------------------------------
volatile int8     commsRxBuffer[BUFFER_SIZE_LARGE];
volatile uint16   commsRxIntoBuffer = 0;
volatile uint16   commsRxOutOfBuffer = 0;
volatile int8     commsTxBuffer[BUFFER_SIZE];
volatile uint16   commsTxIntoBuffer = 0;
volatile uint16   commsTxOutOfBuffer = 0;
//---
volatile int8     bleRxBuffer[BUFFER_SIZE];
volatile uint16   bleRxIntoBuffer = 0;
volatile uint16   bleRxOutOfBuffer = 0;
volatile int8     bleTxBuffer[BUFFER_SIZE];
volatile uint16   bleTxIntoBuffer = 0;
volatile uint16   bleTxOutOfBuffer = 0;
//---
uint8             uuIdCommon[13] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
uint8             serialNumbers[3] = {0x0D, 0x0E, 0x0F};
uint8             UUIDserial[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
//---
uint8             uuIdCommonBuffer[13] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8             serialNumbersBuffer[3] = {0x00, 0x00, 0x00};
uint8             UUIDserialBuffer[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//---
const uint8       UUIDdeviceInfo[2] = {0x00, 0x28};   // Service

uint8             blueSensorAddress[6];
uint8             blueSensorAddressFound = NO;
uint8             blueSensorAddressType;

uint8             blueState = BLE_IN_RESET;
        
CONNECTION        blueSensor;
BLE_SERVICE       blueService[10];
uint8             blueServicesFound = 0;
uint8             blueServiceCount = 0;

uint8             serialConnection;
uint16            serialHandle;
uint8             serialLength;
uint8             serialData[20];
uint8             deviceNameConnection;
uint16            deviceNameHandle;
uint8             deviceNameLength;
uint8             connected_seq = 0;
uint8             deviceNameUUID[2];
uint8             deviceNameData[20];
volatile uint16   blueTimer = 0;
unsigned char     sensorStatus = NOT_CONNECTED;
volatile uint16   sensorDataTimerCount = 0;
uint8             bleJustReset = YES;
volatile uint16   timer50mSec = 0;
volatile uint8    flag50mSec = RESET;

uint8             constructedPacket[20];
uint8             shootPacketFlag = RESET;
unsigned char     packetCount = 0;
uint16            ledCount = 0;

//uint16            resetFlagCount = 0;
uint8             bleResetRequest = RESET;
uint8             bleDisconnectRequest = RESET;
uint8             enableBluetoothSequence = NO;
uint8             bleConnectRequestFlag1 = RESET;
uint8             bleConnectRequestFlag2 = RESET;
//---------------------------------------------
extern int ResetDevice(void);
//---------------------------------------------
//void ledToggle(void)
//{
//   static unsigned char toggle = 0;
//   
//   if (toggle == 1)
//   {
//      toggle = 0;
//      LED_BLUE;
//   }
//   else
//   {
//      toggle = 1;
//      LED_AQUA;
//   }
//}
//---------------------------------------------
void analyseCommsIn(void)
{
   unsigned char data, n;
   static unsigned char header = NO;
   static unsigned char msnLsn = 0;
   static unsigned char msN;
   static unsigned char byteCount = 0;
   static unsigned char datas[256];
   
   if (commsRxIntoBuffer != commsRxOutOfBuffer)
	{
		//---
		_U2RXIE = 0;			
		data = commsRxBuffer[commsRxOutOfBuffer];
		if (++commsRxOutOfBuffer >= BUFFER_SIZE_LARGE)
			commsRxOutOfBuffer = 0;
		_U2RXIE = 1;
		//---
      if ((data & 0b10000000) == 0b10000000)
      {
         //----------------------------------------
         // is a header
         //----------------------------------------
         header = data;
         //----------------------------------------
         switch(header)
         {
            //----------------------------------------
            case REBOOT_DONGLE:
            {
               ResetDevice();
               break;
            }
            //----------------------------------------
            case DONGLE_WRITE_BACK_HEADER:
            {
               shootCommsData(DONGLE_WRITE_BACK_HEADER);
               break;
            }
            //----------------------------------------
            case ENABLE_BT_SEQ:
            {
               enableBluetoothSequence = YES;
               break;
            }
            //----------------------------------------
            case TEST_CONNECTION_HEADER:
            {
               if (sensorStatus == CONNECTED)
                  shootCommsData(CONNECTION_ESTABLISHED_HEADER); 
               else
                  shootCommsData(NO_CONNECTION_HEADER);
               break;
            }
            //----------------------------------------
            case START_HEADER:
            {
               packetCount = 0;
               msnLsn = 0;
               break;
            }
            //----------------------------------------
            case STOP_HEADER:
            {
               if ((packetCount == 20) && (msnLsn == 0))
               {
                  // all good
                  shootPacketFlag = SET;
                  if (ledCount == 0)
                     ledCount = BLUETIMER_0d5_SECOND;
               }
               else
               {
                  // an error has occurred
               }
               break;
            }
            //----------------------------------------
            case GATT_DATA_HEADER:
            {
               byteCount = 0;
               break;
            }
            //----------------------------------------
            case GETGATT_HEADER:
            {
               shootCommsData(GETGATT_HEADER);
               for (n = 0 ; n < 13 ; n++)
               {
                  shootCommsData(uuIdCommon[n] >> 4);
                  shootCommsData(uuIdCommon[n] & 0x0F);
               }
               for (n = 0 ; n < 3 ; n++)
               {
                  shootCommsData(serialNumbers[n] >> 4);
                  shootCommsData(serialNumbers[n] & 0x0F);
               }
               for (n = 0 ; n < 16 ; n++)
               {
                  shootCommsData(UUIDserial[n] >> 4);
                  shootCommsData(UUIDserial[n] & 0x0F);
               }
               break;
            }
            //----------------------------------------
            case DISCONNECT_BLUETOOTH:
            {
               bleDisconnectRequest = SET;
               break;
            }
            //----------------------------------------
            default:
            {
               break;
            }
            //----------------------------------------
         }
      }
      else
      {
         //----------------------------------------
         // is data
         //----------------------------------------
         switch(header)
         {
            //------------------------------------------------------
            case START_HEADER:
            {
               if (packetCount < 20)
               {
                  if (msnLsn == 0)
                  {
                     msN = data;
                     msnLsn = 1;
                  }
                  else
                  {
                     constructedPacket[packetCount++] = ((msN & 0x0F) * 16) + (data & 0x0F);
                     msnLsn = 0;
                  }      
               }
               else
               {
                  // an error has occurred
               }
               break;
            }
            //------------------------------------------------------
            case GATT_DATA_HEADER:
            {
               //---
               datas[byteCount] = data;
               //---
               if(byteCount == 63)
               {
                  //---
                  for (n = 0 ; n < 13 ; n++)
                     uuIdCommonBuffer[n] = (16 * datas[n * 2]) + datas[(n * 2) + 1];
                  //---
                  for (n = 13 ; n < 16 ; n++)
                     serialNumbersBuffer[n - 13] = (16 * datas[n * 2]) + datas[(n * 2) + 1];
                  //---
                  for (n = 16 ; n < 32 ; n++)
                     UUIDserialBuffer[n - 16] = (16 * datas[n * 2]) + datas[(n * 2) + 1];
                  //---
                  bleResetRequest = SET;
                  enableBluetoothSequence = YES;
                  bleConnectRequestFlag1 = SET;
                  //---
               }
               //---
               if (++byteCount >= 64) // protecting array from overwrite
                  byteCount = 64;
               //---
               break;
               //---
            }
            //------------------------------------------------------
            default:
            {
               // an error has occurred
               break;
            }         
            //------------------------------------------------------
         }
      }
      //---
   }
}
//---------------------------------------------
void analyseBleIn(void)
{
   unsigned char data;
   
   if (bleRxIntoBuffer != bleRxOutOfBuffer)
	{
		//---
		_U1RXIE = 0;			
		data = bleRxBuffer[bleRxOutOfBuffer];
		if (++bleRxOutOfBuffer >= BUFFER_SIZE)
			bleRxOutOfBuffer = 0;
		_U1RXIE = 1;
		//---
      ReceiveBTMessage(data);
      //---
   }
}
//---------------------------------------------
void led_display(void)
{
   if (sensorStatus == CONNECTED)
   {    
      if (ledCount > BLUETIMER_0d25_SECOND)
      {
        LED_OFF;
      }
      else
      {
         LED_GREEN;
      }
   }
   else
   {
      LED_BLUE;
   }
}
//---------------------------------------------
int main(int argc, char** argv)
{
   //unsigned char shootPacketFlagStow = RESET;
   //unsigned char n;
   //---
   mcuInit();
   initialiseTimer1();
   initialiseUart1();
   initialiseUart2();
   //---
   while(1)
   {
      //---
//      if (shootPacketFlag != shootPacketFlagStow)
//      {
//         if (shootPacketFlag == SET)
//            shootCommsData(RESET_FLAG_1);
//         else
//            shootCommsData(RESET_FLAG_0);
//         shootPacketFlagStow = shootPacketFlag;
//      }
      //---
      if (shootPacketFlag == RESET)
      {
         analyseCommsIn();
      }
      else
      {       
      }
      //---
      
      //analyseCommsIn();
      if (enableBluetoothSequence == YES)
      {
         analyseBleIn();
         BLE_state_manage();
      }
      led_display();
      //---
   }
   //---
}
//---------------------------------------------
void _ISR_PSV _T1Interrupt()
{
   //--------------------------------
   // 1 millisecond period
   //--------------------------------
   IFS0bits.T1IF = 0;
   //--- 
   if (++timer50mSec >= 50)
   {
      timer50mSec = 0;
      flag50mSec = SET;
      //---
      if (++blueTimer >= 10000)
         blueTimer = 10000;
      //---
      if (ledCount != 0)
         --ledCount;
      //---
      //if (++resetFlagCount > 1000)
      //   resetFlagCount = 10000;
      //---
   }
   //--------------------------------
}
//---------------------------------------------
void _ISR_PSV _U1RXInterrupt()
{
   IFS0bits.U1RXIF = 0;
   bleRxBuffer[bleRxIntoBuffer] = U1RXREG; 
   if (++bleRxIntoBuffer >= BUFFER_SIZE)
      bleRxIntoBuffer = 0;
   if(U1STAbits.OERR == 1)
		U1STAbits.OERR = 0;
	if(U1STAbits.FERR == 1)
		U1STAbits.FERR = 0;
	if(U1STAbits.PERR == 1)
		U1STAbits.PERR = 0;
 }
//---------------------------------------------
void _ISR_PSV _U2RXInterrupt()
{   
   IFS1bits.U2RXIF = 0;
   commsRxBuffer[commsRxIntoBuffer] = U2RXREG; 
   if (++commsRxIntoBuffer >= BUFFER_SIZE_LARGE)
      commsRxIntoBuffer = 0;
   if(U2STAbits.OERR == 1)
		U2STAbits.OERR = 0;
	if(U2STAbits.FERR == 1)
		U2STAbits.FERR = 0;
	if(U2STAbits.PERR == 1)
		U2STAbits.PERR = 0;
 }
//---------------------------------------------
void _ISR_PSV _U1TXInterrupt()
{
	IFS0bits.U1TXIF = 0;
	if (bleTxIntoBuffer == bleTxOutOfBuffer)
		IEC0bits.U1TXIE = 0;
	else
		{
			while((U1STAbits.UTXBF == 0) && (bleTxIntoBuffer != bleTxOutOfBuffer))
				{
					U1TXREG = bleTxBuffer[bleTxOutOfBuffer];
					if (++bleTxOutOfBuffer >= BUFFER_SIZE)
						bleTxOutOfBuffer = 0;
				} 	
		  	if (bleTxIntoBuffer == bleTxOutOfBuffer)
				IEC0bits.U1TXIE = 0;
		  	else
		  		IEC0bits.U1TXIE = 1;
		}
}
 //---------------------------------------------
void _ISR_PSV _U2TXInterrupt()
{
	IFS1bits.U2TXIF = 0;
	if (commsTxIntoBuffer == commsTxOutOfBuffer)
		IEC1bits.U2TXIE = 0;
	else
		{
			while((U2STAbits.UTXBF == 0) && (commsTxIntoBuffer != commsTxOutOfBuffer))
				{
					U2TXREG = commsTxBuffer[commsTxOutOfBuffer];
					if (++commsTxOutOfBuffer >= BUFFER_SIZE)
						commsTxOutOfBuffer = 0;
				} 	
		  	if (commsTxIntoBuffer == commsTxOutOfBuffer)
				IEC1bits.U2TXIE = 0;
		  	else
		  		IEC1bits.U2TXIE = 1;
		}
}
 //---------------------------------------------


