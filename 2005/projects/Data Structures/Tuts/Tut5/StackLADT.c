#include <stdlib.h>
#include "StackLADT.h"

stackNode newNode(itemType item, stackNode next){
	stackNode x;

	//Create a new stack node and assign the item to the new stack node
	//return the new node
	x = malloc(sizeof(struct stackNodeDef));
	x->next = next;
	x->item = item;
	return x;
}

void stackInit(stackNode *s){

	//Set s to null
//	s = malloc(sizeof(struct stackNodeDef));
	s = NULL;

}

void deleteStack(stackNode *s){
	stackNode x;

	//If s != Null, free all the memory used for s
	while ( (*s)-> next != NULL)
	{
		x = (*s)->next;
		free((*s)->item);
		free(s);
		(*s) = x;
	}
	if (s != NULL)
	{
		free(s);
	}
}

int stackIsEmpty(stackNode s){

	//return > 0 if stack s is empty, else <= 0
	if (s != NULL)
	{
		return 1;
	}else
	{
		return 0;
	}
}

void stackPush(stackNode *s, itemType item){
	stackNode tmp;

	//Push an item on stack s
	tmp = newNode(item,*s);
	*s = tmp;
}

itemType stackPop(stackNode *s){
	stackNode tmp;
	itemType item = NULL;

	//Pop an item from stack s
	item = (*s)->item;
	tmp = (*s)->next;
	free(s);
	(*s) = tmp;
	return item;
}

itemType stackTop(stackNode s){
	itemType item = NULL;

	//return the top item of stack s
	item = s->item;
	return item;	
}

