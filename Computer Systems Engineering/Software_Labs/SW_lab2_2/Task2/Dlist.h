#ifndef DLIST_H
#define DLIST_H
typedef struct	DLinkedList	{
    double	sensorData;
    struct	DLinkedList	*prev;
    struct	DLinkedList	*next;
}Dlist;

//a)
void insertFirst(Dlist	**first, Dlist *el);
//b)
int	isMember (Dlist	**first, Dlist *el);
//c)
void printList(Dlist *first);
void backwardPrintList(Dlist *last);
//d)
void rem(Dlist **first, Dlist *el);

Dlist* allocate(double data);
void freeList(Dlist **first);
#endif
