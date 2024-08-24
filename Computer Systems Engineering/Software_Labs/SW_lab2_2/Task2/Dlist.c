#include "Dlist.h"
#include <stdlib.h>
#include <stdio.h>

//Used to see memory leaks.
extern int memoryUsed;

//a
//Inserts the element first in the given list
void insertFirst(Dlist	**first, Dlist *el)
{
    el->next = *first;
    if(*first != NULL)
    {
        el->next->prev = el;
    }
    *first = el;
    el->prev = NULL;
}

//b
//Returns 0 if the element is in the given list or 1 if not
int	isMember (Dlist	**first, Dlist *el)
{
    int isEle = 1;
    Dlist* temp = *first;
    while(temp != NULL)
    {
        if(temp == el)
        {
            isEle = 0;
            break;
        }
        temp = temp->next;
    }
    return isEle;
}

//c
//Prints the list in forward order
void printList(Dlist *first)
{
    int nEle = 0;
    Dlist* temp = first;
    while(temp != NULL)
    {
        nEle++;
        printf("[%g]", temp->sensorData);
        if(nEle % 10 == 0)
        {
            printf("\n");
        }
        temp = temp->next;
    }
    printf("\nSize of list: %d\n", nEle);
}

//Prints the list in backwards order
void backwardPrintList(Dlist *last)
{
    int nEle = 0;
    Dlist* temp = last;
    while(temp != NULL)
    {
        nEle++;
        printf("[%g]", temp->sensorData);
        if(nEle % 10 == 0)
        {
            printf("\n");
        }
        temp = temp->prev;
    }
    printf("\nSize of list: %d\n", nEle);
}

//d
//This decouples the given element from the given list but does not free it from memory.
void rem(Dlist **first, Dlist *el)
{
    if(isMember(first, el) == 0)
    {
        Dlist* temp = *first;
        //If the element is first in the list
        if(temp == el)
        {
            //If the element isn't the last element left in the list
            if(temp->next != NULL)
            {
                temp->next->prev = NULL;
            }
            *first = temp->next;
            temp->next = NULL;
            temp->prev = NULL;
        }
        else
        {
            while(temp->next != NULL)
            {
                //If the element is somewhere between first and last in the list
                if(temp == el)
                {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                    break;
                }
                temp = temp->next;
            }
            //If the element is at the end of the list
            if(temp->next == NULL & temp == el)
            {
                temp->prev->next = NULL;
            }
            temp->next = NULL;
            temp->prev = NULL;
        }
    }
}

//Allocates an element with the given data and returns it as a pointer.
Dlist* allocate(double data)
{
    Dlist* el = (Dlist*) malloc(sizeof(Dlist));
    if(el == NULL)
    {
        printf("Error: Not enough memory\n");
        return NULL;
    }
    memoryUsed += sizeof(Dlist);
    el->next = NULL;
    el->prev = NULL;
    el->sensorData = data;

    return el;
}

//Goes through the list and frees the memory of each element
void freeList(Dlist **first)
{
    Dlist* temp = *first;
    Dlist* next = NULL;
    while(temp != NULL)
    {
        next = temp->next;
        free(temp);
        memoryUsed -= sizeof(Dlist);
        temp = next;
    }
    *first = NULL;
}
