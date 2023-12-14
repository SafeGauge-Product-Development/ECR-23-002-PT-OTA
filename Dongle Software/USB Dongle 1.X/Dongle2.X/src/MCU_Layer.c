//---------------------------------------------
#include <xc.h>
#include <string.h>
#include <stdint.h> // for Uint8/16/32 and Int8/16/32 data types
#include "const.h"
#include "apitypes.h"
//---------------------------------------------
extern volatile int8     bleTxBuffer[BUFFER_SIZE];
extern volatile uint16   bleTxIntoBuffer;
extern volatile uint16   bleTxOutOfBuffer;

extern volatile int8     commsRxBuffer[BUFFER_SIZE];
extern volatile uint16   commsRxIntoBuffer;
extern volatile uint16   commsRxOutOfBuffer;
extern volatile int8     commsTxBuffer[BUFFER_SIZE];
extern volatile uint16   commsTxIntoBuffer;
extern volatile uint16   commsTxOutOfBuffer;
//--------------------------------------------
void initialiseTimer1(void)
{
   T1CON = 0;                 // Clear Timer 1 configuration
   T1CONbits.TCS = 0;         // Fosc / 2 , internal clock
   T1CONbits.TCKPS = 0b00;    // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256) divide by 8
   PR1 = 16000;               // Set Timer 1 period (max value is 65535) 1 mSec interrupt
   _T1IP = 1;                 // Set Timer 1 interrupt priority
   _T1IF = 0;                 // Clear Timer 1 interrupt flag
   _T1IE = 1;                 // Enable Timer 1 interrupt
   T1CONbits.TON = 1;         // Turn on Timer 1
}
//---------------------------------------------
void initialiseUart1(void)
{
   //--------------------------------------------
   // Setup UART for BLE121LR
   //--------------------------------------------  
   
   //bit 15 UARTEN: UARTx Enable bit(1)
   //1 = UARTx is enabled; all UARTx pins are controlled by UARTx as defined by UEN<1:0>
   //0 = UARTx is disabled; all UARTx pins are controlled by port latches; UARTx power consumption is minimal
   U1MODEbits.UARTEN = 1; 
   
   //bit 13 USIDL: Stop in Idle Mode bit
   //1 = Discontinue module operation when the device enters Idle mode
   //0 = Continue module operation in Idle mode
   U1MODEbits.USIDL = 0;
   
   //bit 12 IREN: IrDA® Encoder and Decoder Enable bit(2)
   //1 = IrDA encoder and decoder enabled
   //0 = IrDA encoder and decoder disabled
   U1MODEbits.IREN = 0;
   
   //bit 11 RTSMD: Mode Selection for UxRTS Pin bit
   //1 = UxRTS pin in Simplex mode
   //0 = UxRTS pin in Flow Control mode
   U1MODEbits.RTSMD = 0;
   
   //bit 10 Unimplemented: Read as ?0?
   
   //bit 9-8 UEN<1:0>: UARTx Enable bits
   //11 = UxTX, UxRX and BCLKx pins are enabled and used; UxCTS pin controlled by port latches
   //10 = UxTX, UxRX, UxCTS and UxRTS pins are enabled and used
   //01 = UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin controlled by port latches
   //00 = UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx pins controlled by port latches
   U1MODEbits.UEN = 0b10;
   
   //bit 7 WAKE: Wake-up on Start Bit Detect During Sleep Mode Enable bit
   //1 = UARTx will continue to sample the UxRX pin; interrupt generated on falling edge, bit cleared in
   //hardware on following rising edge
   //0 = No wake-up enabled
   U1MODEbits.WAKE = 1;
   
   //bit 6 LPBACK: UARTx Loopback Mode Select bit
   //1 = Enable Loopback mode
   //0 = Loopback mode is disabled
   U1MODEbits.LPBACK = 0;
   
   //bit 5 ABAUD: Auto-Baud Enable bit
   //1 = Enable baud rate measurement on the next character ? requires reception of a Sync field (55h);
   //cleared in hardware upon completion
   //0 = Baud rate measurement disabled or completed
   U1MODEbits.ABAUD = 0;
   
   //bit 4 RXINV: Receive Polarity Inversion bit
   //1 = UxRX Idle state is ?0?
   //0 = UxRX Idle state is ?1?
   U1MODEbits.RXINV = 0;
   
   //bit 3 BRGH: High Baud Rate Enable bit
   //1 = High-Speed mode (baud clock generated from FCY/4)
   //0 = Standard mode (baud clock generated from FCY/16)
   U1MODEbits.BRGH = 1;
   //U1BRG = 416; //9600 baud @ 32MHz clock frequency
   U1BRG = 68; //57600 baud @ 32MHz clock frequency
   //bit 2-1 PDSEL<1:0>: Parity and Data Selection bits
   //11 = 9-bit data, no parity
   //10 = 8-bit data, odd parity
   //01 = 8-bit data, even parity
   //00 = 8-bit data, no parity
   U1MODEbits.PDSEL = 0x00;
   
   //bit 0 STSEL: Stop Bit Selection bit
   //1 = Two Stop bits
   //0 = One Stop bit
   U1MODEbits.STSEL = 0; 
   
   //bit 15,13 UTXISEL<1:0> Transmission Interrupt Mode Selection bits
   //11 = Reserved
   //10 = Interrupt generated when a character is transferred to the Transmit Shift Register and the
   //transmit buffer becomes empty
   //01 = Interrupt generated when the last transmission is over (last character shifted out of the Transmit
   //Shift Register) and all the transmit operations are completed
   //00 = Interrupt generated when any character is transferred to the Transmit Shift Register (this implies
   //at least one location is empty in the transmit buffer)
   U1STAbits.UTXISEL0 = 0;
   U1STAbits.UTXISEL1 = 0;
   
   //bit 14 UTXINV: Transmit Polarity Inversion bit
   //IREN = 0:
   //1 = UxTX Idle state is ë0í
   //0 = UxTX Idle state is ë1í
   //IREN = 1:
   //1 = IrDA encoded UxTX Idle state is ë1í
   //0 = IrDA encoded UxTX Idle state is ë0í
   U1STAbits.UTXINV = 0;
   
   //bit 12 Unimplemented: Read as ë0í
   
   //bit 11 UTXBRK: Transmit Break bit
   //1 = UxTX pin is driven low regardless of transmitter state (Sync Break transmission ñ Start bit followed
   //by twelve ë0ís and followed by a Stop bit)
   //0 = Sync Break transmission is disabled or completed
   U1STAbits.UTXBRK = 0; 
   
   //bit 9 UTXBF: Transmit Buffer Full Status bit (read-only)
   //1 = Transmit buffer is full
   //0 = Transmit buffer is not full, at least one more data word can be written
   
   //bit 8 TRMT: Transmit Shift Register is Empty bit (read-only)
   //1 = Transmit Shift Register is empty and transmit buffer is empty (the last transmission has completed)
   //0 = Transmit Shift Register is not empty, a transmission is in progress or queued in the transmit buffer
   
   //bit 7-6 URXISEL<1:0>: Receive Interrupt Mode Selection bits
   //11 = Interrupt flag bit is set when receive buffer is full (i.e., has 4 data characters)
   //10 = Interrupt flag bit is set when receive buffer is 3/4 full (i.e., has 3 data characters)
   //0x = Interrupt flag bit is set when a character is received
   U1STAbits.URXISEL = 0b00;
   
   //bit 5 ADDEN: Address Character Detect bit (bit 8 of received data = 1)
   //1 = Address Detect mode enabled. If 9-bit mode is not selected, this control bit has no effect.
   //0 = Address Detect mode disabled
   U1STAbits.ADDEN = 0;
   
   //bit 4 RIDLE: Receiver Idle bit (read-only)
   //1 = Receiver is Idle
   //0 = Data is being received
   
   //bit 3 PERR: Parity Error Status bit (read-only)
   //1 = Parity error has been detected for the current character
   //0 = Parity error has not been detected
   
   //bit 2 FERR: Framing Error Status bit (read-only)
   //1 = Framing error has been detected for the current character
   //0 = Framing error has not been detected
   
   //bit 1 OERR: Receive Buffer Overrun Error Status bit (clear/read-only)
   //1 = Receive buffer has overflowed
   //0 = Receive buffer has not overflowed (clearing a previously set OERR bit will reset the receiver buffer
   //and RSR to empty state)
   
   //bit 0 URXDA: Receive Buffer Data Available bit (read-only)
   //1 = Receive buffer has data, at least one more character can be read
   //0 = Receive buffer is empty
   
   IPC3bits.U1TXIP = 7;       // set the interrupt priority to the highest
   IPC2bits.U1RXIP = 7;       // set the interrupt priority to the highest
   
   _U1RXIP = 1;               // Set RX1 interrupt priority
   _U1RXIF = 0;               // Clear RX1 interrupt flag
   _U1RXIE = 1;               // Enable RX1 interrupt
  
   _U1TXIF = 0;               // Clear TX1 interrupt flag
   _U1TXIE = 0;               // disable TX1 interrupts
   
   //bit 10 UTXEN: Transmit Enable bit
   //1 = UARTx transmitter enabled, UxTX pin controlled by UARTx (if UARTEN = 1)
   //0 = UARTx transmitter disabled, any pending transmission is aborted and buffer is reset. UxTX pin
   //controlled by PORT.
   U1STAbits.UTXEN = 1;
}
//---------------------------------------------
void initialiseUart2(void)
{
   //--------------------------------------------
   // Setup UART for USB Comms
   //--------------------------------------------  
   U2MODEbits.UARTEN = 1; 
   U2MODEbits.USIDL = 0;
   U2MODEbits.IREN = 0;
   U2MODEbits.RTSMD = 1;
   U2MODEbits.UEN = 0b00;
   U2MODEbits.WAKE = 1;
   U2MODEbits.LPBACK = 0;
   U2MODEbits.ABAUD = 0;
   U2MODEbits.RXINV = 0;
   U2MODEbits.BRGH = 1;
   U2BRG = 68;
   U2MODEbits.PDSEL = 0x00;
   U2MODEbits.STSEL = 0; 
   U2STAbits.UTXISEL0 = 0;
   U2STAbits.UTXISEL1 = 0;
   U2STAbits.UTXINV = 0;
   U2STAbits.UTXBRK = 0; 
   U2STAbits.URXISEL = 0b00;
   U2STAbits.ADDEN = 0;
   IPC7bits.U2TXIP = 7;       // set the interrupt priority to the highest
   IPC7bits.U2RXIP = 7;       // set the interrupt priority to the highest 
   _U2RXIP = 1;               // Set RX1 interrupt priority
   _U2RXIF = 0;               // Clear RX1 interrupt flag
   _U2RXIE = 1;               // Enable RX1 interrupt
   _U2TXIF = 0;               // Clear TX1 interrupt flag
   _U2TXIE = 0;               // disable TX1 interrupts
   U2STAbits.UTXEN = 1;
}
//---------------------------------------------
void shootBleData(unsigned char data)
{	
	if (bleTxIntoBuffer == bleTxOutOfBuffer)
		{
			if (U1STAbits.UTXBF == 0)
				U1TXREG = data;
			else
				{
					IEC0bits.U1TXIE = 0;
					bleTxBuffer[bleTxIntoBuffer] = data;
					if (++bleTxIntoBuffer >= BUFFER_SIZE)
						bleTxIntoBuffer = 0;
					IEC0bits.U1TXIE = 1;
				}	
		}
	else
		{
			IEC0bits.U1TXIE = 0;
			bleTxBuffer[bleTxIntoBuffer] = data;
			if (++bleTxIntoBuffer >= BUFFER_SIZE)
				bleTxIntoBuffer = 0;
			IEC0bits.U1TXIE = 1;
		}
}
//---------------------------------------------
void shootCommsData(unsigned char data)
{	
	if (commsTxIntoBuffer == commsTxOutOfBuffer)
		{
			if (U2STAbits.UTXBF == 0)
				U2TXREG = data;
			else
				{
					IEC1bits.U2TXIE = 0;
					commsTxBuffer[commsTxIntoBuffer] = data;
					if (++commsTxIntoBuffer >= BUFFER_SIZE)
						commsTxIntoBuffer = 0;
					IEC1bits.U2TXIE = 1;
				}	
		}
	else
		{
			IEC1bits.U2TXIE = 0;
			commsTxBuffer[commsTxIntoBuffer] = data;
			if (++commsTxIntoBuffer >= BUFFER_SIZE)
				commsTxIntoBuffer = 0;
			IEC1bits.U2TXIE = 1;
		}
}
//---------------------------------------------
void mcuInit(void)
{
   //--------------------------------------------
   // AD Converter Input Pins, 1 = ADC , 0 = I/O
   //--------------------------------------------
   ANSA = 0b0000000000000000; 
   ANSB = 0b0000000000000000; 
   //--------------------------------------------
   // LEDs
   //--------------------------------------------
   _TRISA0 = 0; // Blue LED
   _TRISA1 = 0; // Red LED
   _TRISB2 = 0; // Green LED
   //---
   LED_RED; // power is on, has booted
   //--------------------------------------------
   // Set BLE pins
   //--------------------------------------------
   _LATB13 = 0;
   _TRISB13 = 0;      // RD0 to BLE121LR uP reset - is an output  (RST) - checked
   //---
   _LATB15 = 1;      // this sets the wake pin of the BLE121LR to 1 so it can't go into sleep mode
   _TRISB15 = 0;
   //--------------------------------------------
   //--- unlock ---
   OSCCON = 0x46;
   OSCCON = 0x57;
   OSCCONbits.IOLOCK = 0;
   //---
   // UART2 I/O Setup
   //---
   RPOR2 = 0x0500;     //  RP5 : RP4 
   RPINR19 = 0x0004;    //    U2CTS : U2RX
   //---
   // UART1 I/O Setup
   //---
   RPOR7 = 0x0003;      // this is for RP14 pin
   RPOR3 = 0x0400;      // this is for RP7 pin
   RPINR18 = 0x0608;    // U1CTS : U1RX
   //--------------------------------------------
   //--- configure oscillator ---
   _RCDIV = 0; 
   _CPDIV = 0b00; 
   _DOZE = 0b000;
   _DOZEN = 0;
   //--------------------------------------------
   //--- lock ---
   OSCCON = 0x46;
   OSCCON = 0x57;
   OSCCONbits.IOLOCK = 1;
   //--------------------------------------------
   _GIE = 1;
   //--------------------------------------------
}
//----------------------------------------------------------------------
