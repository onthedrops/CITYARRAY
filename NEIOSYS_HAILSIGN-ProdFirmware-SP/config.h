/*******************************************************************************
  File Name:
    config.h

  Summary:
  Utilities for configuration

  Description:

 *******************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif

void setupNVS();


char *getConfigKey(char *key);
void putConfigKey(char *key, char *value);
void loadConfig();

typedef struct
{
  char *ssid;
  char *password;
  char *bluetoothID;
  char *fetchHost;
  char *signID;
  char *upgradeURL;
  char *sigURL;
  int signWidth;
  
} CONFIG;

CONFIG signConfig;

#ifdef  __cplusplus
}
#endif
#endif /* _BCM_H */
/*******************************************************************************
 End of File
*/
