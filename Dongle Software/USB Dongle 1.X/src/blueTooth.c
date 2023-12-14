#include <xc.h>
#include <string.h>
#include <stdint.h> // for Uint8/16/32 and Int8/16/32 data types
#include "MCU_Layer.h"
#include "const.h"
#include "eveLowLevel.h"
#include "eve.h"

//---------------------------------------------
extern unsigned int        debounce;
extern unsigned char       screen;
extern long                dummyValue[4];
extern unsigned char       id[4];
extern unsigned int        maxValue[4];
extern unsigned int        minValue[4];
extern char                StringArray[10];
extern unsigned char       unitSelect;
extern char                sensorNames[4][30];
extern char                numberNames[4][4];
extern unsigned long int   sensorColours[4];
extern unsigned char       sensorStatus[4];
//extern unsigned char       unitMinMax;
extern unsigned int        xBattery;
extern unsigned int        yBattery;
extern unsigned char       sensorSelected[4];
extern uint16_t            xPosition;
extern uint16_t            yPosition;
extern volatile int16_t    timer25mSec;
extern uint16_t            touchAction;
extern int8_t              sunlightMode;
extern int8_t              testDemo;
extern char                sensorIds[4][7];
extern int8_t              editId;
//---------------------------------------------
void blueTooth(void)
{
  unsigned int x[4], y[4], xIndent, yIndent, xSpacing, statusWidth, statusHeight, statusYoffset, textYoffset, testButtonYindent, testButtonWidth, testButtonHeight;
  unsigned int buttonBoxHeight, buttonBoxWidth, buttonRadius;
  unsigned char n;
  char numberString[][4] = {"1", "2", "3", "4"};
  unsigned int switchesYIndent, switchesWidth, switchesHeight, switchesXIndent, switchesGap; 
  unsigned int swAx1, swAx2, swAy1, swAy2, swBx1, swBx2, swBy1, swBy2, edge;
  unsigned int lineExpand;
  //---
  WriteCmd(CMD_DLSTART); 
  setBackgroundColour(BLACK);
  Clear(1, 1, 1);
  //---
  xIndent = 20;
  xSpacing = 118;
  yIndent =  90;
  //---
  for (n = 0 ; n < 4 ; n++)
  {
    x[n] = xIndent + (xSpacing * n);
    y[n] = yIndent;
  }
  //---
  edge = 3;
  //---
  buttonBoxWidth = 86;
  buttonBoxHeight = 60;
  buttonRadius = 3;
  statusWidth = buttonBoxWidth;
  statusHeight = 4;
  statusYoffset = 16;
  textYoffset = 28;
  testButtonYindent = 16;
  testButtonWidth = 380;
  testButtonHeight = 44;
  //---
  switchesXIndent = 10;
  switchesYIndent = 216;
  switchesWidth = 380;
  switchesHeight = 38;
  switchesGap = 12;
  //---
  lineExpand = 2;
  //---
  swAx1 = switchesXIndent;
  swAx2 = swAx1 + switchesWidth;
  swAy1 = switchesYIndent;
  swAy2 = swAy1 + switchesHeight;
  //---
  swBx1 = xIndent;
  swBx2 = swBx1 + switchesWidth;
  swBy1 = switchesYIndent;
  swBy2 = swBy1 + switchesHeight;
  //---
  displayButton(xIndent, 16, 480 - (xIndent * 2), 38, 3, WHITE, BLACK, "EDIT SENSOR ID", 30);
  //---
  displayButton(swBx1 - lineExpand, swBy1 - lineExpand, switchesWidth + (lineExpand * 2), switchesHeight + (lineExpand * 2), 3, WHITE, WHITE, "", 27);
  displayButton(swBx1, swBy1, switchesWidth, switchesHeight, 3, BLACK, WHITE, "BACK", 27);
  //---
  drawBattery(xBattery, yBattery, ON_BLACK);
  //drawBattery2(420, 31);
  //---
  for (n = 0 ; n < 4 ; n++)
  {
    //---
    drawRectangle(x[n], y[n], x[n] + buttonBoxWidth, y[n] + buttonBoxHeight, buttonRadius, sensorColours[n]);
    displayButton(x[n] + edge, y[n] + edge, buttonBoxWidth - (2 * edge), buttonBoxHeight - (2 * edge), buttonRadius, sensorColours[n], BLACK, numberString[n], 31);
    //---
    setColour(WHITE);
    Cmd_Text(x[n] + (buttonBoxWidth / 2), y[n] + buttonBoxHeight + textYoffset, 27, FT_OPT_CENTER, sensorIds[n]);
    //---
  }
  //---
  // BACK BUTTON
  if (((touchTest(swBx1, swBy1, switchesWidth, switchesHeight) == TRUE) && (touchAction == SET)))
  {
    touchAction = RESET;
    screen = SELECT_SENSORS;
  }
  //---
  DLEnd();
  Finish();  
  //--- 
  for (n = 0 ; n < 4 ; n++)
  {
    if ((touchTest(x[n], y[n], buttonBoxWidth, buttonBoxHeight) == TRUE) && (touchAction == SET))
      {
        touchAction = RESET;
        editId = n;
        screen = EDIT_ID;
      }
  }
  //---
}
