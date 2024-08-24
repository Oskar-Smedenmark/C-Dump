#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dlist.h"

void testInsertFirst(Dlist** first, Dlist** last);
void testIsMember(Dlist** first, Dlist** last);
void testPrintList(Dlist** first, Dlist** last);
void testRem(Dlist** first, Dlist** last);

int memoryUsed = 0;

int main()
{
    Dlist** first = (Dlist**)malloc(sizeof(Dlist));
    Dlist** last = (Dlist**)malloc(sizeof(Dlist));

    if(first == NULL || last == NULL)
    {
        printf("Error: Not enough memory\n");
        return 0;
    }
    memoryUsed += sizeof(Dlist)*2;

    *first = NULL;
    *last = NULL;

    testInsertFirst(first, last);
    *first = NULL;
    *last = NULL;

    testIsMember(first, last);
    *first = NULL;
    *last = NULL;

    testPrintList(first, last);
    *first = NULL;
    *last = NULL;

    testRem(first, last);

    free(first);
    free(last);
    memoryUsed -= sizeof(Dlist)*2;

    printf("Memory in usage at end of program: %d Bytes\n", memoryUsed);
    return 0;
}

void testInsertFirst(Dlist** first, Dlist** last)
{
    printf("-----------------------InsertFirst() tests begin-----------------------\n");
    Dlist* el = allocate(99);
    insertFirst(first, el);
    *last = el;
    for(int i = 98; i >= 0; i--)
    {
        el = allocate(i);
        insertFirst(first, el);
    }
    printList(*first);
    printf("\nis *first = el(1 = true, 0 = false): %d\n", *first == el);
    freeList(first);
    printf("-----------------------InsertFirst() tests are done-----------------------\n\n");
}

void testIsMember(Dlist** first, Dlist** last)
{
    printf("-----------------------IsMember() tests begin-----------------------\n");
    Dlist* el = allocate(99);
    insertFirst(first, el);
    *last = el;
    for(int i = 98; i >= 0; i--)
    {
        el = allocate(i);
        insertFirst(first, el);
    }

    printf("is *first = el(1 = true, 0 = false): %d\n", *first == el);
    printf("\nis el a member(0=yes, 1=no): %d\n", isMember(first, el));

    Dlist* notAMember = (Dlist*)malloc(sizeof(Dlist));
    memoryUsed += sizeof(Dlist);

    printf("is notAMember a member(0=yes, 1=no): %d\n", isMember(first, notAMember));

    rem(first, el);
    printf("\nis el a member after it's been removed(0=yes, 1=no): %d\n", isMember(first, el));

    free(el);
    free(notAMember);
    memoryUsed -= sizeof(Dlist)*2;
    freeList(first);
    printf("\n-----------------------isMember() tests are done-----------------------\n\n");
}

void testPrintList(Dlist** first, Dlist** last)
{
    printf("-----------------------PrintList() tests begin-----------------------\n");
    Dlist* el = allocate(99);
    insertFirst(first, el);
    *last = el;
    for(int i = 98; i >= 0; i--)
    {
        el = allocate(i);
        insertFirst(first, el);
    }

    printList(*first);
    backwardPrintList(*last);
    freeList(first);
    printf("-----------------------PrintList() tests are done-----------------------\n\n");
}

void testRem(Dlist** first, Dlist** last)
{
    printf("-----------------------Rem() tests begin-----------------------\n");
    Dlist* elements[100];
    Dlist* el = allocate(99);
    insertFirst(first, el);
    elements[99] = el;
    *last = el;
    for(int i = 98; i >= 0; i--)
    {
        el = allocate(i);
        insertFirst(first, el);
        elements[i] = el;
    }

    rem(first, elements[1]);
    free(elements[1]);
    memoryUsed -= sizeof(Dlist);

    rem(first, elements[0]);
    free(elements[0]);
    memoryUsed -= sizeof(Dlist);

    rem(first, elements[99]);
    free(elements[99]);
    memoryUsed -= sizeof(Dlist);

    rem(first, elements[50]);
    free(elements[50]);
    memoryUsed -= sizeof(Dlist);

    printf("After removing element 1, 0, 50 and 99\n");

    printList(*first);

    elements[0] = allocate(0);
    elements[1] = allocate(1);
    elements[50] = allocate(50);
    elements[99] = allocate(99);
    insertFirst(first, elements[0]);
    insertFirst(first, elements[1]);
    insertFirst(first, elements[50]);
    insertFirst(first, elements[99]);

    for(int i = 0; i < 100; i++)
    {
       rem(first, elements[i]);
       free(elements[i]);
       memoryUsed -= sizeof(Dlist);
    }
    printList(*first);

    freeList(first);
    printf("\n-----------------------Rem() tests are done-----------------------
           \n\n");
}
