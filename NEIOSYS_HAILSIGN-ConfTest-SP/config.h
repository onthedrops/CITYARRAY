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

#ifdef  __cplusplus
}
#endif
#endif /* _BCM_H */
/*******************************************************************************
 End of File
*/
