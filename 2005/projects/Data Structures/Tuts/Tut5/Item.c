#include <stdlib.h>
#include <stdio.h>
#include "Item.h"


itemType createItem(short type, int intValue, char charValue){
	itemType tmp;

	//Alocate space for a new Item
	tmp = malloc(sizeof(struct itemTypeDef));
	//Assign the the values
	tmp->intValue = intValue;
	tmp->charValue = charValue;
	tmp->type = type;
	return tmp;
}

itemType addItems(itemType x, itemType y){
	itemType tmp;

	//Alocate space for a new Item
	//Add the correct base type by using  
	//x->type field.
	if (x->type == y->type)
	{
		if (x->type == INT)
		{
			tmp = createItem(INT,x->intValue + y->intValue,'b');
		}else
		{
			tmp = createItem(CHAR, 12, x->intValue + y->intValue);
		}
	}else
	{
		tmp = NULL;
	}
	deleteItem(&x);
	deleteItem(&y);
	return tmp;
}

itemType mulItems(itemType x, itemType y){
	itemType tmp;

	//Alocate space for a new Item
	//Multiply the correct base type by using
	//x->type field.
	if (x->type == y->type)
	{
		if (x->type == INT)
		{
			tmp = createItem(INT,x->intValue * y->intValue,'b');
		}else
		{
			tmp = createItem(INT,x->intValue * y->intValue,'b');
		}
	}else
	{
		tmp = NULL;
	}
	deleteItem(&x);
	deleteItem(&y);
	return tmp;
}

char *itemToString(itemType x){
	char *string;	
	
	if (x->type == INT){
		string = malloc(32);
		snprintf(string, 32, "%d", x->intValue);
		return string;
	}
	if (x->type == CHAR){
		string = malloc(2);
		snprintf(string, 2, "%c", x->charValue);
		return string;
	}
}

void deleteItem(itemType *x){

	// free the memory of x and set x to NULL
	free(x);
	x = NULL;
}

int itemIntValue(itemType x){

	//return the integer field of x
	return x->intValue;
}

char itemCharValue(itemType x){

	//return the char field of x
	return x->charValue;
}
