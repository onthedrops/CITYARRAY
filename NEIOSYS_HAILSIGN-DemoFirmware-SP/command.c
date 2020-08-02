#include "command.h"
#include "config.h"
#include "global_variables.h"
#include "bsp.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_https_ota.h"
#include "ssl_cert.h"

extern int networkState;

void processCommand(char *string)
{        
        if(strlen(string)) {
          switch(string[0]) {
            default: sendBT("-ERR Unknown command: [");
                     sendBT(string);
                     sendlineBT("]"); return;
            case 'S': comamnd_set_key(string); return;  
         //   case 'D': command_delete_key(string); return;
            case 'G': comamnd_get_key(string); return;
            case 'M': command_set_message(string); return; 
            case 'R': command_reboot(string); return;
            case 'U': command_upgrade(string); return;
            case 'V': command_version(); return;  
            case '?': command_info(); return;
          }
       }
       
        sendBT("read string [");
        sendBT(string);
        sendlineBT("]");
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
    networkState = 3;    
    vled_off();
    closeBT();
    do_firmware_upgrade();
    openBT();
#endif    
  }
}

void command_version() {
  sendBT("+OK VERSION [");
  sendBT(SIGN_VERSION);
  sendlineBT("]");
}

void command_info() {
  char workbuf[64];
  sendlineBT("+OK INFO BLOCK TO EOF");
  sprintf(workbuf,"heap largest block: %d", heap_caps_get_free_size(MALLOC_CAP_8BIT) );
  sendlineBT(workbuf);
  sprintf(workbuf,"heap size: %d", esp_get_free_heap_size());
  sendlineBT(workbuf);
  sendlineBT("EOF");
}

esp_err_t do_firmware_upgrade()
{
    esp_http_client_config_t config = {
        .url = signConfig.upgradeURL,
        .cert_pem = ssl_cert,
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
