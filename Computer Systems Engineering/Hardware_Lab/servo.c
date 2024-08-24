#include "servo.h"

//Initiates the pins and sets PWM settings
void initServo(){

  *AT91C_PMC_PCER1 = 1<<4; //Enable register 1 for bit 4
  *AT91C_PIOB_PDR = 1<<17;
  *AT91C_PIOB_ABMR = 1<<17; //ABSR
  *AT91C_PWMC_ENA = 1<<1; // PWM Enable Register
  *AT91C_PWMC_CH1_CMR = 5; //SET Pre-scaler to Master_CLK/32
  *AT91C_PWMC_CH1_CPRDR = 52500;
  *AT91C_PWMC_CH1_CDTYR = 1050; //52500/20 = 2625 = 1ms, lowest = 0,4 ms = 2625*0,4 = 1050, highest = 2,3 ms = 2625*2,3 = 6037,5
  //range = 6037-1050 -> 4987/180 = CDTYR per degree = 28

}

//Set the servo in the given angle
void setServo(int angle){
   *AT91C_PWMC_CH1_CDTYR = 28*angle + 1350;
}









