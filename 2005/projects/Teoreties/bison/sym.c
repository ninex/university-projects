#include <stdio.h>
#include "sym.h"

node* list = NULL;

node* insert_node(char* name)
{
	if (list == NULL)
	{
		list = malloc(sizeof(struct node));
		list->next = NULL;
		list->val = 0;
		list->name = name;
		return list;
	}else
	{
		node * new = malloc(sizeof(struct node));
		new->next = list;
		new->val = 0;
		new->name = name;
		list = new;
		return new;
	}
}

node * sym_look(char* name)
{
	node * cur = list;
	while ((cur != NULL) && (strcmp(cur->name,name) != 0))
	{
		cur = cur->next;
	}
	if (cur == NULL)
	{
		return insert_node(name);
	}else
	{
		return cur;
	}
	
}

void sym_clear(char* name)
{
	node* cur = list;
	if (cur == NULL)
	{
		printf("nothing to free\n");
	}else
	{
		cur = sym_look(name);
		if (cur == NULL)
		{
			printf("nothere\n");
		}else
		{
			cur->val = 0;
		}
	}
			
}