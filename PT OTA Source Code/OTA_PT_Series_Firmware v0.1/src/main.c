//---------------------------------------------------
//  PT Series Firmware (PT600) Bootloader Ready Revision #??? - Scott
//---------------------------------------------------
//  Search for "BootLoader Modifications" to find the areas 
//  with explanations of the extra or modified code to allow 
//  it to be able to be used with the bootloader.
//
//  Also note that while developing the PT code it is not necessary to use
//  the bootloader. 
//  This code can be flashed onto a device using
//  the PicKit4, but note if you do so the bootloader code on the PT device
//  will be overwritten.
//---------------------------------------------------
// PIC18LF46K40 Configuration Bit Settings
// 'C' source line config statements
// Real Sensor 1_01
//---------------------------------------------------
// CONFIG1L
#pragma config FEXTOSC = OFF              //
#pragma config RSTOSC = LFINTOSC          //

// CONFIG1H
#pragma config CLKOUTEN = OFF             //
#pragma config CSWEN = ON                 // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON                 // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = INTMCLR            // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTE = ON                 // Power-up Timer Enable bit (Power up timer disabled) // was ON
#pragma config LPBOREN = OFF              // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF

// CONFIG2H
#pragma config BORV = VBOR_190            // Brown Out Reset Voltage selection bits (Brown-out Reset Voltage (VBOR) set to 1.90V)
#pragma config ZCD = OFF                  // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON               // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = OFF               // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF                // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF                // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_11         // WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = ON                  // WDT operating mode (WDT enabled regardless of sleep)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7          // WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = LFINTOSC          // WDT input clock selector (internal low freq osc)

// CONFIG4L
#pragma config WRT0 = OFF  //////         // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF                 // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF                 // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF                 // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG4H
#pragma config WRTC = OFF //////                 // Configuration Register Write Protection bit (Configuration registers (300000-30000Bh) not write-protected)
#pragma config WRTB = OFF  ///////////                // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF                 // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config SCANE = OFF                // Scanner Enable bit (Scanner module is available for use, SCANMD bit can control the module)
#pragma config LVP = ON                   // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF  ////////////////                    // UserNVM Program Memory Code Protection bit (UserNVM code protection disabled)
#pragma config CPD = OFF                  // DataNVM Memory Code Protection bit (DataNVM code protection disabled)

// CONFIG5H

// CONFIG6L
#pragma config EBTR0 = OFF                /// Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF                /// Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF                /// Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF                /// Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG6H
#pragma config EBTRB = OFF                /// Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks

//###########################################################################
//  BootLoader Modifications
//###########################################################################
//  Right click on the PT project and select Properties
//  1. in the "Pack:" Section you need to be using... 
//  PIC18F-K_DFP -> 1.8.249
//  
//  2. in the "Compiler Toolchain:" use V2.2 of the compiler

//  3. The linker needs to be modified to place the code in the flash memory $4000 to $FFFF
//  Select "XC8 Linker" and in the Options Categories: drop down menu select "Memory Model"
//  Then in the Rom Ranges box type -0000-3fff  , note the minus sign at the beginning,
//  The minus sign indicates a memory range not to use.
//  Click Apply and then OK.
//###########################################################################


#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "blueStates.h"
#include "apitypes.h"
#include "blueWork.h"
#include "InterruptFunction.h"


//###########################################################################
//  BootLoader Modifications
//###########################################################################
// This persistent absolute address variable is shared by the bootloader and the app
//---
__persistent unsigned char normalOrBoot __at(0x0000);  // this is located in data memory
//###########################################################################


//###########################################################################
//  BootLoader Modifications
//###########################################################################
// This memory is reserved for future "Normal Code" that needs persistent memory
// Eg: is time data to calculate the charge status
//---
__persistent unsigned char reservedFileRegisters[64] __at(0x0001);
//###########################################################################


//###########################################################################
//  BootLoader Modifications
//###########################################################################
// Software Revision Number, this will read by the bootloader and sent to the tablet
// so i knows what code is already programmed into the PT.
// if it reads 0xFF (255), then it has not been programmed yet.
// 0xFF is a reserved code revision number
// This must always be updated as the code revisions are made.
//---
const unsigned int codeRevisionNumber __at(0xFFFE) = 1; // 16 bit
//###########################################################################


//###########################################################################
//  BootLoader Modifications
//###########################################################################
// This Flash memory is reserved so the Delphi code can make modifications within this space.
// this all bootloader centric / not data
const unsigned int constReserveMemory[32] __at(0x4000) = 
{  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
   0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};
//###########################################################################


//###########################################################################
//  BootLoader Modifications
//###########################################################################
//------------------------------------------------------------------------------
// This Flash memory is reserved for the writing of data
// Need to consider the ability of App code to be able to flash its own 
// space with data.
// In Delphi we need to make sure that this section is not flashed, we can do this by
// setting these "blocks to program" to 0.
//------------------------------------------------------------------------------
const unsigned int reservedFlashDataMemory[0x07DF] __at(0x4040);
//###########################################################################



//Bluetooth variables
volatile uint8    blueState = BLE_START;
volatile uint16   blueTimer = 0;
volatile uint8    bleReady = NOT_READY;
uint8             bleReadyStore = NOT_READY;
volatile uint8    timer25mSec = 0;
volatile uint8    flag25mSec = FALSE;
//---
volatile uint16   periodTimer = 0;
volatile uint8    periodFlag = 0;
//---------------------------------------------------
unsigned char     systemClockSpeed = CLOCK_SPEED_LOW;
volatile uint8    ledCount = 0;
volatile uint8    ledDisplayCount = 0;
//---------------------------------------------------
volatile unsigned char     calibrationPointsLSB[8];
volatile unsigned char     calibrationPointsMSB[8];
volatile unsigned char     calibrationFlagsSet[8] = {NO, NO, NO, NO, NO, NO, NO, NO};
//---------------------------------------------------
volatile uint8    bleRxBuffer[BUFFER_SIZE];
volatile uint16   bleRxIntoBuffer = 0;
volatile uint16   bleRxOutOfBuffer = 0;

volatile uint8    bleTxBuffer[BUFFER_SIZE];
volatile uint16   bleTxIntoBuffer = 0;
volatile uint16   bleTxOutOfBuffer = 0;

volatile uint16   bleBufferDepthCount = 0;

unsigned char     ads1120Config0;
unsigned char     ads1120Config1;
unsigned char     ads1120Config2;
unsigned char     ads1120Config3;

volatile uint16   solidColourTimer = 0;
volatile uint8    solidColour = BLACK;
uint8    calibrationColours[8] = {WHITE, RED, BLUE, YELLOW, GREEN, PURPLE, AQUA, BLACK};

volatile uint8    batteryVoltage = 0;
uint8             secTimeFlag = 0;
uint16            secTimeCount = 0;

uint8             sensorLsb;
uint8             sensorMsb;

uint8             autoCalibrationLsb;
uint8             autoCalibrationMsb;

//---------------------------------------------------
void timer0Setup(void);
void uart1Setup(void);
void switchToHighFreqOsc(void);
void switchToLowFreqOsc(void);
void initialiseLeds(void);
void initialiseAdc(void);
void initialiseDac(void);
void initialiseVoltageReference(void);
void initialiseIoBitsAndPieces(void);
void initialiseBitBashSpi(void);
void initialiseBlueToothIo(void);
unsigned char ifDcPlugInserted(void);
void analyseBleRxIn(void);
void __interrupt () my_isr (void);
//---------------------------------------------
void main(int argc, char** argv) 
{
   uint8 data, n;
   uint16 x;
   uint16 d;
  
   //------------------------------------------------------
   normalOrBoot = NORMAL;
   //------------------------------------------------------
   ANSELA = 0;             // all analog inputs disabled
   ANSELB = 0;             // all analog inputs disabled
   ANSELC = 0;             // all analog inputs disabled
   ANSELD = 0;             // all analog inputs disabled
   //---
   initialiseLeds();
   initialiseIoBitsAndPieces();
   initialiseBlueToothIo();
   //------------------------------------------------------
   if (ifDcPlugInserted() == YES)
   {
      //---------------------------
      // OFF MODE or Charging
      //---------------------------
      // ACP = 1 means no charging - BLACK ultra low power mode (RD5)
      // ACP = 0 means we are charging can now consider CHRG's state
      // CHRG = 0 is still charging ie RED (RD0)
      // CHRG = 1 means the battery is charged ie GREEN
      // Both these signals need internal weak pullups
      //---------------------------
      T2CONbits.ON = 1;       // Timer2 turned on
      T2CONbits.CKPS = 0b101; // 4Hz pulse
      T2CONbits.OUTPS = 0b000;
      T2CLKCONbits.CS = 0b0100;
      //---------------------------
      // XXXX - Good to here
      //---------------------------
      if ((PORTD & 0b00100000) == 0b00100000)   // test to see if power is applied
      {
         //----------------------------------------------------------
         // power is not applied - ultra low power mode we do nothing
         //----------------------------------------------------------   
         LATDbits.LATD2 = 0; // we are taking the BLE121LR out of reset here so it can go to low power mode
         //----
         for (d = 0 ; d < 500 ; d++)  // wait for a while
            ClrWdt();
         //---
         LATDbits.LATD2 = 1;
         //---
         LATCbits.LATC0 = 0;   // 5.5V enable, OFF = 0, ON = 1 
         
         LATEbits.LATE0 = 0;     // BT wakeup pin is cleared so it can go to sleep
         TRISEbits.TRISE0 = 0;

         TRISAbits.TRISA4 = 1;  
         TRISAbits.TRISA3 = 1;   
         TRISAbits.TRISA1 = 1;
         TRISAbits.TRISA2 = 1;
         WPUA = 0b00000000;
         TRISAbits.TRISA5 = 1;
         
         T0CON0bits.T0EN = 0;     
         TXSTA1bits.TXEN = 0;
         RCSTA1bits.SPEN = 0;
         
         ledColour(BLACK);
         //---
         while(1)
         {
            //----
            ClrWdt();
            //---
            if (ifDcPlugInserted() == NO)
               while(1){}   // we will reset and then go into new mode
            //---
            if ((PORTD & 0b00100000) == 0b00100000)   // test to see if power is applied
            {
               //---
               // power is not applied - ultra low power mode we keep do nothing
               // we are in sleep mode, so stay in while loop
               //---
               if (PIR4bits.TMR2IF == 1)
               {
                  // we enter this 4 times per second
                  PIR4bits.TMR2IF = 0;
               }
               //---
            }    
            else
               while(1){}    // we will reset and then go into new mode 
            //---
         }
         //----------------------------------------------------------
      }
      else
      {
         //----------------------------------------------------------
         // charging mode
         //----------------------------------------------------------
         while(1) 
         {
            //---
            ClrWdt();
            //---
            LATCbits.LATC0 = 1;     // 5.5V enable, OFF = 0, ON = 1
            //---
            if ((PORTD & 0b00000001) == 0b00000001)
            {
               //---
               ledColour(GREEN); // is charged GREEN
               //---
            }
            else 
            {
               //---
               ledColour(RED);    // charging RED 
               //---
               if (PIR4bits.TMR2IF == 1)
               {
                  PIR4bits.TMR2IF = 0;
               }
               //---
            }
            //---
            if ((PORTD & 0b00100000) == 0b00100000) // test to see if power is applied
            {
               ledColour(BLACK);    // we will reset and then go into new mode   
               while(1)
               {
               }
            }
            //---
            if (ifDcPlugInserted() == NO)
            {
               ledColour(BLACK);    // we will reset and then go into new mode   
               while(1)
               {
               }
            }
            //---
         }
         //----------------------------------------------------------
      }
      //---------------------------
   }
   else
   {
      //-----------------------------------------------------------------------
      // Bluetooth Sensor Mode
      //-----------------------------------------------------------------------
      LATCbits.LATC0 = 1;     // 5.5V enable, OFF = 0, ON = 1
      //-----------------------------------------------------------------------
      // New Code here - 5.5V power supply needs time to ramp up
      //-----------------------------------------------------------------------
      for (d = 0 ; d < 500 ; d++) 
         ClrWdt();
      //-----------------------------------------------------------------------
      if (systemClockSpeed != CLOCK_SPEED_HIGH)
      {
         systemClockSpeed = CLOCK_SPEED_HIGH;
         switchToHighFreqOsc();
      }
      //-----------------------------------------------------------------------
      LATEbits.LATE0 = 1;     // BT wakeup pin is set so it can't go to sleep
      TRISEbits.TRISE0 = 0;
      //---
      initialiseVoltageReference(); // new
      initialiseDac(); // new
      initialiseAdc(); // new
      initialiseBitBashSpi();
      timer0Setup();
      uart1Setup();
      setupADS1120();
      readInCalibrationPoints();  
      readAllADS1120configRegisters();
      //---
      if ((ads1120Config0 != ADS1120_CONFIG_0) || 
              (ads1120Config1 != ADS1120_CONFIG_1) || 
              (ads1120Config2 != ADS1120_CONFIG_2) || 
              (ads1120Config3 != ADS1120_CONFIG_3))
      {
         ledColour(YELLOW);
      }    
      //---------------------------
      for (n = 0 ; n < 10 ; n++)
         readDataSensor(STARTSYNC);
      //---------------------------
      autoCalibrationLsb = sensorLsb;
      autoCalibrationMsb = sensorMsb;
      //---------------------------  
      GIE = 1;
      //---------------------------
      LATDbits.LATD2 = 1; // take BLE121LR out of reset
      //---------------------------
      while(1)
      {
         //---
         LATCbits.LATC0 = 1;     // 5.5V enable, OFF = 0, ON = 1
         //---
         if (ifDcPlugInserted() == YES)  
         {
            //---
            ledColour(BLACK);
            LATCbits.LATC0 = 0;     // 5.5V enable, OFF = 0, ON = 1
            //---
            GIE = 0;
            readDataSensor(POWERDOWN);
            //---
            //Turn off ADC module
            ADCON0bits.ADON = 0;
            //---
            // DAC is turned off
            DAC1CON0bits.EN = 0;
            //---
            // Reference voltage is switched off
            FVRCONbits.FVREN = 0;
            //---
            switchToLowFreqOsc();
            //---
            while(1) // watchdog will reset it from here
            {  
            }
            //---
         }
         //---
         if (bleBufferDepthCount > BUFFER_SIZE_DIV_2)
            mcuRtsHigh();
         else
            mcuRtsLow();
         //---
         analyseBleRxIn();
         XBLE_state_manage();
         //---
         ClrWdt();
         //---
         if (secTimeFlag == 1)
         {
            //---
            secTimeFlag = 0;
            //---
            if (ADCON0bits.ADGO == 0)
            {
               x = ADRES;
               x = x >> 2;
               batteryVoltage = (unsigned char)x;
               ADCON0bits.ADGO = 1;
            }  
            //---
         }
         //---
         if (solidColourTimer != 0)
            ledColour(calibrationColours[solidColour]);   
         else
         {
            //---
            if (ledDisplayCount == 19)
            {
               if (blueState == BLE_IS_CONNECTED)
                  ledColour(GREEN);
               else
                  ledColour(BLUE);
            }
            else
               ledColour(BLACK);
         }
      }
      //---------------------------
   }
   //------------------------------------------------------
}
//---------------------------------------------------
/*
 * Function: timer0Setup
 * ----------------------------
 *   Sets up a 16MHz timer on timer0
 */
void timer0Setup(void)
{
   //---
   T0CON0bits.T016BIT = 0;       // 8 bit mode = 0, 16 bit timer = 1
   T0CON0bits.T0OUTPS = 0b0000;  // post divider, set to 1 
   //---
   T0CON1bits.T0CS = 0b011;      // clock source = HFINTOSC
   T0CON1bits.T0ASYNC = 0;       // sync to Fosc / 4
   T0CON1bits.T0CKPS = 0b0110;   // prescaler = 6 - 16MHz
   //---
   T0CON0bits.T0EN = 1;          //enable timer
   //---
   PIR0bits.INT0IF = 0;          // reset interrupt flag
   //---
   PIE0bits.TMR0IE = 1;
   //---
}
//--------------------------------------------
/*
 * Function: uart1Setup
 * ----------------------------
 *   Sets up UART1 for high speed asychronous slave-mode 
 *   communication with a BLE module
 *   - 56700 Baud Rate 
 *   - 16MHz
 */
void uart1Setup(void)
{
   uint8 dummyRead;
   //--------------------------------------------
   // Setup UART
   //--------------------------------------------
   // TxSTAx: TRANSMIT STATUS AND CONTROL REGISTER
   //--------------------------------------------
   
   // bit 7 CSRC: Clock Source Select bit
   // Asynchronous mode:
   // Don?t care
   // Synchronous mode:
   // 1 = Master mode (clock generated internally from BRG)
   // 0 = Slave mode (clock from external source)
   TXSTA1bits.CSRC = 1;
   
   // bit 6 TX9: 9-bit Transmit Enable bit
   // 1 = Selects 9-bit transmission
   // 0 = Selects 8-bit transmission
   TXSTA1bits.TX9 = 0;
   
   // bit 4 SYNC: EUSART Mode Select bit
   // 1 = Synchronous mode
   // 0 = Asynchronous mode
   TXSTA1bits.SYNC = 0;
   
   // bit 3 SENDB: Send Break Character bit
   // Asynchronous mode:
   // 1 = Send Sync Break on next transmission (cleared by hardware upon completion)
   // 0 = Sync Break transmission completed
   // Synchronous mode:
   // Don?t care
   TXSTA1bits.SENDB = 0; // ????????
   
   // bit 2 BRGH: High Baud Rate Select bit
   // Asynchronous mode:
   // 1 = High speed
   // 0 = Low speed
   // Synchronous mode:
   // Unused in this mode
   TXSTA1bits.BRGH = 1;
   
   // bit 1 TRMT: Transmit Shift Register Status bit
   // 1 = TSR empty
   // 0 = TSR full
   //TXSTA1bits.TRMT / Read only
   
   // bit 0 TX9D: Ninth bit of Transmit Data
   // Can be address/data bit or a parity bit.
   //TXSTA1bits.TX9D / Read only
   
   // bit 5 TXEN: Transmit Enable bit(1)
   // 1 = Transmit enabled
   // 0 = Transmit disabled
   TXSTA1bits.TXEN = 1;
   //--------------------------------------------
   // RCSTA1: RECEIVE STATUS AND CONTROL REGISTER
   //--------------------------------------------
   
   // bit 6 RX9: 9-bit Receive Enable bit
   // 1 = Selects 9-bit reception
   // 0 = Selects 8-bit reception
   RCSTA1bits.RX9 = 0;
           
   // bit 5 SREN: Single Receive Enable bit
   // Asynchronous mode:
   // Don?t care
   // Synchronous mode ? Master:
   // 1 = Enables single receive
   // 0 = Disables single receive
   // This bit is cleared after reception is complete.
   // Synchronous mode ? Slave
   // Don?t care
   
   RCSTA1bits.SREN = 0;
           
   // bit 4 CREN: Continuous Receive Enable bit
   // Asynchronous mode:
   // 1 = Enables receiver
   // 0 = Disables receiver
   // Synchronous mode:
   // 1 = Enables continuous receive until enable bit CREN is cleared (CREN overrides SREN)
   // 0 = Disables continuous receive
   RCSTA1bits.CREN = 1;
           
   // bit 3 ADDEN: Address Detect Enable bit
   // Asynchronous mode 9-bit (RX9 = 1):
   // 1 = Enables address detection, enable interrupt and load the receive buffer when RSR<8> is set
   // 0 = Disables address detection, all bytes are received and ninth bit can be used as parity bit
   // Asynchronous mode 8-bit (RX9 = 0):
   // Don?t care
   RCSTA1bits.ADDEN = 0;
           
   // bit 2 FERR: Framing Error bit
   // 1 = Framing error (can be updated by reading RCREGx register and receive next valid byte)
   // 0 = No framing error
   // RCSTA1bits.FERR / Read Only
           
   // bit 1 OERR: Overrun Error bit
   // 1 = Overrun error (can be cleared by clearing bit CREN)
   // 0 = No overrun error
   //RCSTA1bits.OERR / Read Only
           
   // bit 0 RX9D: Ninth bit of Received Data
   // This can be address/data bit or a parity bit and must be calculated by user firmware.
   //RCSTA1bits.RX9D / Read Only
   
   //--------------------------------------------
   // BAUDCON1: BAUD RATE CONTROL REGISTER
   //--------------------------------------------
   
   // bit 7 ABDOVF: Auto-Baud Detect Overflow bit
   // Asynchronous mode:
   // 1 = Auto-baud timer overflowed
   // 0 = Auto-baud timer did not overflow
   // Synchronous mode:
   // Don?t care
   // BAUDCON1bits.ABDOVF / Read Only
           
   // bit 6 RCIDL: Receive Idle Flag bit
   // Asynchronous mode:
   // 1 = Receiver is Idle
   // 0 = Start bit has been detected and the receiver is active
   // Synchronous mode:
   // Don?t care
   // BAUDCON1bits.RCIDL / Read Only
           
   // bit 5 DTRXP: Data/Receive Polarity Select bit
   // Asynchronous mode:
   // 1 = Receive data (RXx) is inverted (active-low)
   // 0 = Receive data (RXx) is not inverted (active-high)
   // Synchronous mode:
   // 1 = Data (DTx) is inverted (active-low)
   // 0 = Data (DTx) is not inverted (active-high)
   //////BAUDCON1bits.DTRXP = 0; // ???????????
           
   // bit 4 CKTXP: Clock/Transmit Polarity Select bit
   // Asynchronous mode:
   // 1 = Idle state for transmit (TXx) is low
   // 0 = Idle state for transmit (TXx) is high
   // Synchronous mode:
   // 1 = Data changes on the falling edge of the clock and is sampled on the rising edge of the clock
   // 0 = Data changes on the rising edge of the clock and is sampled on the falling edge of the clock
   BAUDCON1bits.CKTXP = 0;
           
   // bit 3 BRG16: 16-bit Baud Rate Generator bit
   // 1 = 16-bit Baud Rate Generator is used (SPBRGHx:SPBRGx)
   // 0 = 8-bit Baud Rate Generator is used (SPBRGx)
   //BAUDCON1bits.BRG16 = 0;
   BAUDCON1bits.BRG16 = 1;
           
   // bit 1 WUE: Wake-up Enable bit
   // Asynchronous mode:
   // 1 = Receiver is waiting for a falling edge. No character will be received but RCxIF will be set on the falling
   // edge. WUE will automatically clear on the rising edge.
   // 0 = Receiver is operating normally
   // Synchronous mode:
   // Don?t care
   // BAUDCON1bits.WUE / Read only

   // bit 0 ABDEN: Auto-Baud Detect Enable bit
   // Asynchronous mode:
   // 1 = Auto-Baud Detect mode is enabled (clears when auto-baud is complete)
   // 0 = Auto-Baud Detect mode is disabled
   // Synchronous mode:
   // Don?t care
   BAUDCON1bits.ABDEN = 0;

   //SPBRG1 = 25; // 19200 BAUD
   //SPBRG1 = 16; // 31250 BAUD = midi
   //SPBRGH1 = 0;
   //SPBRG1 = 68; // 56700 BAUD // was 34
   //SP1BRG = 68;
   //SPBRG1 = 34; // 56700 BAUD
   SPBRG1 = 68; // 56700 BAUD - 16MHz
   
   // bit 7 SPEN: Serial Port Enable bit
   // 1 = Serial port enabled (configures RXx/DTx and TXx/CKx pins as serial port pins)
   // 0 = Serial port disabled (held in Reset)
   RCSTA1bits.SPEN = 1;
   
   INTCONbits.PEIE = 1;
   //PIR3bits.RC1IF = 0; // clear interrupt
   dummyRead = RC1REG;
   PIE3bits.RC1IE = 1;
}
//---------------------------------------------------
/*
 * switchToHighFreqOsc
 * ----------------------------
 *   
 */
void switchToHighFreqOsc(void)
{
   //---
   OSCCON1bits.NOSC = 0b110;  // select HFINTOSC
   OSCFRQ = 0b00000101;       // 16Mhz
   //---
   OSCCON1bits.NDIV = 0b0000; // divide clock by 1
   //---
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
   //---
   while(OSCCON3bits.ORDY == 0)
   {
   }
   //---
   systemClockSpeed = CLOCK_SPEED_HIGH;
   //---
}
//---------------------------------------------------
void switchToLowFreqOsc(void)
{
   //---
   OSCCON1bits.NOSC = 0b101;  // select LFINTOSC
   OSCCON1bits.NDIV = 0b0000; // divide clock by 1
   //---
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
   //---
   while(OSCCON3bits.ORDY == 0)
   {
   }
   //---
   systemClockSpeed = CLOCK_SPEED_LOW;
   //---
}
//---------------------------------------------------
void initialiseLeds(void)
{
   //---
   LATCbits.LATC0 = 0;     // 5.5V enable, OFF = 0, ON = 1 we have turned the 5.5V power off
   TRISCbits.TRISC0 = 0;
   //---
   TRISBbits.TRISB1 = 0;   // RED LED
   TRISBbits.TRISB2 = 0;   // GREEN LED
   TRISBbits.TRISB3 = 0;   // BLUE LED
   //--- 
   ledColour(BLACK);
   //---
}
//---------------------------------------------------
void initialiseAdc(void)
{ 
   //---
   ANSELA = 0;    // all analog inputs disabled
   //ANSELB = 0b10000000;    // all analog inputs disabled
   ANSELB = 0;
   ANSELC = 0;    // all analog inputs disabled
   ANSELD = 0;    // all analog inputs disabled
   //---
   //TRISBbits.TRISB7 = 1; //Set RB7 to input 
   //---
   
   //Select ADC conversion clock
   //ADCON0bits.ADCS = 1; //FRC
   ADCON0bits.ADCS = 0; //Fosc
   ADCLK = 0b00111111;
   //ADCLK = 0b00001111;
   
   //Configure voltage reference to 0 volts
   ADREFbits.ADNREF = 0; 
   
   //Configure voltage reference
   //ADREFbits.ADPREF = 0b11; // connected to FVR - error in silicon?
   ADREFbits.ADPREF = 0b00; // connected to Vcc
   
   //Select ADC input channel 
   //ADPCH = 0b001111; // RB7/ANB7
   //ADPCH = 0b111111; // fixed voltage reference selected
   //ADPCH = 0b111100; // Vss
   ADPCH = 0b111110;   // DAC
   
   // Right Justify the results
   ADCON0bits.ADFM = 1;
   
   // number for data aquisition time
   //ADACQ = 0b11111111;
   ADACQ = 0;
   
   //---
   //Turn on ADC module
   ADCON0bits.ADON = 1;
   //---
}
//---------------------------------------------------
void initialiseDac(void)
{
   //---
   // DAC Voltage Output Enable bit
   //---
   DAC1CON0bits.OE1 = 0; // DAC voltage level is disconnected from the DAC1OUT1 pin
   //---
   // DAC Voltage Output Enable bit
   //---
   DAC1CON0bits.OE2 = 0; // DAC voltage level is disconnected from the DAC1OUT2 pin
   //---
   // DAC Positive Source Select bit
   //---
   //DAC1CON0bits.PSS = 0b00; // AVDD
   DAC1CON0bits.PSS = 0b10; // FVR
   //---
   // DAC Negative Source Select bit
   //---
   DAC1CON0bits.NSS = 0; // AVSS 
   //---
   // DAC1R[4:0]?Data Input Register for DAC bits this is 12 / 32 = 0.25 of the actual voltage
   //---
   DAC1CON1 = 31;
   //---
   // DAC Enable bit
   //---
   DAC1CON0bits.EN = 1; // DAC is enabled
   //---
}
//---------------------------------------------------
void initialiseVoltageReference(void)
{
   // Bit 7 ? FVREN?Fixed Voltage Reference Enable bit
   // Value Description
   // 1 Fixed Voltage Reference is enabled
   // 0 Fixed Voltage Reference is disabled
   FVRCONbits.FVREN = 1;
   
   // Bit 5 ? TSEN
   // Temperature Indicator Enable bit(2)
   // Value Description
   // 1 Temperature Indicator is enabled
   // 0 Temperature Indicator is disabled
   FVRCONbits.TSEN = 0;
   
   // Bit 4 ? TSRNG
   // Temperature Indicator Range Selection bit(2)
   // Value Description
   // 1 VOUT = VDD - 4Vt (High Range)
   // 0 VOUT = VDD - 2Vt (Low Range)
   FVRCONbits.TSRNG = 0;
   
   // Bits 3:2 ? CDAFVR[1:0]?Comparator & DAC FVR Buffer Gain Selection bits
   // Value Description
   // 11 Comparator FVR Buffer Gain is 4x, (4.096V)(1)
   // 10 Comparator FVR Buffer Gain is 2x, (2.048V)(1)
   // 01 Comparator FVR Buffer Gain is 1x, (1.024V)
   // 00 Comparator FVR Buffer is off
   FVRCONbits.CDAFVR = 0b01;
   
   // Bits 1:0 ? ADFVR[1:0]?ADC FVR Buffer Gain Selection bit
   // Value Description
   // 11 ADC FVR Buffer Gain is 4x, (4.096V)(1)
   // 10 ADC FVR Buffer Gain is 2x, (2.048V)(1)
   // 01 ADC FVR Buffer Gain is 1x, (1.024V)
   // 00 ADC FVR Buffer is off
   FVRCONbits.ADFVR = 0b00;

   //---
   while (FVRCONbits.FVRRDY == 0)
   {
      // wait for the Reference Voltage to be initalised and stable
   }
   //---
}
//---------------------------------------------------
void initialiseIoBitsAndPieces(void)
{
   //---
   TRISBbits.TRISB4 = 1;   // P_PRES input
   //---
   // ACP RD5 - input with pullup
   TRISDbits.TRISD5 = 1;   // ACP input  
   // CHRG RD0 - input with pullup
   TRISDbits.TRISD0 = 1;   // CHRG input
   //---
   WPUD = 0b00100001;  // weak pullups for charging ACP & CHRG
   //---
}
//---------------------------------------------------
void initialiseBitBashSpi(void)
{
   //---------------------------
   // Bit bash SPI software port for 16bit ADC
   //---------------------------
   // S_CS - uP output - peripheral input
   S_CS = 1;
   TRISAbits.TRISA4 = 0;
   //---
   // S_SCLK - uP output - peripheral input
   S_SCLK = 0;   
   TRISAbits.TRISA3 = 0;
   //---
   // S_DOUT - uP input - peripheral output   
   TRISAbits.TRISA1 = 1;
   //---
   // S_DIN - uP output - peripheral input
   S_DIN = 0;
   TRISAbits.TRISA2 = 0;
   //---
   // S_DRDY - uP input - peripheral output
   WPUA = 0b00100000;
   TRISAbits.TRISA5 = 1;
   //---
}
//---------------------------------------------------
void initialiseBlueToothIo(void)
{
   //---
   TRISCbits.TRISC6 = 0;   // TX is an input, was output
   RC6PPS = 0x09;          // TX1 on RC6 // note this is the startup value so don't need to do the unlock sequence
   //---
   TRISCbits.TRISC7 = 1;   // RX is an input TRISCbits.TRISC7 = 1;
   RX1PPS = 0x17;          // RX1 on RC7 // note this is the startup value so don't need to do the unlock sequence 
   //---
   TRISCbits.TRISC5 = 1;   // BT CTS - is an input to uP
   //---
   LATCbits.LATC4 = 0;     
   TRISCbits.TRISC4 = 0;   // BT CTS / uP RTS - is an output from uP - an input on the BLE121LR - is controlled by UART
   //---
   LATDbits.LATD2 = 0;     // was 0 
   TRISDbits.TRISD2 = 0;   // BT Reset pin - is an output from uP  
   //---
}
//---------------------------------------------------
unsigned char ifDcPlugInserted(void)
{
   unsigned char n;
   
   //---
   LATAbits.LATA6 = 1;
   TRISAbits.TRISA6 = 0;   // IN_PWR driver
   //---
   if (systemClockSpeed == CLOCK_SPEED_LOW)
   {
      for (n = 0 ; n < 10 ; n++)
         NOP();
   }
   else
   {
      for (n = 0 ; n < 200 ; n++)
         NOP();
   }
   //---
   if ((PORTB & 0b00010000) == 0b00010000)
   {
      TRISAbits.TRISA6 = 1;
      return YES;
   }
   else
   {
      TRISAbits.TRISA6 = 1;
      return NO;
   }
   //---
}
//---------------------------------------------
void analyseBleRxIn(void)
{
   uint8 data;
   
   if (bleRxIntoBuffer != bleRxOutOfBuffer)
   {
      //---------------------------------------------
      PIE0bits.TMR0IE = 0;
      data = bleRxBuffer[bleRxOutOfBuffer];
      if (++bleRxOutOfBuffer >= BUFFER_SIZE)
         bleRxOutOfBuffer = 0;
      --bleBufferDepthCount;
      PIE0bits.TMR0IE = 1;
      //---------------------------------------------
      XReceiveBTMessage(data);
      //---------------------------------------------
   }
}
//---------------------------------------------
// Note the compiler will issue a warning for this code as "not called". 
// ignore this warning.
//---------------------------------------------
void interrupt_normal_code(void)
{
   uint8 data;
   
   //---
   // bit 2 FERR: Framing Error bit
   // 1 = Framing error (can be updated by reading RCREGx register and receive next valid byte)
   // 0 = No framing error  
   if (RCSTA1bits.FERR == 1)
   {
      data = RCREG1;
      return;
   } 
   //---
   // bit 1 OERR: Overrun Error bit
   // 1 = Overrun error (can be cleared by clearing bit CREN)
   // 0 = No overrun error
   if (RCSTA1bits.OERR == 1)
   {
      RCSTA1bits.CREN = 0;
      RCSTA1bits.CREN = 1;
      return;
   }
   //---        
   if (PIR3bits.RC1IF == 1)
   {
      bleRxBuffer[bleRxIntoBuffer] = RC1REG; 
      if (++bleRxIntoBuffer >= BUFFER_SIZE)
         bleRxIntoBuffer = 0;
      ++bleBufferDepthCount;
   }
   //---
   if (PIR0bits.TMR0IF == 1)
   {
      //---
      // we have a timer interrupt at 16,000,000 / 256 / 64 = 976.5625 Hz ie close to a millisecond
      //---
      PIR0bits.TMR0IF = 0;
      //---
      if (++timer25mSec >= 25)
      {
         //---
         timer25mSec = 0;
         //---
         flag25mSec = TRUE;
         //---
         if (++blueTimer >= 10000)
            blueTimer = 10000;
         //---
         if (solidColourTimer != 0)
            --solidColourTimer;
         //---
         if (++periodTimer >= BLUETIMER_0d125_SECOND)
         {
            periodTimer = 0;
            periodFlag = 1;
         }
         //---
      }
      //---
      if (++ledCount >= 100)
      {
         ledCount = 0;
         if (++ledDisplayCount >= 20)
            ledDisplayCount = 0;
      }
      //---
      if (++secTimeCount >= 977)
      {
         secTimeCount = 0;
         secTimeFlag = 1;
      }
      //---
   }
   //---
}

//###########################################################################
//  BootLoader Modifications
//###########################################################################
//---------------------------------------------
// do not touch this code!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Interrupt structure must not change
// has to be a single call sub-routine to the interrupt code above
// You can change the code above in the sub-routine, just don't touch the code below.
//---------------------------------------------
void __interrupt () my_isr (void)
{
   interrupt_normal_code();
}
//---------------------------------------------
//###########################################################################