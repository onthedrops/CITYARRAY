/*******************************************************************************
  File Name:
    bsp.c

  Summary:
  Board Support Package

  Description:

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "bsp.h"

// *****************************************************************************
// *****************************************************************************
// Section: Board Support Package Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void turn_on_row(unsigned int x)
 ******************************************************************************/
void BSP_Initialize(void)
{  
  pinMode(VLED_EN_PIN, OUTPUT);
  pinMode(ROW_EN_PIN, OUTPUT);
  pinMode(ROW_A_PIN, OUTPUT);
  pinMode(ROW_B_PIN, OUTPUT);
  pinMode(ROW_C_PIN, OUTPUT);
  pinMode(ROW_D_PIN, OUTPUT);
  pinMode(DATA_RED_PIN, OUTPUT);
  pinMode(DATA_GREEN_PIN, OUTPUT);
  pinMode(DATA_CLK_PIN, OUTPUT);
  pinMode(DATA_LAT_PIN, OUTPUT); 
  pinMode(DATA_RETURN_PIN, INPUT);
  shift_registers_clear(); 
  vled_off();
  
  signConfig.detectedSignWidth = getSignWidth();
    
  if(signConfig.detectedSignWidth<200)
    signConfig.signWidth = signConfig.detectedSignWidth + 1;
    else signConfig.signWidth = DEFAULT_SIGN_RESOLUTION_X;

   shift_registers_clear(); 

}

int getSignWidth() {
  int readSignWidth;
  int i, j;
  int ok = 1;
  int width = 0;

  for(i=0;i<15;i++) {
     ok = 1;
    for(j=0;j<10;j++) {
      delay(25);
      shift_registers_clear(); 

      if(width) {
        if(width != checkSignWidth()) {
          ok = 0;
          width = 0;
        }
      } else {
        width = checkSignWidth();
      }
    }
    if(ok)
      return(width);
  }

  return 256;
}


int checkSignWidth() {
  digitalWrite(DATA_RED_PIN, LOW);
  int i;
  
  for(i=0;i<512;i++) {
    digitalWrite(DATA_CLK_PIN, HIGH);
    ets_delay_us(1);
    digitalWrite(DATA_CLK_PIN, LOW);
  }

   digitalWrite(DATA_RED_PIN, HIGH);
  for(i=0;i<256;i++) {
    digitalWrite(DATA_CLK_PIN, HIGH);
    ets_delay_us(1);
    digitalWrite(DATA_CLK_PIN, LOW);
    ets_delay_us(1);
    if(digitalRead(DATA_RETURN_PIN) == HIGH)
      break;
  }

  return i;

}
/*******************************************************************************
  Function:
    void turn_on_row(unsigned int x)
 ******************************************************************************/
void turn_on_row(unsigned int x) {
  row_off();
  digitalWrite(ROW_A_PIN, (x & 0x01));
  digitalWrite(ROW_B_PIN, ((x >> 1) & 0x01));
  digitalWrite(ROW_C_PIN, ((x >> 2) & 0x01));
  digitalWrite(ROW_D_PIN, ((x >> 3) & 0x01));
  row_on();
}

/*******************************************************************************
  Function:
    void turn_on_row()
 ******************************************************************************/
void row_data_latch()
{  
  digitalWrite(DATA_LAT_PIN, HIGH);
  digitalWrite(DATA_LAT_PIN, LOW);
}

/*******************************************************************************
  Function:
    void column_shift_single(bool rBit, bool gBit)
 ******************************************************************************/
void column_shift_single(bool rBit, bool gBit)
{
  //shift out 1 bit data
  digitalWrite(DATA_RED_PIN, rBit);
  digitalWrite(DATA_GREEN_PIN, gBit);
  
  //clock bit into shift registers
  digitalWrite(DATA_CLK_PIN, HIGH);
  digitalWrite(DATA_CLK_PIN, LOW);
}

/*******************************************************************************
  Function:
    void column_shift(unsigned int rData, unsigned int gData, unsigned int nOfBits)
 ******************************************************************************/
void column_shift(unsigned int rData, unsigned int gData, unsigned int nOfBits)
{
  unsigned int i = 0;
  nOfBits = MIN(nOfBits, 32);
  while(i < nOfBits)
  {      
    column_shift_single(((rData >> i) & 0x01), ((gData >> i) & 0x01));
    i++;
  }  
}



void shift_registers_clear(void)
{  
  column_shift(0xFFFFFFFF, 0xFFFFFFFF, signConfig.signWidth);//reset shift registers
  column_shift(0xFFFFFFFF, 0xFFFFFFFF, signConfig.signWidth);//reset shift registers
}
/*******************************************************************************
 End of File
*/
