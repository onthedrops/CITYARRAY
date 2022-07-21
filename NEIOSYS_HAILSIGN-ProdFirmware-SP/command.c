#include "command.h"
#include "config.h"
#include "global_variables.h"
#include "bsp.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_https_ota.h"

extern int networkState;
extern char newMessage;
extern char currentScreen;
extern char outputstring;
extern char maxScreen;
extern char pagestring[SCREEN_BUFFER_COUNT][SCREEN_BUFFER_SIZE];
extern uint8_t gBrightness;
extern uint8_t rBrightness;

#define STR_C_VERSION "_-_-" SIGN_VERSION


void processCommand(char *string)
{        
        if(strlen(string)) {
          switch(string[0]) {
            default: sendBT("-ERR Unknown command: [");
                     sendBT(string);
                     sendlineBT("]"); return;
            case 'S': comamnd_set_key(string); return;  
            case 'p': command_programming_mode(string); return;
            case 'x': command_programming_exit(); return;
         //   case 'D': command_delete_key(string); return;
            case 'G': comamnd_get_key(string); return;
            case 'M': command_set_message(string); return; 
            case 'R': command_reboot(string); return;
            case 'F': command_format(string); return;
            case 'U': command_upgrade(string); return;
            case 'V': command_version(); return;  
            case '?': command_info(); return;
            case 'P': command_pages(string); return;
            case '+': command_brightness_up(); return;
            case '-': command_brightness_down(); return;
          }
       }
       
        sendBT("read string [");
        sendBT(string);
        sendlineBT("]");
}

void command_brightness_up() {
  if(gBrightness < 0x0F)
    gBrightness++;
    rBrightness = gBrightness;

  char buf[64];
  sprintf(buf, "+OK brightness %d", rBrightness);
  sendlineBT(buf);
 }

void command_brightness_down() {
  if(gBrightness > 0)
    gBrightness--;
    rBrightness = gBrightness;

  char buf[64];
  sprintf(buf, "+OK brightness %d", rBrightness);
  sendlineBT(buf);
  
      
}

void comamnd_set_key(char *string) {
  // todo: some kind of enable password
  
  // format should be S key,value
  // note that this does obliterate bits of the input buffer
    char *commaptr = strchr(string,',');
    
  if(strlen(string) < 2 || !commaptr) {
     sendBT("-ERR incorrect format: [");
     sendBT(string);
     sendlineBT("]"); return;
  }

    char *keyptr = string+2;
    char *dataptr = commaptr+1;
    *commaptr = '\0';

    putConfigKey(keyptr, dataptr);
    
    sendBT("+OK ");
    sendBT(keyptr);
    sendBT("=[");
    sendBT(dataptr);
    sendlineBT("]");
}

void comamnd_get_key(char *string) {
   if(strlen(string) < 2) {
     sendBT("-ERR incorrect format: [");
     sendBT(string);
     sendlineBT("]"); return;
  }

  char *workptr = string+2;
  char *rvalue = getConfigKey(workptr);

  if(!rvalue) {
     sendBT("-ERR no such key [");
     sendBT(workptr);
     sendlineBT("]");
     return;
  }
  
  sendBT("+OK ");
  sendBT(workptr);
  sendBT(" R=[");
  sendBT(rvalue);
  sendlineBT("]");
  free(rvalue);
}

void command_set_message(char *string) {
  char *msgptr = string+2;

  if(strlen(string) < 2) {
     sendBT("-ERR incorrect format: [");
     sendBT(string);
     sendlineBT("]"); return;
  }

  newMessage = 1;
  sprintf(workstring, "%s", msgptr);
}

void command_reboot(char *string) {
  if(strcmp(string,"R xyzzy")) {
     sendBT("-ERR incorrect password: [");
     sendBT(string);
     sendlineBT("]"); return;
  } else {
    reboot();
  }
}

void command_programming_exit() {
  if(networkState < 4) {
    sendlineBT("-ERR not in programming mode");
    return;
  }
    
  newMessage = 1;
  sprintf(workstring, "PrgModeEnd");
}

void command_programming_mode(char *string) {
  if(strcmp(string, "p xyzzy")) {
     sendBT("-ERR incorrect password: [");
     sendBT(string);
     sendlineBT("]"); return;
  } else {
    const uint8_t* point = esp_bt_dev_get_address();
    char str[20] = "";
    
    for(int i=0;i<6;i++) {
      sprintf(str, "%s%02X", str, (int)point[i]);      
    }
    
    newMessage = 1;
    networkState = 4;
    sprintf(workstring, "%s", str);
  }
}

void command_format(char *string) {
  if(strcmp(string,"F xyzzy")) {
    sendBT("-ERR incorrect password: [");
    sendBT(string);
    sendlineBT("]"); return;
  } else {
    formatNVS();
  }
}

void command_upgrade(char *string) {
   if(strcmp(string,"U xyzzy")) {
     sendBT("-ERR incorrect password: [");
     sendBT(string);
     sendlineBT("]"); return;
  } else {
    if(!signConfig.upgradeURL) {
      sendlineBT("-ERR no OTA URL available");
    }

#ifdef __FIRMWARE_ON_CORE_0
    sendlineBT("+OK commanding firmware update");
    networkState = 3;    
#else    
   
    do_firmware_upgrade();
    openBT();
#endif    
  }
}

void command_version() {
  sendBT("+OK VERSION [");
  sendBT(STR_C_VERSION);
  sendlineBT("]");
}

void command_info() {
  char workbuf[256];
  int i;
  
  sendlineBT("+OK INFO BLOCK TO EOF");
  sprintf(workbuf,"heap largest block: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT) );
  sendlineBT(workbuf);
  sprintf(workbuf,"heap size: %d", esp_get_free_heap_size());
  sendlineBT(workbuf);
  sprintf(workbuf, "Network state: %d", networkState);
  sendlineBT(workbuf);
  sprintf(workbuf, "Current screen %d max screen %d", currentScreen, maxScreen);
  sendlineBT(workbuf);
  sprintf(workbuf, "Sign width %d", signConfig.signWidth);
  sendlineBT(workbuf);
  /*sprintf(workbuf, "Master buffer: [%s]", outputstring);
  
  sendlineBT(workbuf);
  sendlineBT("Page buffers");
  for(i=0;i<SCREEN_BUFFER_COUNT;i++) {
    sprintf(workbuf,"%d: %s" , i, pagestring[i]);
    sendlineBT(workbuf);
    //sendlineBT(pagestring[i]);
  }
  */
  sendlineBT("EOF");
}

void command_pages() {
  char workbuf[256];
  int i;
    sprintf(workbuf, "Current screen %d max screen %d", currentScreen, maxScreen);
  sendlineBT(workbuf);
   sprintf(workbuf, "Master buffer: [%s]", workstring);
  sendlineBT(workbuf);
  sendlineBT("Page buffers");
  for(i=0;i<=maxScreen;i++) {
    sprintf(workbuf,"%d: [%s]" , i, pagestring[i]);
    sendlineBT(workbuf);
  }
  
  sendlineBT("EOF");

}
esp_err_t do_firmware_upgrade()
{
    if(!signConfig.upgradeURL) {
      return ESP_ERR_TIMEOUT;
    }

    networkState = 3;    
    vled_off();
    closeBT();
    
   slog("fetching sig");
   
   char *sig = get_firmware_sig();

   slog(sig);
   
   if(!sig) {
    slog("Failed to fetch sig");
    return ESP_ERR_TIMEOUT;
   }

    
    esp_http_client_config_t config = {
        .url = signConfig.upgradeURL,
        .cert_pem = sig,
    };


    slog("do_firmware_upgrade calling ota procedure");
   
    delay(100);

    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {
        slog("do_firmware_upgrade completed ok");
        esp_restart();
    } else {
        return ret;
    }
    return ESP_OK;
}
