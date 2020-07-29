/*
*/

#include "bsp.h"
#include "bcm.h"
#include "display.h"

  
uint8_t bitmap1[1024];

uint32_t rBitmap1[32] = {
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF, 
0xFFFFFFFF,0x0000FFFF, 
0xFFFFFFFF,0x0000FFFF, 
0xFFFFFFFF,0x0000FFFF, 
0xFFFFFFFF,0x0000FFFF, 
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF,
0xFFFFFFFF,0x0000FFFF, 
0xFFFFFFFF,0x0000FFFF   
};
DISPLAY_BITMAP_1BIT rBitmap1Bit = {rBitmap1, 16, 64};

uint32_t gBitmap1[32] = {
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF,
0xFFFF0000,0xFFFFFFFF
};
DISPLAY_BITMAP_1BIT gBitmap1Bit = {gBitmap1, 16, 64};

uint32_t neiosysBitmap[32] = {
0x00000000, 0x00000000,
0x00000000, 0x000FFFF0,
0x00000000, 0x00088BD0,
0x00000000, 0x0008CA10,
0x00000000, 0x000BAFD0,
0xD13DF7D1, 0x00089A13,
0x1140441B, 0x00088A14,
0x11404415, 0x00088BD4,
0x8A384795, 0x00080013,
0x44044411, 0x00089C10,
0x44044411, 0x0008A210,
0x847847D1, 0x0008A217,
0x00000000, 0x0008A210,
0x00000000, 0x0008A210,
0x00000000, 0x00089C10,
0x00000000, 0x000FFFF0,
};

// each byte below is XXXX four rows
// 0,1 is 
/*uint32_t testBitmap[64] = {
0xF0000000, 0x00000000, 0x10000000, 0x00000000,
0x0F000000, 0x00000000, 0x01000000, 0x00000000,
0x00F00000, 0x00000000, 0x00100000, 0x00000000,
0x000F0000, 0x00000000, 0x00010000, 0x00000000,
0x0000F000, 0x00000000, 0x00001000, 0x00000000,
0x00000F00, 0x00000000, 0x00000100, 0x00000000,
0x000000F0, 0x00000000, 0x00000010, 0x00000000,
0x0000000F, 0x00000000, 0x00000001, 0x00000000,
0x00000000, 0xF0000000, 0x00000000, 0x10000000,
0x00000000, 0x0F000000, 0x00000000, 0x01000000,
0x00000000, 0x00F00000, 0x00000000, 0x00100000,
0x00000000, 0x000F0000, 0x00000000, 0x00010000,
0x00000000, 0x0000F000, 0x00000000, 0x00001000,
0x00000000, 0x00000F00, 0x00000000, 0x00000100,
0x00000000, 0x000000F0, 0x00000000, 0x00000010,
0x00000000, 0x0000000F, 0x00000000, 0x00000001

};
*/
uint32_t testBitmap[32] = {
0x80FC817C, 0x08400000, //0x10000000, 0x00000000,
0x80848140, 0x08400000, //0x01000000, 0x00000000,
0x80848140, 0x08400000, //0x00100000, 0x00000000,
0x80848140, 0x08400000, //0x00010000, 0x00000000,
0x80844240, 0x08400000, //0x00001000, 0x00000000,
0x80844240, 0x08400000, //0x00000100, 0x00000000,
0x80844240, 0x08400000, //0x00000010, 0x00000000,
0x8084427C, 0x08400000, //0x00000001, 0x00000000,
0x80844240, 0x08400000, //0x00000000, 0x10000000,
0x80842440, 0x08400000, //0x00000000, 0x01000000,
0x80842440, 0x08400000, //0x00000000, 0x00100000,
0x80842440, 0x08400000, //0x00000000, 0x00010000,
0x80842440, 0x08400000, //0x00000000, 0x00001000,
0x80841840, 0x08400000, //0x00000000, 0x00000100,
0x80841840, 0x08400000, //0x00000000, 0x00000010,
0xFEFC087C, 0x0FC00000 //0x00000000, 0x00000001
};

//DISPLAY_BITMAP_1BIT neiosysBitmap1Bit = {neiosysBitmap, 16, 64};

DISPLAY_BITMAP_1BIT neiosysBitmap1Bit = {testBitmap, 16, 64};

uint8_t gBrightness = 0x00;
uint8_t rBrightness = 0x00;


int r,c;
void setup() 
{
  BSP_Initialize();
  DISPLAY_Initialize();  
  BCM_Initialize();  
  vled_on();
  Serial.begin(115200);
  
//---------------------------------------------
//For testing purpose only
//---------------------------------------------
  for(c = 0; c < 16; c++)
  {    
    bitmap1[c + 4*64] = c;
    bitmap1[c + 8*64] = c << 4;
    bitmap1[c + 12*64] = (c << 4) | c;
  } 
  
  for(c = 0; c < 256; c++)
  {    
    bitmap1[c%16 + 32 + c/16*64] = c;
  }  

//  DISPLAY_Bitmap_Put_8Bit(bitmap1, 16, 64);
  //DISPLAY_Bitmap_Put_1Bit(rBitmap1Bit, gBitmap1Bit);
  DISPLAY_Write_String_1Bit(neiosysBitmap1Bit,"LOVE YOU");

  DISPLAY_Bitmap_Put_1Bit(neiosysBitmap1Bit, neiosysBitmap1Bit);
  DISPLAY_Brightness_Set(rBrightness, gBrightness);
//BCM_Tmr_Continue();
  DISPLAY_Method_Set(BITMAP_1BIT);
  //Serial.println(ESP.getCpuFreqMHz());
  int i;
  
  for(i=0;i<18;i++) 
     Bitmap1_Scroll(neiosysBitmap1Bit, LEFT, 1, true);
   
}


int redBrightnessOffset = -1;
int greenBrightnessOffset = 1;
bool grayScaleFlag = false;
int displayMethodDelay = 0;
DISPLAY_METHOD displayMethod = BITMAP_1BIT;
uint32_t softDelay = 0;
bool neioFlag = false;


// *****************************************************************************
// *****************************************************************************
// Section: MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP MAIN LOOP
// *****************************************************************************
// *****************************************************************************
void loop() 
{
  DISPLAY_Task();
  //put any other application task here

//---------------------------------------------
//For testing purpose only
//---------------------------------------------
  softDelay++; 
     
  if (softDelay == 100000) 
  {
    softDelay = 0;
    rBrightness += redBrightnessOffset;    
    gBrightness += greenBrightnessOffset;
    
    DISPLAY_Brightness_Set(rBrightness, gBrightness);
    /*if(gBrightness == 0x0F)
    {
      greenBrightnessOffset = 0;
      redBrightnessOffset = 1;
      if(rBrightness == 0x0F)
      {        
        greenBrightnessOffset = -1;
        redBrightnessOffset = 0;
      }
    }
    if(gBrightness == 0x00)
    {
      greenBrightnessOffset = 0;
      redBrightnessOffset = -1;
      if(rBrightness == 0x00)
      {        
        greenBrightnessOffset = 1;
        redBrightnessOffset = 0;
      }
    }
*/
   //
  /*  displayMethodDelay++;
    if(displayMethodDelay == 250)
    {
      if(DISPLAY_Method_Get() == BITMAP_8BIT)
      { 
        DISPLAY_Method_Set(BITMAP_1BIT);
        if(neioFlag)
        {        
          neioFlag = false;
          displayMethodDelay = 0;
          DISPLAY_Bitmap_Put_1Bit(neiosysBitmap1Bit, neiosysBitmap1Bit);          
        }
        else
        {          
          neioFlag = true;
          displayMethodDelay = 100;
          DISPLAY_Bitmap_Put_1Bit(rBitmap1Bit, gBitmap1Bit);  
        }
      }
      else
      {
        DISPLAY_Method_Set(BITMAP_8BIT);
        displayMethodDelay = 200;
      }  
    }*/
  }
//---------------------------------------------
//For testing purpose only
//---------------------------------------------
}
/*******************************************************************************
 End of File
*/
