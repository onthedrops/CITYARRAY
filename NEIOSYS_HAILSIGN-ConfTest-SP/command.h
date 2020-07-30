/*******************************************************************************
  File Name:
    command.h

  Summary:
  Utilities for processing bluetooth commands

  Description:

 *******************************************************************************/
#ifndef _COMMAND_H
#define _COMMAND_H

#ifdef  __cplusplus
extern "C" {
#endif

void processCommand(char *string);

extern void sendBT(char *string);
extern void sendlineBT(char *string);
extern void reboot();
extern volatile char workstring[256];



#ifdef  __cplusplus
}
#endif
#endif /* _COMMAND_H */
/*******************************************************************************
 End of File
*/
