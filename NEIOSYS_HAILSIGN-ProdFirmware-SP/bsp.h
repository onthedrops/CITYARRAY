/*******************************************************************************
  File Name:
    bsp.h

  Summary:
    Board Support Package

  Description:

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
  Licesnse and Copyright Here
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef _BSP_H
#define _BSP_H

#ifdef  __cplusplus
extern "C" {
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "global_variables.h"


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// PIN Configuration
#define VLED_EN_PIN       23
#define DATA_RED_PIN      4
#define DATA_GREEN_PIN    5
#define DATA_CLK_PIN      18
#define DATA_LAT_PIN      19
#define ROW_A_PIN         32
#define ROW_B_PIN         33
#define ROW_C_PIN         25
#define ROW_D_PIN         2
#define ROW_EN_PIN        27
#define DATA_RETURN_PIN   35

//function macros

#define vled_on()       digitalWrite(VLED_EN_PIN, HIGH)
#define vled_off()      digitalWrite(VLED_EN_PIN, LOW)
#define row_on()        digitalWrite(ROW_EN_PIN, LOW)
#define row_off()       digitalWrite(ROW_EN_PIN, HIGH)


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void BSP_Initialize(void)

  Summary:
     This routine initializes I/O pins and peripherals of the Hailsign.

  Description:
    

  Precondition:

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    BSP_Initialize();
    </code>

  Remarks:
    This routine must be called from the system setup() function.
 */
void BSP_Initialize(void);

/*******************************************************************************
  Function:
    void turn_on_row(unsigned int x)

  Summary:
    Turn on Mosfet of row "x"

  Description:
    This routine turn on the MOSFET of row "x", connect VLED to common anode of
    the matrix LED at row "x"

  Precondition:

  Parameters:
    x: row number, range from 0-15 for this particular Hailsign version.

  Returns:
    None.

  Example:
    <code>
    turn_on_row(3);
    </code>

  Remarks:
    ...
 */
void turn_on_row(unsigned int x);

/*******************************************************************************
  Function:
    void row_data_latch()

  Summary:
    Latching whatever data that's been shifted to row.

  Description:

  Precondition:

  Parameters:
    None
    
  Returns:
    None.

  Example:
    <code>
    row_data_latch();
    </code>

  Remarks:
    ...
 */
void row_data_latch();

/*******************************************************************************
  Function:
    void column_shift_single(bool rBit, bool gBit)

  Summary:
    Shift out 1 bit of data (for both red and green), but does not latch it.
    REMEMBER: LED turns on when bit is low "0".
    If "1" means led ON in the bitmap used, bit reverse should be done
    before shifting out to registers.

  Description:

  Precondition:

  Parameters:
    rBit: red color bit
    gBit: green color bit
    
  Returns:
    None.

  Example:
    <code>
    column_shift_single(1, 0);
    or    
    column_shift_single(true, false);
    </code>

  Remarks:
    ...
 */
void column_shift_single(bool rBit, bool gBit);

/*******************************************************************************
  Function:
    void column_shift(unsigned int rData, unsigned int gData, unsigned int nOfBits)

  Summary:
    Shift out nOfBits data (LSB first) (for both red and green), but does not latch it.
    REMEMBER: LED turns on when bit is low "0".
    If "1" means led ON in the bitmap used, bit reverse should be done
    before shifting out to registers.

    Number of bit shift out is limited at 32 bits (int type)

  Description:

  Precondition:

  Parameters:
    rData: red color data word
    gData: green color data word
    nOfBits: number of bits will be shift out (max 32), LSB first
    
  Returns:
    None.

  Example:
    <code>
    column_shift(0x12345678, 0xABCDEF01, 32);
    or    
    column_shift_single(redDataWord, greenDataWord,16);
    </code>

  Remarks:
    ...
 */
void column_shift(unsigned int rData, unsigned int gData, unsigned int nOfBits);



void shift_registers_clear(void);

#ifdef  __cplusplus
}
#endif
#endif /* _BSP_H */

/*******************************************************************************
 End of File
*/
