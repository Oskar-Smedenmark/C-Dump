#include "Display.h"



#define COLSIZE 32
#define ROWSIZE 16
static volatile coord cursor;

//Initiates the display pins and settings
void initDisplay()
{
  *AT91C_PMC_PCER = 1<<13 | 1<<14; //Peripheral Clock Enable, port C, D

  for(int i = 0; i < 10; i++)
  {
    *AT91C_PIOC_PER = *AT91C_PIOC_PER | 1<<(i+2);
  }

  for(int i = 0; i < 8; i++)
  {
    *AT91C_PIOC_PER = *AT91C_PIOC_PER | 1<<(i+12);
  }
  
  *AT91C_PIOC_OER = 1<<12 | 1<<13 | 1<<14 | 1<<15 | 1<<16 | 1<<17 | 1<<18 | 1<<19; //Set outputs
  *AT91C_PIOC_CODR = 1<<18 | 1<<19;

  *AT91C_PIOD_PER = 1;
  *AT91C_PIOD_OER = 1;

  //Clear and reset display
  *AT91C_PIOD_CODR = 1;
  delay(10000);
  *AT91C_PIOD_SODR = 1;

  writeData2Display(0x00);
  writeData2Display(0x00);
  writeCommand2Display(0x40); //Set text home address
  writeData2Display(0x00);
  writeData2Display(0x40);
  writeCommand2Display(0x42); //Set graphic home address
  writeData2Display(COLSIZE); //32 columns
  writeData2Display(0x00);
  writeCommand2Display(0x41); // Set text area
  writeData2Display(COLSIZE); //32 columns
  writeData2Display(0x00);
  writeCommand2Display(0x43); // Set graphic area
  writeCommand2Display(0x80); // text mode
  writeCommand2Display(0x94); // Text on graphic off
  
  writeData2Display(0x00);
  writeData2Display(0x00);
  writeCommand2Display(0x24); //Set ADP to 0,0
  
  //writeCommand2Display(0x93); //Cursor on, Blink on
  cursor.x = 0;
  cursor.y = 0;
}

//Do nothing for set amount of loops
void delay(int del)
{
  for(int i=0; i < del; i++)
  {
    asm("nop");
  }
}

//Reads the status on the display
unsigned char readStatusDisplay()
{
  unsigned char temp;

  *AT91C_PIOC_ODR = 1<<2 | 1<<3 | 1<<4 | 1<<5
                  | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Disable output on pin34-41

  *AT91C_PIOC_SODR = 1<<13; //Set dir as input (dir on 74HC245)
  *AT91C_PIOC_CODR = 1<<12; //Enable output (G on 74HC245)
  *AT91C_PIOC_SODR = 1<<14; //Set C/D
  *AT91C_PIOC_CODR = 1<<15; //Clear Chip Select Display (CE)
  *AT91C_PIOC_CODR = 1<<16; //Clear Read Display (RD)

  delay(10000);

  temp = *AT91C_PIOC_PDSR & (1<<2 | 1<<3 | 1<<4
                           | 1<<5 | 1<<6 | 1<<7
                           | 1<<8 | 1<<9); //Read from A1-A8 on 74HC245

  *AT91C_PIOC_SODR = 1<<15; //Set Chip Select Display (CE)
  *AT91C_PIOC_SODR = 1<<16; //Set Read Display (RD)
  *AT91C_PIOC_SODR = 1<<12; //Disable output (G on 74HC245)
  *AT91C_PIOC_CODR = 1<<13; //Set dir as output (dir on 74HC245)

  return temp;
}

//Writes a command to the display
void writeCommand2Display(unsigned char command)
{
  while((readStatusDisplay() & (1<<2 | 1<<3)) != (1<<2 | 1<<3)){/*Wait*/}
  *AT91C_PIOC_CODR = 1<<2 | 1<<3 | 1<<4 | 1<<5
                   | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Clear pin34-41
  *AT91C_PIOC_SODR = command << 2;
  *AT91C_PIOC_CODR = 1<<13; //Set dir as output (dir on 74HC245)
  *AT91C_PIOC_CODR = 1<<12; //Enable output (G on 74HC245)
  *AT91C_PIOC_OER = 1<<2 | 1<<3 | 1<<4 | 1<<5
                  | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Enable output on pin34-41
  *AT91C_PIOC_SODR = 1<<14; //Set C/D High
  *AT91C_PIOC_CODR = 1<<15; //Clear Chip Select Display (CE)
  *AT91C_PIOC_CODR = 1<<17; //Clear WR
  delay(10000);
  *AT91C_PIOC_SODR = 1<<15; //Set Chip Select Display (CE)
  *AT91C_PIOC_SODR = 1<<17; //Set WR
  *AT91C_PIOC_SODR = 1<<12; //Disable output (G on 74HC245)
  *AT91C_PIOC_ODR = 1<<2 | 1<<3 | 1<<4 | 1<<5
                  | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Disable output on pin34-41
}

//Writes data to the display
void writeData2Display(unsigned char data)
{
  while((readStatusDisplay() & (1<<2 | 1<<3)) != (1<<2 | 1<<3)){/*Wait*/}
  *AT91C_PIOC_CODR = 1<<2 | 1<<3 | 1<<4 | 1<<5
                   | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Clear pin34-41
  *AT91C_PIOC_SODR = data << 2;
  *AT91C_PIOC_CODR = 1<<13; //Set dir as output (dir on 74HC245)
  *AT91C_PIOC_CODR = 1<<12; //Enable output (G on 74HC245)
  *AT91C_PIOC_OER = 1<<2 | 1<<3 | 1<<4 | 1<<5
                  | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Enable output on pin34-41
  *AT91C_PIOC_CODR = 1<<14; //Clear C/D
  *AT91C_PIOC_CODR = 1<<15; //Clear Chip Select Display (CE)
  *AT91C_PIOC_CODR = 1<<17; //Clear WR
  delay(10000);
  *AT91C_PIOC_SODR = 1<<15; //Set Chip Select Display (CE)
  *AT91C_PIOC_SODR = 1<<17; //Set WR
  *AT91C_PIOC_SODR = 1<<12; //Disable output (G on 74HC245)
  *AT91C_PIOC_ODR = 1<<2 | 1<<3 | 1<<4 | 1<<5
                  | 1<<6 | 1<<7 | 1<<8 | 1<<9; //Disable output on pin34-41
}

//Clears the display from characters and move the cursor to the top left
void clearDisplay()
{
  resetCursor();
  int dispSize = COLSIZE*ROWSIZE;
  for(int i = 0; i < dispSize; i++)//40 Columns * 16 Rows
  {
    writeData2Display(0x0);
    writeData2Display(0x0);
    writeCommand2Display(0xC0); //Write and increment
  }
  resetCursor();
}

//Moves the cursor to the top left of the display
void resetCursor()
{
  setCursor(0,0);  
}

//Set the cursor to the x and y position
void setCursor(unsigned int x, unsigned int y)
{
  int lower = y*COLSIZE & 0xFF;
  int upper = y*COLSIZE & 0xFF00;
  writeData2Display(lower);
  
  writeData2Display(upper>>8);
  writeCommand2Display(0x21); //Set cursor at y
  writeCommand2Display(0x24); //Set ADP to y
  cursor.y = y;
  for(int i = 0; i < x; i++)
  {
    writeCommand2Display(0xC1); //read and increment ADP
  }
  cursor.x = x;
}

/*Prints the corresponding symbol on the datasheet
on the current cursor position and increments it*/
void printSymbol(unsigned int a)
{
  if(cursor.y >= ROWSIZE)
  {
    cursor.y = 0;
    resetCursor();
  }
  writeData2Display(a);
  writeCommand2Display(0xC0);
  cursor.x++;
  if(cursor.x > COLSIZE)
  {
    cursor.y++;
    cursor.x = 0;
  }
}

//Prints the character
void printChar(unsigned char a)
{
  printSymbol((int)a - 0x20);
}

//Rounds to 2 decimals then prints the value
void printDouble(double a)
{
  int size = 0;
  
  //To get the amount of numbers above the decimal point
  for (int i = 1; i <= a; i *= 10) 
  {
    size++;
  }
  size += 2; //Adding space for decimal numbers
  
  /*Rounding to the closest second decimal.
  And making it into an int 100 times larger*/
  int roundedA = (int)((a+0.005) * 100); 

  //Allocating memory for the number
  int* numbers = (int*)malloc(sizeof(int)*(size)); 
  //Adding the numbers backwards by singleing out the least significant number.
  for(int i = size-1; i >= 0; i--) 
  {
    numbers[i] = roundedA % 10;
    roundedA /= 10;
  }
  
  for(int i = 0; i < size; i++)
  {
    if(i == size-2)
    {
      printChar('.');
    }
     printSymbol(numbers[i] | 0x10);
  }
  free(numbers);
}

//Prints the given integer to the screen
void printInteger(int a)
{
  int size = 0;
  int neg = 0;
  if(a < 0)
  {
     a *= -1;
     neg = 1;
  }
  for (int i = 1; i <= a; i *= 10)
  {
    size++;
  }

  int* numbers = (int*)malloc(sizeof(int)*size);
  for(int i = size-1; i >= 0; i--)
  {
    numbers[i] = a % 10;
    a /= 10;
  }

  if(neg == 1)
  {
    printChar('-'); 
  }
  for(int i = 0; i < size; i++)
  {
     printSymbol(numbers[i] | 0x10);
  }
  free(numbers);
}

//Clears current row and moves the cursor to the left most positon on it
void clearRow()
{
  int y = cursor.y;
  //Prints an empty space on all positions on current row
  for(int i = 0; i < COLSIZE; i++)
  {
    printChar(' '); 
  }
  setCursor(0, y);
}

//Returns the cursor at its current position
coord getCursor()
{
  return cursor;
}

/*Prints the given char* to the display.
Handles break lines, integers and doubles.
clear is either 1 or 0,
1 to clear the current row before printing the new string, 
0 to print it on current cursor position.*/
void printString(int clear, char* str, ...)
{
  va_list inputs;
  
  va_start(inputs, str);
  
  if(clear == 1)
  {
    clearRow(); //Clear the current row
  }
  
  int next = 0;
  while(*(str+next) != '\0')
  {
    switch(*(str+next))
    {
    //Breakline so increase y and move the cursor
    case '\n': 
      cursor.y++;
      setCursor(0, cursor.y);
      break;
      
    case '%':
      if(*(str+next+1) == 'd') //If an integer should be printed
      {
        //Get the next argument from the variable argument list as an int
        printInteger(va_arg(inputs, int)); 
        next++;
      }
      else if(*(str+next+1) == 'f') //If a double should be printed
      {
        //Get the next argument from the variable argument list
        printDouble(va_arg(inputs, double)); 
        next++;
      }
      break;
      
    default:
      printSymbol((int)(*(str+next)) - 0x20); //Print the current symbol
      break;
    }
    next++;
  }
}


