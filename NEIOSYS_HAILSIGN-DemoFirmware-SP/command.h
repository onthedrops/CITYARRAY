/*******************************************************************************
  File Name:
    command.h

  Summary:
  Utilities for processing bluetooth commands

  Description:

 *******************************************************************************/
#ifndef _COMMAND_H
#define _COMMAND_H

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_https_ota.h"


#ifdef  __cplusplus
extern "C" {
#endif

void processCommand(char *string);

extern void sendBT(char *string);
extern void sendlineBT(char *string);
extern void openBT();
extern void closeBT();
extern void reboot();
esp_err_t do_firmware_upgrade();

extern volatile char workstring[256];



#ifdef  __cplusplus
}
#endif
#endif /* _COMMAND_H */
/*******************************************************************************
 End of File
*/
