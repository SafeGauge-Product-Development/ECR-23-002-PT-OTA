
//---------------------------------------------
#include <xc.h>
#include "apitypes.h"
#include "const.h"
//---------------------------------------------
extern uint8               bleRxBuffer[BUFFER_SIZE];
extern volatile uint16     bleRxIntoBuffer;
extern volatile uint16     bleRxOutOfBuffer;
extern volatile uint16     bleBufferDepthCount;
extern volatile uint8      timer25mSec;
extern volatile uint8      flag25mSec;
extern volatile uint16     blueTimer;
extern volatile uint16     solidColourTimer;
extern volatile uint16     periodTimer;
extern volatile uint8      periodFlag;
extern volatile uint8      ledCount;
extern volatile uint8      ledDisplayCount;
extern uint8               secTimeFlag;
extern uint16              secTimeCount;
//---------------------------------------------
// Note the compiler will issue a warning for this code as "not called". 
// ignore this warning.
void __at(0xA008) interrupt_normal_code(void)
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
//---------------------------------------------