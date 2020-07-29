/*******************************************************************************
  File Name:
    display.c

  Summary:
  Board Support Package

  Description:

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c)
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "bcm.h"
#include "bsp.h"
#include "display.h"

// *****************************************************************************
// *****************************************************************************
// Section: 
// *****************************************************************************
// *****************************************************************************

DISPLAY_DATA displayData;


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void DISPLAY_Bitmap_8Bit(DISPLAY_BITMAP_8BIT bitmap)
{
  uint32_t i = 0;
  uint32_t bcmBit;
  uint32_t row;
  uint8_t *dataOffsetPtr;
  bool rBit, gBit;
    
  row = BCM_Row_Get();
  if(bitmap.nRows <= row)//do not have data for current row (bitmap height is smaller than module height)
    return;
    
  dataOffsetPtr = bitmap.dataPtr + row * bitmap.nColumns;
  bitmap.nColumns = MIN(bitmap.nColumns, SIGN_RESOLUTION_X);
  bcmBit = BCM_Current_Bit_Get();
  while(i < bitmap.nColumns)
  {
    rBit = !bit_get(*dataOffsetPtr, BIT(bcmBit));
    gBit = !bit_get(*dataOffsetPtr, BIT(BRIGHTNESS_PRECISION_BITS + bcmBit));
    column_shift_single(rBit, gBit);
    dataOffsetPtr++;
    i++;
  }
  row_off();
  row_data_latch();
  turn_on_row(row);
}

//Red and Green columns must be the same, if not, it will only display up to the one that has less columns
void DISPLAY_Bitmap_1Bit(DISPLAY_BITMAP_1BIT rBitmap, uint8_t rBrightness, DISPLAY_BITMAP_1BIT gBitmap, uint8_t gBrightness)
{
  static uint32_t zero = 0;
  uint32_t *rDataPtr = &zero;
  uint32_t *gDataPtr = &zero;
  uint32_t rFlag = 1;
  uint32_t gFlag = 1;
  uint32_t nColumns;
  uint32_t row;
  uint32_t bcmBit;

  
  row = BCM_Row_Get();
  nColumns = MIN(rBitmap.nColumns, gBitmap.nColumns);
  bcmBit = BCM_Current_Bit_Get();

  if(bit_get(rBrightness, BIT(bcmBit)) && (rBitmap.nRows > row))
    rDataPtr = rBitmap.dataPtr + row*(rBitmap.nColumns/32);
  else
    rFlag = 0;
    
  if(bit_get(gBrightness, BIT(bcmBit)) && (gBitmap.nRows > row))
    gDataPtr = gBitmap.dataPtr + row*(gBitmap.nColumns/32);
  else
    gFlag = 0;    
    
  while(nColumns>0)
  {
    column_shift(WORD_BIT_FLIP(*rDataPtr), WORD_BIT_FLIP(*gDataPtr), nColumns);
    nColumns -= MIN(nColumns, 32);
    rDataPtr += rFlag;
    gDataPtr += gFlag;
  }
  row_off();
  row_data_latch();
  turn_on_row(row);
}

void Bitmap1_Scroll(DISPLAY_BITMAP_1BIT bitmap, SCROLL_DIRECTION dir, unsigned int nBits, bool loop)
{
  int row = 0;
  unsigned int nColumns = bitmap.nColumns;
  unsigned int *dataPtr = bitmap.dataPtr;
  unsigned int tempWordA;
  unsigned int tempWordB;
  
  nBits = MIN(nBits, 32);//will not scroll more than 32 bits at a time
  
  for(row = 0; row < bitmap.nRows; row++)
  {
    switch(dir)
    {
      case LEFT:
        if(loop)
            tempWordA = *(bitmap.dataPtr + (row+1)*bitmap.nColumns/32-1);
        else
            tempWordA = 0;
            
        while(nColumns > 0)
        {
            tempWordB = *dataPtr;
            *dataPtr = ((long)(*dataPtr)<<nBits) | (tempWordA>>(32-nBits));
            tempWordA = tempWordB;
            dataPtr++;
            nColumns -= MIN(nColumns,32);
        }
        break;          
          
      case RIGHT:
        if(loop)
            tempWordA = *dataPtr;
        else
            tempWordA = 0;
        while(nColumns > 32)
        {
            *dataPtr = ((long)(*dataPtr)>>nBits) | ((long)(*(dataPtr+1))<<(32-nBits));
            dataPtr++;
            nColumns -= MIN(nColumns,32);
        }
        *dataPtr = ((long)(*dataPtr)>>nBits) | (tempWordA<<(32-nBits));
        dataPtr++;
        break;
      
      default:
        break;        
    }
    nColumns = bitmap.nColumns;
  }
}

// *****************************************************************************
// *****************************************************************************
// Section: 
// *****************************************************************************
// *****************************************************************************

void DISPLAY_Initialize(void)
{
  displayData.state = DISPLAY_STATE_INIT;
  displayData.displayMethod = BITMAP_1BIT;  
  displayData.bitmap8Bit = (DISPLAY_BITMAP_8BIT){NULL, 0, 0};  
  displayData.bitmap1BitRed = (DISPLAY_BITMAP_1BIT){NULL, 0, 0};
  displayData.bitmap1BitGreen = (DISPLAY_BITMAP_1BIT){NULL, 0, 0};
  displayData.rBrightness = 0;
  displayData.gBrightness = 0;
}

void DISPLAY_Task(void)
{
  BCM_Tick();
  
  switch(displayData.state)
  {
    case DISPLAY_STATE_INIT:
      displayData.state = DISPLAY_STATE_BCM_CHECK;
      break;
      
    case DISPLAY_STATE_BCM_CHECK:
      if(BCM_Is_Ready())
      {
        BCM_Bit_Complete_Flag_Clear();
        shift_registers_clear();
        if(displayData.displayMethod == BITMAP_8BIT)        
          displayData.state = DISPLAY_STATE_ROW_PUT_8BIT;
        else          
          displayData.state = DISPLAY_STATE_ROW_PUT_1BIT;
      }
      break;
      
    case DISPLAY_STATE_ROW_PUT_1BIT:
      DISPLAY_Bitmap_1Bit(displayData.bitmap1BitRed, displayData.rBrightness, displayData.bitmap1BitGreen, displayData.gBrightness);
      //BCM_Tmr_Continue();
      displayData.state = DISPLAY_STATE_BCM_CHECK;
      break;
      
    case DISPLAY_STATE_ROW_PUT_8BIT:
      DISPLAY_Bitmap_8Bit(displayData.bitmap8Bit);
      //BCM_Tmr_Continue();
      displayData.state = DISPLAY_STATE_BCM_CHECK;
      break;

    default:
      break;
  }
}

void DISPLAY_Bitmap_Put_8Bit(uint8_t *bDataPtr, uint32_t nRows, uint32_t nColumns)
{
  displayData.bitmap8Bit.dataPtr = bDataPtr;
  displayData.bitmap8Bit.nRows = nRows;
  displayData.bitmap8Bit.nColumns = nColumns;
}


void DISPLAY_Bitmap_Put_1Bit(DISPLAY_BITMAP_1BIT rBitmap, DISPLAY_BITMAP_1BIT gBitmap)
{  
  displayData.bitmap1BitRed = rBitmap;
  displayData.bitmap1BitGreen = gBitmap;
}

void DISPLAY_Method_Set(DISPLAY_METHOD displayMethod)
{
  displayData.displayMethod = displayMethod;
}

DISPLAY_METHOD DISPLAY_Method_Get(void)
{
  return displayData.displayMethod;
}

void DISPLAY_Brightness_Set(uint8_t rBrightness, uint8_t gBrightness)
{  
  displayData.rBrightness = rBrightness;
  displayData.gBrightness = gBrightness;
}

/*******************************************************************************
 End of File
*/
