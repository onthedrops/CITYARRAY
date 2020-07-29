/*******************************************************************************
  File Name:
    bcm.c

  Summary:
    Binary Code Modulation header

  Description:

 *******************************************************************************/



// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "bcm.h"
#include "display.h"

static BCM_DATA bcmData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
 
void bcm_precision_bit_update(void) 
{  
 // timerAlarmDisable(bcmData.bcmTimer);  
  if(bcmData.bcmCurrentBit == (BRIGHTNESS_PRECISION_BITS - 1))
  {
    bcmData.bcmCurrentBit = 0;//reset brightness bit back to 0 when reach the highest bit
    bcmData.bcmTimerTickCount = FIRST_BCM_BIT_TICK_COUNT;
    if(bcmData.bcmRow == (MODULE_RESOLUTION_Y - 1))//
      bcmData.bcmRow = 0;
    else
      bcmData.bcmRow++;
  }
  else
  {    
    bcmData.bcmCurrentBit++;
    bcmData.bcmTimerTickCount <<= 1;//double tick counts when precision bit increase
  }  
  BCM_Bit_Complete_Flag_Set();//bcmData.bcmBitCompleteFlag = true;
  
  //if(DISPLAY_Method_Get() == BITMAP_1BIT)
    BCM_Tmr_Continue();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void BCM_Bit_Complete_Flag_Set()
{
  bcmData.bcmBitCompleteFlag = true;
}

void BCM_Bit_Complete_Flag_Clear()
{
  bcmData.bcmBitCompleteFlag = false;
}

bool BCM_Bit_Complete_Flag_Get()
{
  return bcmData.bcmBitCompleteFlag;
}

int BCM_Current_Bit_Get()
{
  return bcmData.bcmCurrentBit;
}

uint32_t BCM_Row_Get()
{
  return bcmData.bcmRow;
}

void BCM_Tmr_Continue()
{  
  //timerAlarmWrite(bcmData.bcmTimer, bcmData.bcmTimerTickCount, false);
  timerWrite(bcmData.bcmTimer, 0);//reset timer count to 0
  //timerAlarmEnable(bcmData.bcmTimer);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void BCM_Initialize(void)
 */
void BCM_Initialize(void) //DO NOT CHANGE THE ORDER OF THESE INITIAL FUNCTIONS!!!
{
  bcmData.bcmCurrentBit = 0;
  bcmData.bcmTimerTickCount = FIRST_BCM_BIT_TICK_COUNT;
  bcmData.bcmTimer = timerBegin(0, 240, true);//prescale 240 (1 Mhz), CPU freq. 240MHz
  //timerAttachInterrupt(bcmData.bcmTimer, &bcm_precision_bit_update, true);
//  timerAlarmWrite(bcmData.bcmTimer, bcmData.bcmTimerTickCount, false);
//  timerAlarmEnable(bcmData.bcmTimer);
}

void BCM_Tick(void)
{
  uint64_t timerValue = timerRead(bcmData.bcmTimer);
  if(timerValue >= bcmData.bcmTimerTickCount)
    bcm_precision_bit_update();
}

/*******************************************************************************
 End of File
*/
