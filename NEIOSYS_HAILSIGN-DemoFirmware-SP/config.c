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

     slog("opening flash");
     err = nvs_open("storage", NVS_READWRITE, &my_nvs_handle);
     if(err) {
      char workbuf[64];
      sprintf(workbuf,"Failed to nvs_open: %d", err);
      slog(workbuf);
     }
}

void loadConfig()
{
  // first, get the bluetooth ID key. If it doesn't exist, create one
  int signId = 0;

  slog("loading configuration");
  
  signConfig.ssid = getConfigKey("ssid");
  signConfig.password = getConfigKey("password");

  if(signConfig.password && !strcmp(signConfig.password, "~!EMPTY")) {
    free(signConfig.password);
    signConfig.password = NULL;
  }

  signConfig.bluetoothID = getConfigKey("bluetoothID");
  if(!signConfig.bluetoothID) {
    char workbuf[64];
    signId = rand();
    sprintf(workbuf,"SIGN-%d",signId);
    putConfigKey("bluetoothID",workbuf);
    sprintf(workbuf,"%d",signId);
    putConfigKey("signID",workbuf);
  }
  
  signConfig.bluetoothID = getConfigKey("bluetoothID");
  if(!signConfig.bluetoothID)
    signConfig.bluetoothID = strdup("SIGN-INIT");
    
  signConfig.signID = getConfigKey("signID");
  signConfig.fetchHost = getConfigKey("fetchHost"); 
  signConfig.upgradeURL = getConfigKey("upgradeURL");
  signConfig.sigURL = getConfigKey("sigURL");

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
