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


uint32_t fonttestBitmap[128] = {
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000,
0x00000000, 0x00000000

};

uint16_t fxBitmap[32] = {
  0x0001, 0x0010, 0x0100, 0x1000,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000,
  0xF000, 0x000F, 0x00FF, 0xFFFF,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000,
  0xF000, 0x0F00, 0x00F0, 0x000F
};

DISPLAY_SBITMAP_1BIT testBitmap = { fxBitmap, 32 };
DISPLAY_SBITMAP_1BIT *testBitmap2;
DISPLAY_BITMAP_1BIT neiosysBitmap1Bit = {fonttestBitmap, 16, 128};
DISPLAY_BITMAP_1BIT *sheerBitmap1Bit;

uint8_t gBrightness = 0x00;
uint8_t rBrightness = 0x00;

static char s[64];

int r,c;
void setup() 
{
  
  BSP_Initialize();
  DISPLAY_Initialize();  
  BCM_Initialize();  
  vled_on();
  Serial.begin(115200);
    testBitmap2 = Write_String_1Bit("LoVe U ");
    
    sheerBitmap1Bit = Convert_SBitmap(testBitmap2);

  Serial.println("setup()");
  for(c=0;c<sheerBitmap1Bit->nColumns;c++) {
    
    sprintf(s,"%d %lx",c,sheerBitmap1Bit->dataPtr[c]);
    Serial.println(s);
  }

  sprintf(s, "----\nRows: %d Cols: %d",sheerBitmap1Bit->nRows,sheerBitmap1Bit->nColumns);
  Serial.println(s);
  
 // DISPLAY_Write_String_1Bit(neiosysBitmap1Bit,"SIGHSIGH");
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

  DISPLAY_Bitmap_Put_8Bit(bitmap1, 16, 64);
  //DISPLAY_Bitmap_Put_1Bit(rBitmap1Bit, gBitmap1Bit);
//  DISPLAY_Bitmap_Put_1Bit(neiosysBitmap1Bit, neiosysBitmap1Bit);
  DISPLAY_Bitmap_Put_1Bit(*sheerBitmap1Bit, *sheerBitmap1Bit);
  DISPLAY_Brightness_Set(rBrightness, gBrightness);
//BCM_Tmr_Continue();
  DISPLAY_Method_Set(BITMAP_1BIT);
  //Serial.println(ESP.getCpuFreqMHz());
}


int redBrightnessOffset = 0;
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
  if (softDelay == 50000) 
  {
    softDelay = 0;
    rBrightness += redBrightnessOffset;    
    gBrightness += greenBrightnessOffset;
    DISPLAY_Brightness_Set(rBrightness, gBrightness);
    if(gBrightness == 0x0F)
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

    Bitmap1_Scroll(neiosysBitmap1Bit, LEFT, 1, true);
    
    displayMethodDelay++;
    if(displayMethodDelay == 250)
    {
      if(DISPLAY_Method_Get() == BITMAP_8BIT)
      { 
        DISPLAY_Method_Set(BITMAP_1BIT);
        if(neioFlag)
        {        
          neioFlag = false;
          displayMethodDelay = 0;
       //   DISPLAY_Bitmap_Put_1Bit(neiosysBitmap1Bit, neiosysBitmap1Bit);         
         DISPLAY_Bitmap_Put_1Bit(*sheerBitmap1Bit, *sheerBitmap1Bit);
 
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
    }
  }
//---------------------------------------------
//For testing purpose only
//---------------------------------------------
}

void slog(char *p)
{
   Serial.println(p);
}

/*******************************************************************************
 End of File
*/
