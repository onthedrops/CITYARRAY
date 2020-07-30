/*
*/

#include "bsp.h"
#include "bcm.h"
#include "display.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>

#include <BluetoothSerial.h>


  
uint8_t bitmap1[1024];

uint32_t emptyBitmapRed[64] = {
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000
};

uint32_t emptyBitmapGreen[64] = {
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000
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
DISPLAY_SBITMAP_2BIT *testBitmap3;

//DISPLAY_BITMAP_1BIT neiosysBitmap1Bit = {fonttestBitmap, 16, 128};
//DISPLAY_BITMAP_1BIT emptyBitmap1Bit = { emptyBitmap, 16, 64 };

DISPLAY_BITMAP_1BIT emptyBitmap1BitRed = { emptyBitmapRed, 16, 64 };
DISPLAY_BITMAP_1BIT emptyBitmap1BitGreen = { emptyBitmapGreen, 16, 64 };

uint8_t gBrightness = 0xFF;
uint8_t rBrightness = 0xFF;

static char s[64];
const char* ssid = "sheer";
const char* password = "";
String serverName = "http://10.101.1.3/NS/test.txt";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

BluetoothSerial SerialBT;
#define T __


int r,c;
void setup() 
{
  
  
   // testBitmap2 = Write_String_1Bit("... WAITING FOR NETWORK ... ");

          testBitmap3 = Write_String_2Bit("... WAITING FOR NETWORK ... ");


  Serial.begin(115200);
  SerialBT.begin("SIGN");   
  Serial.println("Connecting");

#ifdef T
     WiFi.begin(ssid, NULL);
  Serial.println("Connecting 2");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
#endif
  
/*
   if(!SerialBT.begin("ESP32")){
    Serial.println("An error occurred initializing Bluetooth");
   }
*/

 #ifdef T
   xTaskCreatePinnedToCore(
                    networkTask,   // Function to implement the task 
                    "netTask", // Name of the task 
                    10000,      // Stack size in words 
                    NULL,       // Task input parameter 
                    0,          // Priority of the task 
                    NULL,       // Task handle. 
                    0);  // Core where the task should run 
  //BCM_Initialize();  
 #endif

  initTask(NULL);


  DISPLAY_Bitmap_Put_8Bit(bitmap1, 16, 64);
  DISPLAY_Bitmap_Put_1Bit(emptyBitmap1BitRed, emptyBitmap1BitGreen);
  DISPLAY_Brightness_Set(rBrightness, gBrightness);
  DISPLAY_Method_Set(BITMAP_1BIT);
  DISPLAY_Brightness_Set(rBrightness, gBrightness);
  
  //Serial.println(ESP.getCpuFreqMHz());
}

void initTask(void * pvParameters) {
    Serial.print("initTask starting ");

   BSP_Initialize();
  DISPLAY_Initialize();  
  BCM_Initialize();    
  vled_on();
    Serial.print("initTask done ");
 
}

char outputstring[256];
volatile char workstring[256];
char inputstring[256];
char readchar;

int inputPtr = 0;

void networkTask(void * pvParameters) {
    //disableCore0WDT();
    
  while(1) {
    //  feedLoopWDT();
      delay(10);
      
  
  
  if ((millis() - lastTime) > timerDelay) {

      
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "?temperature=24.37";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
       // Serial.print("HTTP Response code: ");
       // Serial.println(httpResponseCode);
        String payload = http.getString();
       // Serial.println(payload);

        payload.toCharArray((char *)workstring,payload.length()+1);
        workstring[payload.length()+1] = 0;
        
       
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
        lastTime = millis();
      }
  }
}



int redBrightnessOffset = 0;
int greenBrightnessOffset = 1;
bool grayScaleFlag = false;
int displayMethodDelay = 0;
DISPLAY_METHOD displayMethod = BITMAP_1BIT;
uint32_t softDelay = 0;
bool neioFlag = false;
int scrollInt = 0;




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
  if (softDelay >= 50000) 
  {

    if(SerialBT.available()) {
      readchar = SerialBT.read();
      Serial.println("read");
    }
 
     if(strcmp((char *)workstring,outputstring)) {
        Serial.print("New message: [");
        Serial.print((char *)workstring);
        Serial.println("]");

           sprintf(outputstring, "%s", workstring);
           
           Clear_SBitmap2(testBitmap3);
           
           if(strstr(outputstring,"\n")) {
            
                char workBuf[256];
                sprintf(workBuf, outputstring);
                char *line2 = strstr(workBuf,"\n");
                char *line2_2 = line2;
                line2_2++;
                if(*line2_2 == '\0') {
                    *line2 = '\0';
                
                    testBitmap3 = Write_String_2Bit(outputstring);
                } else {
                *line2 = '\0';
                line2++;
                Serial.print("Workbuf:");
                Serial.print(workBuf);
                Serial.print("line2:");
                Serial.print(line2);
                testBitmap3 = Write_2HString_2Bit(workBuf,line2);    
                }
           } else {
            testBitmap3 = Write_String_2Bit(outputstring);
           }
     }
      
    softDelay = 0;
    
/*    if(testBitmap2->nColumns > 64) 
      Update_Bitmap_Window(emptyBitmap1Bit, testBitmap2, scrollInt++ % testBitmap2->nColumns);
    else   Update_Bitmap_Window(emptyBitmap1Bit, testBitmap2, 63);
  */

     if(testBitmap3->nColumns > 64)
      Update_CBitmap_Window(emptyBitmap1BitRed, emptyBitmap1BitGreen, testBitmap3, scrollInt++ % testBitmap3->nColumns);
      else Update_CBitmap_Window(emptyBitmap1BitRed, emptyBitmap1BitGreen, testBitmap3, 63);
    
    displayMethodDelay++;
    if(displayMethodDelay == 250)
    {
      DISPLAY_Method_Set(BITMAP_1BIT);
          displayMethodDelay = 0;
       //   DISPLAY_Bitmap_Put_1Bit(neiosysBitmap1Bit, neiosysBitmap1Bit);         
//         DISPLAY_Bitmap_Put_1Bit(*sheerBitmap1Bit, *sheerBitmap1Bit);
           DISPLAY_Bitmap_Put_1Bit(emptyBitmap1BitRed, emptyBitmap1BitGreen);

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
