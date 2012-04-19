#ifndef ITEM_H
#define ITEM_H

#define CHAR	0
#define INT		1


typedef struct itemTypeDef{
	int intValue;
	char charValue;
	short type;
} *itemType;


itemType createItem(short type, int intValue, char charValue);
char *itemToString(itemType x);
void deleteItem(itemType *x);
int itemIntValue(itemType x);
char itemCharValue(itemType x);
itemType addItems(itemType x, itemType y);
itemType mulItems(itemType x, itemType y);


#endif
