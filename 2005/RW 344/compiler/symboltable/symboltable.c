#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"
#include "scanner.h"

static procedure_node *root;
static procedure_node *current;

void init_table()
{
		root = NULL;
		current = NULL;
		/*malloc for the procedure node (main procedure)*/
		root = malloc(sizeof(struct procedure_node));
		
		if (root!= NULL)
		{
			root->name = NULL;
			root->name = "_main";
			root->level = 0;
			root->next =  NULL;
			root->prev = NULL;
			root->array_node = NULL;
			root->var_list = NULL;
			
			// pretty sure this needs to be string copy. line below wont work. need to malloc for the string. i think??
			//strcpy(root->name,"main");
			if (root->name != NULL)
			{
				printf("Root Node created!!!\n");
				//print_tree(root);
				current = root;
			}
			else
			{
				printf("unable to create root node! exiting:\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("Unable to create root node of tree structure! no space or summin!!\n");
		}
}

/* must i search through the whole tree or just a level
 * i am going to write this to search the whole tree and
 * will modify and create a new function to search just on that level.
 */
struct procedure_node * lookup_proc(char * proc_name)
{
	int exist = FALSE;
	/*must do a linear search through list by name to find the procedure*/
	if(root != NULL)
	{
		current = root;
		
		if(strcmp(proc_name,current->name) == 0)
		{
			//printf("Found procedure [%s]\n",current->name);
			return current;
		}
		
		
		while(current != NULL)
		{
			if(strcmp(proc_name,current->name) == 0)
			{
				//printf("Found procedure [%s]\n",current->name);
				return current;
			}
			current = current->next;
			
		}
		
		printf("procedure [%s] does not exist.\n",proc_name);
		return NULL;	
	}
	else
	{
		printf("Error : Root is NULL\n");
		exit(EXIT_FAILURE);
		return NULL;
	}
}

/*search thruogh the linked list to find the var*/
struct var_node * lookup_var(char * proc_name, char * var_name)
{
	var_node * temp_var_node;
	temp_var_node = NULL;
	
	current = lookup_proc(proc_name);
	
	if(current != NULL)
	{
		if(current->var_list != NULL)
		{
			temp_var_node = current->var_list;
			
			while(temp_var_node != NULL)
			{
				if(strcmp(var_name,temp_var_node->name) == 0)
				{
					printf("Found variable [%s] in procedure [%s]\n",var_name,proc_name);
					return temp_var_node;
					
				}
				temp_var_node = temp_var_node->next;
				
			}
			
			printf("Variable [%s] is not part of proc [%s]'s variables.\n",var_name,proc_name);
			return NULL;	
		}
		else
		{
			printf("Procedure [%s] does not have any variables\n",proc_name);
			return NULL;
		}
	}
	else
	{
		printf("Error: unable to find proc node: cannot find variable\n");
		return 0;
	}
}

void insert_proc(char * name)
{
	procedure_node *new = NULL;
	if(lookup_proc(name) == NULL)
	{
		new = malloc(sizeof(struct procedure_node));
		if((new != NULL) && (root!= NULL))
		{
			//should use strcpy here
			new->name = NULL;
			new->name = name;
			// must find the postion to insert here
			current = findPlace();
			new->next = NULL;
			new->prev = current;
			current->next = new;
			new->array_node = NULL;
			new->var_list = NULL;
			printf("Procedure [%s] inserted\n",name);
		}
		else
		{
			printf("Unable to insert procedure: No space or root NULL\n");
		}
	}
	else
	{
		printf("Unable to insert, procedure [%s] already exists\n",name);
		//exit(EXIT_FAILURE);
	}
}

void printSymTable()
{
	//var_node * print_var = NULL;
	procedure_node *print = NULL;
	
	print = root;
	
	while(print != NULL)
	{
		printf("		[%s]\n",print->name);
		printf("			----->\n");
	
		if(print->var_list != NULL)
		{
			printProcVars(print->name);
		}
		
		if(print->next != NULL)
		{
			printf("		|   \n");
			printf("		|   \n");
			printf("   		V   \n");
		}
		print = print->next;
		
	}
	
}

void printProcVars(char * proc_name)
{
	procedure_node *print = NULL;
	var_node * print_var = NULL;
	print = lookup_proc(proc_name);
	
	if(print != NULL)
	{
		print_var = print->var_list;
		while(print_var != NULL)
		{
			printf("\t\t\t\tname[%s] type[%c] value[%d] ",print_var->name,print_var->type,print_var->value);
			if(print_var->next != NULL)
			{
				printf("next[hasNext]\n");
			}
			else
			{
				printf("[NULL]\n");
			}
			print_var = print_var->next;
		}
	}
	else
	{
		printf("procedure [%s] does not exist.\n",proc_name);
	}
	
}

/* This function will find the appropriate position for
 * the node to be placed. At the moment it just searches
 * the last position in the linked list.
 */
struct procedure_node * findPlace()
{
	procedure_node *temp = NULL;
	if(root == NULL)
	{
		printf("Root was found to be NULL\n");
		exit(EXIT_FAILURE);
	}
	temp = root;
	while(temp->next != NULL)
	{
		temp = temp->next;
	}
	return temp;
}

void insert_var(char * proc_name, char * var_name, char type,int value)
{
	
	var_node * new_var = NULL;
	var_node * temp = NULL;
	var_node * temp_var_node = NULL;
	
	temp_var_node = lookup_var(proc_name,var_name);
	if(temp_var_node == NULL)
	{
		current = lookup_proc(proc_name);

		if(current != NULL)
		{
			new_var = malloc(sizeof(struct var_node));
			if(new_var != NULL)
			{
				new_var->name = var_name;
				new_var->type = type;
				new_var->value = value;
				new_var->array_root = NULL;
				new_var->next = NULL;
			
				if (current->var_list == NULL)
				{
					current->var_list = new_var;
				}
				else
				{
					temp = current->var_list;
					temp_var_node = current->var_list;
					while(temp != NULL)
					{
						temp_var_node = temp;
						temp = temp->next;
					}
					temp_var_node->next = new_var;
				}
				printf("Variable [%s] has been inserted into procedure [%s]\n",var_name,proc_name);
			}
			else
			{
				printf("Cannot create node or variable\n");
			}
		}
		else
		{
			printf("Procedure [%s] does not exist\n",proc_name);
		}
	}
	else
	{
		printf("Variable [%s] already exists\n",var_name);
	}
}
	

void remove_proc()
{
/*

	//what is our new current node????? at the moment current pops up to parent after removal
	struct procedure_node *temp = current;
	
	//remove procedure from procedure tree
	if (current != NULL){
		//Does procedure have any children?
		if (current->child != NULL){
			printf("remove_proc: cannot delete procedure with children\n");
		}
		//if no, remove procedure
		else{  
			//node is eerste in lys
			if (current->prev == NULL){
				if (current->next != NULL){
					current->next->prev = NULL;
					current->parent->child = current->next;
				}
				else{
				//node is enigste op sy level
					current->parent->child = NULL;
				}	
				current = current->parent;
				free(temp);		
			}
			//node is in middel van lys
			else if (current->next != NULL){
				current->prev->next = current->next;
				current->next->prev = current->prev;
				current = current->parent;    ///check against parser!!!!
				free(temp);
			}
			//node is laaste in lys
			current->prev->next = NULL;
			current = current->parent;	   ///check against parser!!!!
			free(temp);
		}
	}
	else{
		printf("remove_proc: pointer to current procedure is invalid\n");
		exit(EXIT_FAILURE);
	}

*/
}
