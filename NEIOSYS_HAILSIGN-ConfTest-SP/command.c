#include "command.h"

void processCommand(char *string)
{        
        if(strlen(string)) {
          switch(string[0]) {
            default: sendBT("-ERR Unknown command: [");
                     sendBT(string);
                     sendlineBT("]"); return;
            case 'S': comamnd_set_key(string); return;  
            case 'G': comamnd_get_key(string); return;
            case 'M': command_set_message(string); return; 
            case 'R': command_reboot(string); return;  
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
  sendBT("R=[");
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
