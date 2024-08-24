#include "Ldr.h"

static int readyRead = 0;
static volatile int ldrReady = NULL;
static int curCh = 0;

//Initiates the ADC
void initLDR()
{
  *AT91C_PMC_PCER = 1<<11; //PA PCE
  *AT91C_PMC_PCER1 = 1<<5; //Activate ADC
  *AT91C_ADCC_MR = 1<<9; //ADC is set to 2 so 14 MHz
  *AT91C_ADCC_IER = 1<<24; //Enable interrupt for DRDY
  *AT91C_ADCC_SR; //Clear Interrupts
  
  NVIC_ClearPendingIRQ(ADC_IRQn);                       
  NVIC_SetPriority(ADC_IRQn, 0);//high priority
  NVIC_EnableIRQ(ADC_IRQn);
}

/*Starts a measurment on the given channel.
If data is ready it returns the value otherwise returns null*/
int readLight(int channel)
{
  if(readyRead == 0)
  { 
    readyRead = 1;
    *AT91C_ADCC_CHER = 1<<channel; //Enable current channel
    *AT91C_ADCC_CHDR = 1<<curCh;
    curCh = channel;
    *AT91C_ADCC_CR = 1<<1; //Start ADC
  }
  if(ldrReady != NULL)
  {
    int ldr = ldrReady;
    ldrReady = NULL;
    readyRead = 0;
    return ldr;
  }
  return NULL;
}

void ADC_Handler()
{
  int status = *AT91C_ADCC_SR;
  int dataMask = status & 1<<curCh;
  if(dataMask == 1<<curCh)
  {
    ldrReady = *AT91C_ADCC_LCDR;
  }
}