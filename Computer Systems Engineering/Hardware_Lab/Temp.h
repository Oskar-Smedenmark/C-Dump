#ifndef TEMP_H
#define TEMP_H
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdlib.h>

void initTemp();
int readTemp();
void startMeasurement();
void delay_temp();

#endif