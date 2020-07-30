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
 
void IRAM_ATTR bcm_precision_bit_update(void) 
{  

#ifdef INTERRUPT_MODE
  timerAlarmDisable(bcmData.bcmTimer);  
#endif

  if(bcmData.bcmCurrentBit == BRIGHTNESS_PRECISION_BITS_MINUS_1)
  {
    bcmData.bcmCurrentBit = 0;//reset brightness bit back to 0 when reach the highest bit
    bcmData.bcmTimerTickCount = FIRST_BCM_BIT_TICK_COUNT;
    if(bcmData.bcmRow == MODULE_RESOLUTION_Y_MINUS_1)//
      bcmData.bcmRow = 0;
    else
      bcmData.bcmRow++;
  }
  else
  {    
    bcmData.bcmCurrentBit++;
    bcmData.bcmTimerTickCount <<= 1;//double tick counts when precision bit increase
  }  

    bcmData.bcmBitCompleteFlag = true;
  
#ifdef INTERRUPT_MODE
  timerAlarmWrite(bcmData.bcmTimer, bcmData.bcmTimerTickCount, false);
#endif

  timerWrite(bcmData.bcmTimer, 0);//reset timer count to 0

#ifdef INTERRUPT_MODE
  timerAlarmEnable(bcmData.bcmTimer);
#endif

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
#ifdef INTERRUPT_MODE
  timerAlarmWrite(bcmData.bcmTimer, bcmData.bcmTimerTickCount, false);
#endif
  
  timerWrite(bcmData.bcmTimer, 0);//reset timer count to 0
  
#ifdef INTERRUPT_MODE
  timerAlarmEnable(bcmData.bcmTimer);
#endif
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
#ifdef INTERRUPT_MODE
  timerAttachInterrupt(bcmData.bcmTimer, &bcm_precision_bit_update, true);
  timerAlarmWrite(bcmData.bcmTimer, bcmData.bcmTimerTickCount, false);
  timerAlarmEnable(bcmData.bcmTimer);
#endif
}

#ifndef INTERRUPT_MODE
void BCM_Tick(void)
{
  uint64_t timerValue = timerRead(bcmData.bcmTimer);
  if(timerValue >= bcmData.bcmTimerTickCount)
    bcm_precision_bit_update();
}

int BCM_OK(void)
{
  if(bcmData.bcmTimerTickCount > 2)
    return 1;
  return 0;
}

#else
int BCM_OK(void)
{
  return 1;
}
#endif

/*******************************************************************************
 End of File
*/
