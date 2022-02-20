/*******************************************************************************
  File Name:
    display.h

  Summary:


  Description:

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Licesnse and Copyright Here
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef _DISPLAY_H
#define _DISPLAY_H

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
/* Application States

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum {
  LEFT,
  RIGHT
} SCROLL_DIRECTION;

typedef enum
{
  BITMAP_8BIT,
  BITMAP_1BIT
} DISPLAY_METHOD;

typedef enum
{
  DISPLAY_STATE_INIT,
  DISPLAY_STATE_BCM_CHECK,
  DISPLAY_STATE_ROW_PUT_1BIT,
  DISPLAY_STATE_ROW_PUT_8BIT
} DISPLAY_STATE;

typedef struct {
  uint8_t *dataPtr;
  uint32_t nRows;
  uint32_t nColumns;
} DISPLAY_BITMAP_8BIT;

typedef struct {
  uint32_t *dataPtr;
  uint32_t nRows;
  uint32_t nColumns;
} DISPLAY_BITMAP_1BIT;

typedef struct {
  uint16_t *dataPtr;
  uint32_t nColumns;
} DISPLAY_SBITMAP_1BIT;

typedef struct {
   uint16_t *dataPtr_red;
   uint16_t *dataPtr_green;
   uint32_t nColumns;
} DISPLAY_SBITMAP_2BIT;

typedef struct
{
  DISPLAY_STATE state;
  DISPLAY_METHOD displayMethod;
  DISPLAY_BITMAP_8BIT bitmap8Bit;
  DISPLAY_BITMAP_1BIT bitmap1BitRed;
  DISPLAY_BITMAP_1BIT bitmap1BitGreen;
  uint8_t rBrightness;
  uint8_t gBrightness;
} DISPLAY_DATA;

#define SPACE_WIDTH 5
#define SPACE2_WIDTH 3

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and Functions
// *****************************************************************************
// *****************************************************************************

void DISPLAY_Bitmap_Put_8Bit(uint8_t *bDataPtr, uint32_t nRows, uint32_t nColumns);
void DISPLAY_Bitmap_Put_1Bit(DISPLAY_BITMAP_1BIT rBitmap, DISPLAY_BITMAP_1BIT gBitmap);
void DISPLAY_Brightness_Set(uint8_t rBrightness, uint8_t gBrightness);
void DISPLAY_Method_Set(DISPLAY_METHOD displayMethod);
void Bitmap1_Scroll(DISPLAY_BITMAP_1BIT bitmap, SCROLL_DIRECTION dir, unsigned int nBits, bool loop);
void DISPLAY_Write_String_1Bit(DISPLAY_BITMAP_1BIT bitmap, char *string);
DISPLAY_METHOD DISPLAY_Method_Get(void);
void DISPLAY_Initialize(void);
void DISPLAY_Task(void);
DISPLAY_BITMAP_1BIT *Convert_SBitmap(DISPLAY_SBITMAP_1BIT *bitmap);
DISPLAY_SBITMAP_1BIT *Write_String_1Bit(char *string);
DISPLAY_SBITMAP_1BIT *Write_2HString_1Bit(char *string, char *string2);

DISPLAY_SBITMAP_2BIT *Write_String_2Bit(char *string, char padding);
DISPLAY_SBITMAP_2BIT *Write_2HString_2Bit(char *string, char *string2, char padding);

void Update_Bitmap_Window(DISPLAY_BITMAP_1BIT screen,DISPLAY_SBITMAP_1BIT *image, int offset);
void Update_CBitmap_Window(DISPLAY_BITMAP_1BIT screenRed, DISPLAY_BITMAP_1BIT screenGreen, DISPLAY_SBITMAP_2BIT *image, int offset);

void Clear_SBitmap(DISPLAY_SBITMAP_1BIT *bitmap);
void Clear_SBitmap2(DISPLAY_SBITMAP_2BIT *bitmap);



#ifdef  __cplusplus
}
#endif
#endif /* _DISPLAY_H */

/*******************************************************************************
  End of File
*/
