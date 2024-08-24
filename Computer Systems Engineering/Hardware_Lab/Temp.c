#include "Temp.h"

extern volatile int ticks;
static volatile int tempReady = 0;
static volatile int delayStart = 0;

//Initiates the pins and setup
void initTemp()
{
  *AT91C_PMC_PCER = 1<<12 | 1<<27; //Peripheral Clock Enable, PIOB and TC0
  *AT91C_TC0_CMR = 0; //Select Timer_Clock1 as TCCLK
  *AT91C_TC0_CCR = 1 | 1<<2; //1 = Enable, 1<<2 = sw_reset
  *AT91C_TC0_CMR = 1<<17 | 1<<18; //1<<17 = Load counter to A when TIOA falling, 1<<18 = Load counter to B when TIOA rising
  
  *AT91C_PIOB_PER = 1<<25; //Activate pin 2
  *AT91C_PIOB_OER = 1<<25; //Set pin 2 as output  
  *AT91C_PIOB_CODR = 1<<25; //Init pulse
  
  NVIC_ClearPendingIRQ(TC0_IRQn);                       
  NVIC_SetPriority(TC0_IRQn, 0);//high priority
  NVIC_EnableIRQ(TC0_IRQn);
}

/*Starts a temperature reading if none is active.
Returns the temperature if data is ready otherwise null*/
int readTemp()
{
  int tmp = NULL;
  if(tempReady == 0 && delayStart + 520 < ticks)
  {
    startMeasurement();
    delayStart = ticks;
  }
  if(tempReady == 1)
  {
    tempReady = 0; 
    delayStart = ticks;
    double rB = *AT91C_TC0_RB;
    double rA = *AT91C_TC0_RA;    
    tmp = (int)(((rB-rA)/(42.0*5))-273.15);    //mck/2 = 42 : formula for temp in celcius
  }
  return tmp;
}

//Starts the measurement
void startMeasurement()
{  
    *AT91C_PIOB_OER = 1<<25; //Start pulse
    *AT91C_PIOB_CODR = 1<<25;
    delay_temp(25);
    *AT91C_PIOB_SODR = 1<<25;
    *AT91C_PIOB_ODR = 1<<25;
    *AT91C_TC0_CCR = 1<<2; //1<<2 = sw_reset
    *AT91C_TC0_SR; //Clear old interrupts
    *AT91C_TC0_IER = 1<<6;  //interupt enable register for LDRBS
}

//Do nothing for del amount of loops
void delay_temp(int del)
{
  for(int i=0; i < del; i++)
  {
    asm("nop");
  }
}

void TC0_Handler(){
  *AT91C_TC0_IDR = 1<<6; //Disable interrupt for LDRBS
  tempReady = 1;
}