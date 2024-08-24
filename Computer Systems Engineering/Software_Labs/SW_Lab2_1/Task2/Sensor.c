#include "Sensor.h"

Dlist* readSensor()
{
    return allocate((double)(rand()%1000 + 1)/1000);
}
