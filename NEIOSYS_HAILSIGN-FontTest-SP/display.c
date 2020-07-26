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
//#include "m38_font.h"
//#include "Minimum_font.h"
//#include "font7x5.h"
//#include "crackers_font.h"
//#include "minimumplus1.h"
//#include "HUNTER_16.h"
//#include "hunter.h"
#include "arialbd_font_manual.h"
//#include "ChunkFive-Regular_font.h"

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

/*
typedef struct {
  uint32_t *dataPtr;
  uint32_t nRows; // 16
  uint32_t nColumns; // width
}DISPLAY_BITMAP_1BIT;
*/

/*
* A1 A2 A3 A4  A5 A6 A7 A8
* B1 B2 B3 B4  B5 B6 B7 B8
* C1 C2 C3 C4  C5 C6 C7 C8
* D1 D2 D3 D4  D5 D6 D7 D8
* E1 E2 E3 E4  E5 E6 E7 E8
* F1 F2 F3 F4  F5 F6 F7 F8
* G1 G2 G3 G4  G5 G6 G7 G8
* H1 H2 H3 H4  H5 H6 H7 H8
*/

/*
* byte 0  & 0x01 
*
*
*
*
*
*/

/*
* H1 G1 F1 E1 D1 C1 B1 A1
* H2 G2 F2 E2 D2 C2 B2 A2
* H3 G3 F3 E3 D3 C3 B3 A3
*
*
*
*
*
*
*/
void DISPLAY_Write_String_1Bit(DISPLAY_BITMAP_1BIT bitmap, char *string)
{
        int rowPtr = 0;
        int stringPtr;
	int colPtr;
	int startPtr = 0;

	uint32_t *dptr = bitmap.dataPtr;
	uint32_t *eptr = bitmap.dataPtr;
	int charPtr = 3;


        for(stringPtr = 0; stringPtr < strlen(string); stringPtr++) {
                char c = string[stringPtr];

		// translate character
		char t[8] = { 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00 };

		t[0] = (font[c-32][7] & 0x01) | ((font[c-32][6] & 0x01) << 1) | ((font[c-32][5] & 0x01) << 2) | ((font[c-32][4] & 0x01) << 3) | ((font[c-32][3] & 0x01) << 4) | ((font[c-32][2] & 0x01) << 5) | ((font[c-32][1] & 0x01) << 6) | ((font[c-32][0] & 0x01) << 7);

		t[1] = ((font[c-32][7] & 0x02) >> 1)  | (font[c-32][6] & 0x02) | ((font[c-32][5] & 0x02) << 1) | ((font[c-32][4] & 0x02) << 2) | ((font[c-32][3] & 0x02) << 3) | ((font[c-32][2] & 0x02) << 4) | ((font[c-32][1] & 0x02) << 5) | ((font[c-32][0] & 0x02) << 6);
		
		t[2] = ((font[c-32][7] & 0x04) >> 2)  | ((font[c-32][6] & 0x04) >> 1)  | ((font[c-32][5] & 0x04)) | ((font[c-32][4] & 0x04) << 1) | ((font[c-32][3] & 0x04) << 2) | ((font[c-32][2] & 0x04) << 3) | ((font[c-32][1] & 0x04) << 4) | ((font[c-32][0] & 0x04) << 5);

		t[3] = ((font[c-32][7] & 0x08) >> 3)  | ((font[c-32][6] & 0x08) >> 2)  | ((font[c-32][5] & 0x08) >> 1) | ((font[c-32][4] & 0x08)) | ((font[c-32][3] & 0x08) << 1) | ((font[c-32][2] & 0x08) << 2) | ((font[c-32][1] & 0x08) << 3) | ((font[c-32][0] & 0x08) << 4);

		t[4] = ((font[c-32][7] & 0x10) >> 4)  | ((font[c-32][6] & 0x10) >> 3)  | ((font[c-32][5] & 0x10) >> 2) | ((font[c-32][4] & 0x10) >> 1) | ((font[c-32][3] & 0x10)) | ((font[c-32][2] & 0x10) << 1) | ((font[c-32][1] & 0x10) << 2) | ((font[c-32][0] & 0x10) << 3);

		t[5] = ((font[c-32][7] & 0x20) >> 5)  | ((font[c-32][6] & 0x20) >> 4)  | ((font[c-32][5] & 0x20) >> 3) | ((font[c-32][4] & 0x20) >> 2) | ((font[c-32][3] & 0x20) >> 1) | ((font[c-32][2] & 0x20)) | ((font[c-32][1] & 0x20) << 1) | ((font[c-32][0] & 0x20) << 2);

		t[6] = ((font[c-32][7] & 0x40) >> 6)  | ((font[c-32][6] & 0x40) >> 5)  | ((font[c-32][5] & 0x40) >> 4) | ((font[c-32][4] & 0x40) >> 3) | ((font[c-32][3] & 0x40) >> 2) | ((font[c-32][2] & 0x40) >> 1) | ((font[c-32][1] & 0x40)) | ((font[c-32][0] & 0x40) << 1);

		t[7] = ((font[c-32][7] & 0x80) >> 7)  | ((font[c-32][6] & 0x80) >> 6)  | ((font[c-32][5] & 0x80) >> 5) | ((font[c-32][4] & 0x80) >> 4) | ((font[c-32][3] & 0x80) >> 3) | ((font[c-32][2] & 0x80) >> 2) | ((font[c-32][1] & 0x80) >> 1) | ((font[c-32][0] & 0x80));
		

		
		dptr = eptr;

		for(colPtr=0;colPtr<8;colPtr++)
		{
			*dptr |= t[colPtr] << (8 * charPtr);
			dptr +=  4;
		}

		charPtr--;

		if(charPtr == -1) {
			charPtr = 3;
			eptr += 32;
		}

        }
}

DISPLAY_BITMAP_1BIT *Convert_SBitmap(DISPLAY_SBITMAP_1BIT *bitmap)
{
  // the assumption here for now is we're dealing with a single sign
  // later we can work on rolling signs either 

  int cols = bitmap->nColumns;
  if(cols < 64) {
     cols = 64;
  }
  
  DISPLAY_BITMAP_1BIT *bptr = malloc(sizeof(DISPLAY_BITMAP_1BIT));
  // the logic here is that each uint32_t stores 32 LED states and each column is 16 LED states
  bptr->dataPtr = malloc(sizeof(uint32_t) * (cols / 2));
  bptr->nColumns = cols;
  bptr->nRows = 16;
  // the logic here is there are two bytes per column
  memset(bptr->dataPtr, 0, cols * 4);

  // now we walk the bitmap
  int i;
  int j;
  int charPtr = 0;
  uint32_t *dptr = bptr->dataPtr;
  uint32_t *eptr = bptr->dataPtr;
  //uint32_t *eptr;
  char s[64];
  
 //   for(i=0;i<bitmap->nColumns;i++) {
   for(i=bitmap->nColumns;i>=0;i--) {
      
  //for(i=0;i<2;i++) {
    dptr = eptr;
    
    for(j=0;j<16;j++) { // for each column in the array
       uint16_t *p = bitmap->dataPtr;
       p += i;

       
       if(*p & ( 1 << j )) { // if the column is turned on
               sprintf(s,"i: %d j: %d on (%d) CP %d",i,j,*p);
              slog(s);
    
          *dptr |= (1 << charPtr);
       }      
       
         dptr += (cols / 32);
      }
      
      charPtr++;
      if(charPtr > 31) {
        charPtr = 0;
        eptr++;
      }
    } 
  
 /* dptr = bptr->dataPtr;
  *dptr |= 0x0001;
  dptr++;
  *dptr |= 0x0001;
  dptr++;
  *dptr |= 0x0001;
  */
 // *dptr |= 0x0000;
  return bptr;
}

DISPLAY_SBITMAP_1BIT *Write_String_1Bit(char *string)
{
      int width = 12;
      int arraySize = sizeof(uint16_t) * (strlen(string) * (width + 1));
      
      uint16_t *stringArray = malloc(arraySize);
      memset(stringArray, 0, arraySize);
      DISPLAY_SBITMAP_1BIT *bitmap = malloc(sizeof(DISPLAY_SBITMAP_1BIT));
      bitmap->dataPtr = stringArray;
      bitmap->nColumns = 0;
      int i;
      int stringPtr;
      
      
     for(stringPtr = 0; stringPtr < strlen(string); stringPtr++) {
         char c = string[stringPtr];
         for(i=0;i<width;i++) {
            if(font[c-32][i]  || c == ' ') {
              *stringArray |= font[c-32][i];
              stringArray++;
              bitmap->nColumns++;
            }
         }
         stringArray++;
         bitmap->nColumns++;
         
      }

    return bitmap;
}

DISPLAY_SBITMAP_1BIT *Write_2HString_1Bit(char *string)
{
      int arraySize = sizeof(uint16_t) * (strlen(string) * 9);
      
      uint16_t *stringArray = malloc(arraySize);
      memset(stringArray, 0, arraySize);
      DISPLAY_SBITMAP_1BIT *bitmap = malloc(sizeof(DISPLAY_SBITMAP_1BIT));
      bitmap->dataPtr = stringArray;
      bitmap->nColumns = 0;
      int i;
      int stringPtr;
      int width = 6;
      
      
     for(stringPtr = 0; stringPtr < strlen(string); stringPtr++) {
         char c = string[stringPtr];
        
         for(i=0;i<width;i++) { 
            *stringArray |= font[c-32][i];
            stringArray++;
         }
         stringArray++;
         bitmap->nColumns+= (width + 1);
     }

    return bitmap;
}


Update_Bitmap_Window(DISPLAY_BITMAP_1BIT screen,DISPLAY_SBITMAP_1BIT *image, int offset)
{
    int cols = 64;
    int local_end = 0;
    
//    if((image->nColumns - offset) > 64) {
       if(offset > 64) {
        local_end = offset - 64;
       } else {
        local_end = 0;
       }
 //   } else {
   //   local_end = ;
  // }

//    char s[64];
  //  sprintf(s,"offset %d le: %d",offset, local_end);
   // slog(s);
    
    memset(screen.dataPtr, 0, cols * 4);
    // now we walk the bitmap
 
  int i;
  int j;
  int charPtr = 0;
  uint32_t *dptr = screen.dataPtr;
  uint32_t *eptr = screen.dataPtr;
  
 //   for(i=0;i<bitmap->nColumns;i++) {
   for(i=offset;i>local_end;i--) {
      
  //for(i=0;i<2;i++) {
    dptr = eptr;
    
    for(j=0;j<16;j++) { // for each column in the array
       uint16_t *p = image->dataPtr;
       p += i;

       
       if(*p & ( 1 << j )) { // if the column is turned on
    //           sprintf(s,"i: %d j: %d on (%d) CP %d",i,j,*p);
      //        slog(s);
    
          *dptr |= (1 << charPtr);
       }      
       
         dptr += (cols / 32);
      }
      
      charPtr++;
      if(charPtr > 31) {
        charPtr = 0;
        eptr++;
      }
    } 

}

 
/*******************************************************************************
 End of File
*/
