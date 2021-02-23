#include "config.h"

#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

// todo: error handling

nvs_handle my_nvs_handle;

 
void setupNVS()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        slog("setupNVS() flash init running");
        
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    if(err) {
      char workbuf2[64];
      sprintf(workbuf2,"nvs_flash_init() returned error: %d", err);
      slog(workbuf2);
    } else {
#ifdef _XDEBUG
      slog("nvs_flash_init ran OK");
#endif
    }

#ifdef _XDEBUG
     slog("opening flash");
#endif
     err = nvs_open("storage", NVS_READWRITE, &my_nvs_handle);
     if(err) {
      char workbuf[64];
      sprintf(workbuf,"Failed to nvs_open: %d", err);
      slog(workbuf);

        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
        if(err) {
           sprintf(workbuf, "Failed to nvs_flash_init: %d", err);
        } else {
               err = nvs_open("storage", NVS_READWRITE, &my_nvs_handle);
               if(err) {
                  sprintf(workbuf, "*still* failed to nvs_open post reformat: %d", err);
                  slog(workbuf);
               }
        }
     } else {
#ifdef _XDEBUG      
       slog("Flash opened successfully");
#endif
     }
}

void formatNVS() 
{
   ESP_ERROR_CHECK(nvs_flash_erase());
   esp_err_t err = nvs_flash_init();
   reboot();
}
void loadConfig()
{
  // first, get the bluetooth ID key. If it doesn't exist, create one
  int signId = 0;

#ifdef _XDEBUG
  slog("loading configuration");
#endif
  
  signConfig.ssid = getConfigKey("ssid");
#ifdef _XDEBUG
  slog("Read first key");
#endif
  
  signConfig.password = getConfigKey("password");

  if(signConfig.password && !strcmp(signConfig.password, "~!EMPTY")) {
    free(signConfig.password);
    signConfig.password = NULL;
  }

  signConfig.bluetoothID = getConfigKey("bluetoothID");
  if(!signConfig.bluetoothID) {
    char workbuf[64];
    srand(esp_random());
    signId = rand();
    sprintf(workbuf,"SIGN-%d",signId);
    slog("No configuration found for bluetooth ID.. setting to");
    slog(workbuf);
    putConfigKey("bluetoothID",workbuf);
    sprintf(workbuf,"%d",signId);
    putConfigKey("signID",workbuf);
  }
  
  signConfig.bluetoothID = getConfigKey("bluetoothID");
  if(!signConfig.bluetoothID) {
    signConfig.bluetoothID = strdup("SIGN-INIT");
    slog("Still no configuration found for bluetoothID, setting to SIGN-INIT");
  } else {
#ifdef XDEBUG
    slog("Bluetooth key loaded OK -- key contents");
    slog(signConfig.bluetoothID);
#endif
  }
  
  signConfig.signID = getConfigKey("signID");
  signConfig.fetchHost = getConfigKey("fetchHost"); 
  signConfig.upgradeURL = getConfigKey("upgradeURL");
  signConfig.sigURL = getConfigKey("sigURL");
#ifdef XDEBUG
  slog("Configuration loaded");
#endif
}

char *getConfigKey(char *key)
{
    size_t required_size;
    if(ESP_OK == nvs_get_str(my_nvs_handle, key, NULL, &required_size)) {
      char* server_name = malloc(required_size);
      nvs_get_str(my_nvs_handle, key, server_name, &required_size);
      return(server_name);   
    }

    return NULL;
   
}

void putConfigKey(char *key, char *value)
{
   nvs_set_str(my_nvs_handle, key, value);
   nvs_commit(my_nvs_handle);
}
