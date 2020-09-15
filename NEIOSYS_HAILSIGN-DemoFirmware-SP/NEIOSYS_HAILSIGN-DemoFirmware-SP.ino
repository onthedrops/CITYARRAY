/*
*/

#include "bsp.h"
#include "bcm.h"
#include "display.h"
#include "command.h"
#include "config.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include "AsyncUDP.h"

#include <HTTPClient.h>
#include <string.h>

#include <BluetoothSerial.h>


  
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

DISPLAY_SBITMAP_2BIT *testBitmap3;

//DISPLAY_BITMAP_1BIT neiosysBitmap1Bit = {fonttestBitmap, 16, 128};
//DISPLAY_BITMAP_1BIT emptyBitmap1Bit = { emptyBitmap, 16, 64 };

DISPLAY_BITMAP_1BIT emptyBitmap1BitRed = { emptyBitmapRed, 16, 64 };
DISPLAY_BITMAP_1BIT emptyBitmap1BitGreen = { emptyBitmapGreen, 16, 64 };

uint8_t gBrightness = 0x0F;
uint8_t rBrightness = 0x0F;

unsigned long lastTime = 0;
unsigned long timerDelay = 120000;

BluetoothSerial SerialBT;
AsyncUDP udp;

#define T __


int r,c;
void setup() 
{
  
  
   // testBitmap2 = Write_String_1Bit("... WAITING FOR NETWORK ... ");


  Serial.begin(115200);
  Serial.println("Initializing");
  delay(500);
  
  setupNVS();
  loadConfig();
  openBT();
  
  
  Serial.println("Connecting");


          
  testBitmap3 = Write_String_2Bit("... WAITING FOR NETWORK ... ");
  
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


//  DISPLAY_Bitmap_Put_8Bit(bitmap1, 16, 64);
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
volatile int networkState = 0;
int execNow = 0;


void networkTask(void * pvParameters) {
    //disableCore0WDT();
    WiFi.setAutoReconnect(true);
    
  while(1) {
    //  feedLoopWDT();
      delay(10);
        
      switch(networkState) {
        
         case 0:    if(signConfig.ssid) {
 
                       if(WiFi.status() == WL_CONNECTED) {
                          networkState = 2;             
                        } else {
                          WiFi.begin(signConfig.ssid, signConfig.password);
                          Serial.println("Connecting");
                          networkState = 1;
                        } 
                    
                      lastTime = millis();
                    }
                    break;
         case 1:    
                    if(WiFi.status() == WL_CONNECTED) {
                        networkState = 2; 
                        udp.listen(1234);
                        udp.onPacket([](AsyncUDPPacket packet) {
                          execNow = 1;
                        });
                        Serial.println("Connected");  
                    }
                    
                     if ((millis() - lastTime) > 10000) {
                      WiFi.disconnect();
                      networkState = 0;
                    }
                    
                    break;     
         case 2:  
                      
                      if (execNow == 1 || ((millis() - lastTime) > timerDelay)) {
                        if(WiFi.status() != WL_CONNECTED) {
                          networkState = 0;
                        } else {
                          if(signConfig.fetchHost) {
                          char workbuf[256];
                        
                          HTTPClient http;

                          sprintf(workbuf, "%s?signId=%s",signConfig.fetchHost, signConfig.signID);
                          http.begin(workbuf);

                          Serial.print("Fetching from ");
                          Serial.println(workbuf);
                        
                          int httpResponseCode = http.GET();
      
                          if (httpResponseCode>0) {
                            String payload = http.getString();

                            payload.toCharArray((char *)workstring,payload.length()+1);
                            workstring[payload.length()+1] = 0;
                          } 
                      
                         http.end();
                       }
                      
                     }
                     lastTime = millis();
                     execNow = 0;
                  }
                  break;
            case 3:
 #ifdef __FIRMWARE_ON_CORE_0
                    disableCore0WDT();
                    SerialBT.end();
                    do_firmware_upgrade();
                    SerialBT.begin(signConfig.bluetoothID);   
                    networkState = 2;
#endif
                  break;
      } // end switch
  } // end while 1
} // end task

int redBrightnessOffset = 0;
int greenBrightnessOffset = 1;
bool grayScaleFlag = false;
int displayMethodDelay = 0;
DISPLAY_METHOD displayMethod = BITMAP_1BIT;
uint32_t softDelay = 0;
bool neioFlag = false;
int scrollInt = 0;
int scrollVal = 1;



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
  if (softDelay >= 50000 && BCM_OK()) 
  {

    if(SerialBT.available()) {
      readchar = SerialBT.read();
 
      if(readchar == '\n' || readchar == '\r') {
        processCommand(inputstring);
        
         inputPtr = 0;        
      } else {
        inputstring[inputPtr++] = readchar;
        inputstring[inputPtr] = '\0';
      }
    }
 
     if(strcmp((char *)workstring,outputstring)) {
        Serial.print("New message: [");
        Serial.print((char *)workstring);
        Serial.println("]");

           sprintf(outputstring, "%s", workstring);
           
           Clear_SBitmap2(testBitmap3);
           
           if(strstr(outputstring,"\n")) {

                if(strstr(outputstring,"~!<")) {
                  scrollVal = -1;  
                } else if(strstr(outputstring,"~!>")) {
                  scrollVal = 1;
                }
                
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
      Update_CBitmap_Window(emptyBitmap1BitRed, emptyBitmap1BitGreen, testBitmap3, scrollInt % testBitmap3->nColumns);
      else Update_CBitmap_Window(emptyBitmap1BitRed, emptyBitmap1BitGreen, testBitmap3, 63);

    scrollInt += scrollVal;
    
          // DISPLAY_Bitmap_Put_1Bit(emptyBitmap1BitRed, emptyBitmap1BitGreen);
           DISPLAY_Brightness_Set(rBrightness, gBrightness);

  }
//---------------------------------------------
//For testing purpose only
//---------------------------------------------
}

void slog(char *p)
{
   Serial.println(p);
}

void sendBT(char *p)
{
  SerialBT.print(p);
}

void sendlineBT(char *p)
{
  SerialBT.println(p);
}

void reboot()
{
    SerialBT.end();
    ESP.restart();
}

void closeBT()
{
  SerialBT.end();
}

void openBT()
{
 SerialBT.begin(signConfig.bluetoothID);   
}

char *get_firmware_sig()
{
    HTTPClient http;

    http.begin(signConfig.sigURL);
    char *sig;
  
    int httpResponseCode = http.GET();
      
    if (httpResponseCode>0) {
      String payload = http.getString();
      sig = (char *) malloc(payload.length()+1);
      payload.toCharArray(sig,payload.length()+1);
      sig[payload.length()+1] = 0;
    } else {
      return 0;                   
    }
  
    http.end();
    return sig;
}



/*******************************************************************************
 End of File
*/
