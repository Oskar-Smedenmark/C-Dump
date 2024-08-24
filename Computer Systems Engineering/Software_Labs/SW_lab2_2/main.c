#include <stdio.h>
#include <stdlib.h>
#include "Dlist.h"
#include "Sensor.h"

void sortList(Dlist** first, Dlist** last); //Not optimized enough but only utilizes insertFirst and not insertSorted.
void sortListNew(Dlist** first, Dlist** last);

double memoryUsed = 0;

int main()
{
    srand(time(NULL)); //Generate a seed based on computer time
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

    int elements = 9;

    for(int i = 0; i < elements; i++)
    {
        insertFirst(first, readSensor());
    }

    printf("Unsorted list: \n");
    printList(*first);

    sortListNew(first, last);

    printList(*first);
    backwardPrintList(*last);

    freeList(first);
    free(first);
    free(last);

    memoryUsed -= sizeof(Dlist)*2;
    printf("\nMemory in usage after free: %g Bytes\n", memoryUsed);
    return 0;
}

void sortList(Dlist** first, Dlist** last) //Not optimized enough but only utilizes insertFirst and not insertSorted.
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

void sortListNew(Dlist** first, Dlist** last)
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
    Dlist* temp = *first;
    Dlist* largest = (*first)->next;
    if(largest == NULL)
    {
        largest = temp;
    }
    while(temp != NULL)
    {
        if(temp->sensorData > largest->sensorData)
        {
            largest = temp;
        }
        Dlist* previous = temp;
        temp = temp->next;
        insertSorted(tempList, previous);
    }
    *last = largest;
    *first = *tempList;
    free(tempList);
    memoryUsed -= sizeof(Dlist);
}
