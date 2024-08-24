#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dlist.h"
#include "Sensor.h"

void sortList(Dlist** first, Dlist** last);

int memoryUsed = 0;
int seed = 106342;

int main()
{
    srand(seed);
    Dlist** first = (Dlist**)malloc(sizeof(Dlist));
    Dlist** last = (Dlist**)malloc(sizeof(Dlist));
    memoryUsed += sizeof(Dlist)*2;
    if(first == NULL || last == NULL)
    {
        printf("Error: Not enough memory\n");
        return 0;
    }
    *first = NULL;
    *last = readSensor();
    insertFirst(first, *last);

    for(int i = 0; i < 9; i++)
    {
        insertFirst(first, readSensor());
    }

    printList(*first);

    sortList(first, last);

    printList(*first);
    backwardPrintList(*last);

    freeList(first);
    free(first);
    free(last);

    memoryUsed -= sizeof(Dlist)*2;
    printf("\nMemory in usage after free: %d Bytes\n", memoryUsed);
    return 0;
}

void sortList(Dlist** first, Dlist** last)
{
    if(*first == NULL)
    {
        printf("Error: List is empty\n");
        return;
    }

    Dlist** tempList = (Dlist**)malloc(sizeof(Dlist));
    if(tempList == NULL)
    {
        printf("Error: Not enough memory\n");
        return;
    }
    memoryUsed += sizeof(Dlist);

    *tempList = NULL;
    Dlist* temp = (*first)->next;
    Dlist* maxVal = *first;

    int size = 2;
    while(size > 1)
    {
        size = 0;
        while(temp != NULL)
        {
            size++;
            if(temp->sensorData > maxVal->sensorData)
            {
                maxVal = temp;
            }
            temp = temp->next;
        }
        rem(first, maxVal);
        if(*tempList == NULL)
        {
            *last = maxVal;
        }
        insertFirst(tempList, maxVal);
        maxVal = *first;
        if((*first)->next != NULL)
        {
            temp = (*first)->next;
        }
        else
        {
            insertFirst(tempList, maxVal);
            temp = NULL;
        }
    }
    *first = *tempList;
    free(tempList);
    memoryUsed -= sizeof(Dlist);
}

