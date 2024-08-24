/*      Authors: Oskar Andersson and Albin Berglind
*       Date: 2020-12-09
*
*/
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Keypad.h"
#include "Display.h"
#include "Ldr.h"
#include "Temp.h"
#include "servo.h"
#include "math.h"
#include <stdio.h>

static volatile int disp = 0;
volatile unsigned int ticks = 0;

int main()
{
  int run = 1;
  SystemInit();
  
  initKeypad();
  initServo();
  
  initDisplay();
  clearDisplay();
  
  initLDR();
  initTemp();
  
  printString(1, "Display is ready!\n");
  setServo(0);
  
  int key = 0;
  int lastKey = 0;
  int temp = 0;
  int ldr[] ={0,0};
  int avgLdr = 0;
  int lastLdr = 0;
  int lastTemp = 0;
  int currentLdr = 0;
  
  SysTick_Config(84000);
  
  while(run == 1)
  {
    //Meassure and save the ldr values
    int light = readLight(currentLdr+1);
    if(light != NULL)
    {
      ldr[currentLdr] = light;
      currentLdr++;
      if(currentLdr > 1)
      {
        currentLdr = 0;
      }
      avgLdr = (ldr[0]+ldr[1])/2;
    }
    //Meassure and save the temp value
    int tmp = readTemp();
    if(tmp != NULL)
    {
      temp = tmp;
    }
    
    //print the readings on the display
    if(disp == 1)
    {
      if(avgLdr + 8 < lastLdr || avgLdr - 8 > lastLdr)
      {
        setCursor(0,0);
        printString(1, "LDR: %fV\n",(double)avgLdr/(4095/3.3));
        lastLdr = avgLdr;
      }
      if(temp != lastTemp)
      {
        setCursor(0,1);
        printString(1, "Temperature: %d C\n", temp);
        lastTemp = temp;
      }
      disp = 0;
    }
    
    //Set the servo at the angle of the button press
    key = readKeypad();
    if(key != 0 && lastKey != key)
    {
      switch(key)
      {        
      default:
        setServo(key*10);
        break;
      }
    }
    lastKey = key;
  }
  return 0;
}

void SysTick_Handler(void){
  ticks++;
  
  if(ticks % 1000 == 0)
  {
    disp = 1;
  }
  if(ticks == 4294967295) //If ticks goes to unsigned int maximum reset it
  {
    ticks = 0;
  }
}