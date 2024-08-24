#ifndef SERVO_H
#define SERVO_H

#include "system_sam3x.h"
#include "at91sam3x8.h"

void initServo();
void setServo(int angle);
#endif