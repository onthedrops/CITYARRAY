/*******************************************************************************
  File Name:
    bcm.h

  Summary:
  Binary Code Modulation header

  Description:

 *******************************************************************************/
#ifndef _BCM_H
#define _BCM_H

#ifdef  __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "global_variables.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define FIRST_BCM_BIT_TICK_COUNT 15 //15us for the 1st brightness bit

#define BCM_Is_Ready() BCM_Bit_Complete_Flag_Get()

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */
typedef struct
{
  unsigned int bcmRow;
  volatile int bcmCurrentBit;
  volatile bool bcmBitCompleteFlag;
  int bcmTimerTickCount;
  hw_timer_t *bcmTimer;
  //portMUX_TYPE bcmTimerMux = portMUX_INITIALIZER_UNLOCKED;
}BCM_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void BCM_Bit_Complete_Flag_Set();
void BCM_Bit_Complete_Flag_Clear();
bool BCM_Bit_Complete_Flag_Get();
int BCM_Current_Bit_Get();
void BCM_Tmr_Continue();
uint32_t BCM_Row_Get();

void BCM_Initialize(void);

#ifdef  __cplusplus
}
#endif
#endif /* _BCM_H */
/*******************************************************************************
 End of File
*/
