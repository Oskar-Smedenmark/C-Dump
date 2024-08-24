#include "Keypad.h"

//Initializes the ports for the keypad
void initKeypad()
{
  *AT91C_PMC_PCER = 1<<13 | 1<<14;
  
  //Enable pins 34-37, 39-41
  *AT91C_PIOC_PER = 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<7 | 1<<8 | 1<<9;
  
  //Set pin 34-37 as inputs
  *AT91C_PIOC_ODR = 1<<2 | 1<<3| 1<<4 | 1<<5 | 1<<7 | 1<<8 | 1<<9;
  
  //Set pin 27
  *AT91C_PIOD_PER = 1<<2;
  
  *AT91C_PIOD_OER = 1<<2;
  *AT91C_PIOD_SODR = 1<<2;
}

//Reads the keypad and returns the corresponding button as an integer, 10 for *, 11 for 0 and 12 for #
int readKeypad()
{
  int button = 0;
  *AT91C_PIOD_CODR = 1<<2;
  *AT91C_PIOC_OER = 1<<7 | 1<<8 | 1<<9;
  *AT91C_PIOC_SODR = 1<<7 | 1<<8 | 1<<9;
  for(int i = 0; i < 3; i++)
  {
    *AT91C_PIOC_CODR = 1 << (7+i);
    for(int n = 0; n < 4; n++)
    {
      int row = 1<<(2+n);
      int pressed = *AT91C_PIOC_PDSR & row;
      if(pressed == 0)
      {
        button = n*3+i+1;
        *AT91C_PIOC_CODR = 1<<7 | 1<<8 | 1<<9;
        *AT91C_PIOC_ODR = 1<<7 | 1<<8 | 1<<9;
        *AT91C_PIOD_SODR = 1<<2;
        return button;
      }
    }
    *AT91C_PIOC_SODR = 1<<(7+i);
  }
  *AT91C_PIOC_CODR = 1<<7 | 1<<8 | 1<<9;
  *AT91C_PIOC_ODR = 1<<7 | 1<<8 | 1<<9;
  *AT91C_PIOD_SODR = 1<<2;
  
  return button;
}