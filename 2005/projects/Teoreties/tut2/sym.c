#include <stdio.h>
#include "sym.h"

sym_node * first = NULL;

sym_node *insert_node(char * name)
{
	sym_node *new;
	if(first  == NULL)
	{
		first  = (sym_node *)malloc(sizeof(sym_node));
		first->name = name;
		first->next = NULL;
		first->value = 0;
		return first;
	}
	else
	{
		new = (sym_node *)malloc(sizeof(sym_node));
		new->name = name;
		new->next = first;
		new->value = 0;
		first = new;
		return new;
	}
}

sym_node * symlook(char *name)
{
	sym_node *cur;
		
	cur = first;
	while ((cur != NULL) && (strcmp(cur->name,name) != 0 ))
	{
		cur = cur->next;
	}
	if (cur == NULL) {
		return insert_node(name);
	} else {
		return cur;
	}
		
	
	printf("NO MATCH, not found in structure\n");
	return NULL;
}

void sym_clear(char * name)
{
	sym_node * temp;
	sym_node * current;
	sym_node * previous;
	
	if(first == NULL)
	{
		printf("Nothing to free \n");
	}
	else
	{	
		if (( temp = symlook(name)) != NULL)
		{	
			temp->value = 0;
		}
		else
		{
			printf("register name is not known so cant clear\n");
		}
	}
}

void sym_clear_all()
{
	sym_node *temp;
	while(first != NULL)
	{
		temp = first;
		first = first->next;
		free(temp->name);
		free(temp);
	}
	first = NULL;
}



