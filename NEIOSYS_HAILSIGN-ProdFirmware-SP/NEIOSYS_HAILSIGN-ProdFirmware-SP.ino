/*
*/

#include "bsp.h"
#include "bcm.h"
#include "display.h"
#include "command.h"
#include "config.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "AsyncUDP.h"

#include <HTTPClient.h>
#include <string.h>

#include <BluetoothSerial.h>


#define PAGE_DELAY 100
  
uint32_t emptyBitmapRed[192] = {
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

uint32_t emptyBitmapGreen[192] = {
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
unsigned long timerDelay = 10;

BluetoothSerial SerialBT;
AsyncUDP udp;

#define T __


int r,c;

char outputstring[HTTP_INBUF_SIZE];
char pagestring[SCREEN_BUFFER_COUNT][SCREEN_BUFFER_SIZE];
char currentScreen=0;
char maxScreen=0;
volatile char workstring[HTTP_INBUF_SIZE];
volatile char newMessage;
char newDisplay = 0;
char inputstring[HTTP_INBUF_SIZE];
char readchar;
char currentPage;

int inputPtr = 0;
volatile int networkState = 0;
int execNow = 0;

void setup() 
{
  
  
   // testBitmap2 = Write_String_1Bit("... WAITING FOR NETWORK ... ");
  // todo : change bitmaps to pointers, created size of sign

  Serial.begin(115200);
  
  slog("Initializing");
  delay(500);
  
  setupNVS();
  loadConfig();
  signConfig.isMaster = isMaster();

  if(signConfig.isMaster) {
    slog("Master\n");
    openBT();
  } 
   
  
  
#ifdef _XDEBUG  
  slog("Writing WFN");
#endif

  for(r=0;r<SCREEN_BUFFER_COUNT;r++)
    pagestring[r][0] = '\0';

  if(signConfig.isMaster) {
    sprintf(pagestring[7], "... V%s WAITING FOR NETWORK ...",SIGN_VERSION);
  } else {
    sprintf(pagestring[7], "... V%s WAITING FOR SLAVE ...", SIGN_VERSION);
  }
  
/*
   if(!SerialBT.begin("ESP32")){
    Serial.println("An error occurred initializing Bluetooth");
   }
*/

 #ifdef T
 #ifdef _XDEBUG
   slog("Starting network task");
 #endif
 
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
  
  emptyBitmap1BitRed.nColumns = signConfig.signWidth;
  emptyBitmap1BitGreen.nColumns = signConfig.signWidth; // note Luan has a strange definition of rows and columns
//  DISPLAY_Bitmap_Put_8Bit(bitmap1, 16, 64);
  testBitmap3 = Write_String_2Bit(pagestring[7], 6);

  DISPLAY_Bitmap_Put_1Bit(emptyBitmap1BitRed, emptyBitmap1BitGreen);
  DISPLAY_Brightness_Set(rBrightness, gBrightness);
  DISPLAY_Method_Set(BITMAP_1BIT);
  DISPLAY_Brightness_Set(rBrightness, gBrightness);
  
  //Serial.println(ESP.getCpuFreqMHz());
}

void initTask(void * pvParameters) {
    slog("initTask starting\n");

   BSP_Initialize();
  DISPLAY_Initialize();  
  BCM_Initialize();    
  vled_on();
    slog("initTask done\n");
 
}




void networkTask(void * pvParameters) {
    //disableCore0WDT();
    HTTPClient http;

    if(signConfig.isMaster) {
      
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
                          slog("Connecting from %s\n", WiFi.macAddress());

                          networkState = 1;
                        } 
                    
                      lastTime = millis();
                    }
                    break;
          case 1:    
                    if(WiFi.status() == WL_CONNECTED) {
                        execNow = 1; // we need to connect once so remote end knows our IP address
                        networkState = 2; 
                        udp.listen(1234);
                        udp.onPacket([](AsyncUDPPacket packet) {
                          execNow = 1;
                        });
                        slog("Connected\n");  
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
                        
                          sprintf(workbuf, "%s&ver=%s&seq=%d", signConfig.fetchHost, SIGN_VERSION, signConfig.seq);

//                          http.begin(signConfig.fetchHost);
                          http.begin(workbuf);
                          http.setTimeout(31000);
                          slog("Fetching from %s\n", workbuf);
                        
                          int httpResponseCode = http.GET();

                          if(httpResponseCode == 204) {
                            // do not incrmeent seq
                          } else if (httpResponseCode == 200) {
                            String payload = http.getString();
                            // increment seq
                            signConfig.seq++;

                            if(payload.length() < HTTP_INBUF_SIZE) {
                              payload.toCharArray((char *)workstring,payload.length()+1);
                              workstring[payload.length()+1] = 0;
                              newMessage = 1;
                            }
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
    } // end if master
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
 
     if(newMessage) {
        newMessage = 0;
        slog("New message: [%s]\n", workstring);
 
        sprintf(outputstring, "%s", workstring);

           

        if(strstr(outputstring,"~!UPGRADE2")) {
          do_firmware_upgrade();
        }
                
        if(strstr(outputstring,"~!<")) {
          scrollVal = -1;  
        } else if(strstr(outputstring,"~!>")) {
          scrollVal = 1;
        }

        // split output string into pages
        // ~!P is page command
        char p = 0;
        int i;
        int j=0;
        maxScreen = 0;
              
        for(i=0;i<strlen(outputstring);i++) {
          if(outputstring[i] == '~' && outputstring[i+1] == '!' && outputstring[i+2] == 'P') {
            i+=2;
            pagestring[p][j] = '\0';
            j=0;
            if(p<SCREEN_BUFFER_COUNT-1)
              p++;  
          } else {
            if(j<SCREEN_BUFFER_SIZE)
              pagestring[p][j++] = outputstring[i];
          }
        } // end for loop to populate screen

       pagestring[p][j] = '\0';
       maxScreen = p;
       
       if(currentScreen > maxScreen)
        currentScreen = 0;
        
       newDisplay = 1;           
    } // end of scan for newline. 
           
    if(newDisplay) {
       slog("New display: [%s]", pagestring[currentScreen]);
 
        Clear_SBitmap2(testBitmap3);
       
        newDisplay = 0;
        if(strstr(pagestring[currentScreen],"\n")) {
          char workBuf[256];
          sprintf(workBuf, pagestring[currentScreen]);
          char *line2 = strstr(workBuf,"\n");
          char *line2_2 = line2;
          line2_2++;
          if(*line2_2 == '\0') {
            *line2 = '\0';
                
             testBitmap3 = Write_String_2Bit(pagestring[currentScreen],6);
           } else {
                *line2 = '\0';
                line2++;
                slog("Workbuf: %s line2: %s\n", workBuf, line2);
       
                testBitmap3 = Write_2HString_2Bit(workBuf,line2,6);    
                }
           } else {
            testBitmap3 = Write_String_2Bit(pagestring[currentScreen],6);
           }
     }
      
    softDelay = 0;
    
/*    if(testBitmap2->nColumns > 64) 
      Update_Bitmap_Window(emptyBitmap1Bit, testBitmap2, scrollInt++ % testBitmap2->nColumns);
    else   Update_Bitmap_Window(emptyBitmap1Bit, testBitmap2, 63);
  */

     if(testBitmap3->nColumns > signConfig.signWidth) {
      Update_CBitmap_Window(emptyBitmap1BitRed, emptyBitmap1BitGreen, testBitmap3, scrollInt % testBitmap3->nColumns);
        if(!(scrollInt % testBitmap3->nColumns))
            switchPage();
     } else {
        Update_CBitmap_Window(emptyBitmap1BitRed, emptyBitmap1BitGreen, testBitmap3, (signConfig.signWidth - 1));
        if(!(scrollInt % PAGE_DELAY))
          switchPage();
     }

    scrollInt += scrollVal;
    
          // DISPLAY_Bitmap_Put_1Bit(emptyBitmap1BitRed, emptyBitmap1BitGreen);
           DISPLAY_Brightness_Set(rBrightness, gBrightness);

  }

 
  
//---------------------------------------------
//For testing purpose only
//---------------------------------------------
}

 void switchPage()
  {
    // add one to current page, check to see if we are past max screen
    scrollInt = 0;

    if(maxScreen == 0)
      return;
    
    if(currentScreen == maxScreen) {
      newDisplay = 1;
      currentScreen = 0;
      return;
    }

    currentScreen++;
    newDisplay =1;
  }
  
void slog(char *fmt, ...)
{
  char slog_buffer[384];
  va_list ap;
  
  va_start (ap, fmt);
  vsprintf(slog_buffer, fmt, ap);
  va_end(ap);

 
   Serial.print(slog_buffer);
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
#ifdef _XDEBUG
 slog("openBT starting");
#endif
 SerialBT.begin(signConfig.bluetoothID);   
#ifdef _XDEBUG
 slog("openBT returning");
#endif
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

char isMaster() {
  Wire.begin();
  Wire.beginTransmission(0x6B);
  Wire.write(0x0A);
  Wire.endTransmission();
  Wire.requestFrom(0x6B,1);
  char c = Wire.read();
  if(c&128) {
    return 0;
  } else {
    return 1;
  }
}

/*******************************************************************************
 End of File
*/
