/*******************************************************************************
  File Name:
    global_variable.h

  Summary:


  Description:

 *******************************************************************************/


#ifndef _GLOBAL_VARIABLES_H
#define _GLOBAL_VARIABLES_H

#ifdef  __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <Arduino.h>
//#include <stdio.h>
//#include <hardwareSerial.h>
#include <stdbool.h>
#include "config.h"

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
#define bit_get(p,m)      ((p) & (m))
#define bit_set(p,m)      ((p) |= (m))
#define bit_clear(p,m)    ((p) &=~(m))
#define bit_flip(p,m)     ((p) ^= (m))
#define BIT(x)            (0x01 << (x)) 
#define WORD_BIT_FLIP(x)  ((x) ^ 0xFFFFFFFF)

#define MIN(a,b)  ((a < b) ? a : b)
#define MAX(a,b)  ((a > b) ? a : b)

// *****************************************************************************
// *****************************************************************************
// Section: SIGN CONFIGURATIONS
// *****************************************************************************
// *****************************************************************************
#define MODULE_RESOLUTION_X 64
#define MODULE_RESOLUTION_X_MINUS_1 63

#define MODULE_RESOLUTION_Y 16
#define MODULE_RESOLUTION_Y_MINUS_1 15

//sign resolutions should be variables......
//#define SIGN_RESOLUTION_X signConfig.signWidth
#define DEFAULT_SIGN_RESOLUTION_X 64
#define SIGN_RESOLUTION_Y 16

#define HTTP_INBUF_SIZE 1024
#define SCREEN_BUFFER_SIZE 256

#define BRIGHTNESS_PRECISION_BITS 4
#define BRIGHTNESS_PRECISION_BITS_MINUS_1 3

#define SIGN_VERSION "0.16.33"
#define SCREEN_BUFFER_COUNT 8

void slog(char *fmt, ...);

#ifdef  __cplusplus
}
#endif
#endif /* _GLOBAL_VARIABLES_H */

/*******************************************************************************
 End of File
*/
