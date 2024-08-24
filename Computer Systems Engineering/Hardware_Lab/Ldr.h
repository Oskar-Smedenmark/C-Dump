#ifndef LDR_H
#define LDR_H

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include <stdlib.h>
#include <stdio.h>

void initLDR();
int readLight(int channel);

#endif