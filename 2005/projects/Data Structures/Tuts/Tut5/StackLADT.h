#ifndef STACKLADT_H
#define STACKLADT_H

#include "Item.h"

typedef struct stackNodeDef{
	itemType item;
	struct stackNodeDef *next;
} *stackNode; 


void stackInit(stackNode *s);
void deleteStack(stackNode *s);
int stackIsEmpty(stackNode s);
void stackPush(stackNode *s, itemType item);
itemType stackPop(stackNode *s);
itemType stackTop(stackNode s);

#endif
