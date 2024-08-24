#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdarg.h>
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdlib.h>
#include <stdio.h> //remove when not using printf
typedef struct coord{
  int x;
  int y;
}coord;
coord getCursor();
void initDisplay();
void delay(int del);
unsigned char readStatusDisplay();
void writeCommand2Display(unsigned char command);
void writeData2Display(unsigned char data);
void clearDisplay();
void resetCursor();
void setCursor(unsigned int x, unsigned int y);

void printSymbol(unsigned int a);
void printChar(unsigned char a);
void printDouble(double a);
void printInteger(int a);
void clearRow();
void printString(int clear, char* str, ...);
#endif
