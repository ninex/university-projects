#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"

/*typedef struct var_node{
  char type; // this type will define if its a integers or booleans
  char * name;
  int value; //if its an array then store the size of the array here.
  //boolean is_constant;  //keep track of constant :   i.e boolean is_constant what about array_constant?
  struct array_node * array_root; // if array_root is null then the value is the value of the integer or bool.
  struct var_node * next; // all variables will be inserted into this linked list in order.
    
}var_node;


typedef struct array_node{
  int value;
  struct array_node * next;  
}node;


typedef struct procedure_node{
  char * name;
  struct procedure_node * parent;
  struct procedure_node * child;
  struct procedure_node * next;
  struct procedure_node * prev;
  struct array_node * array_node; 
  struct var_node * var_list;
}procedure_node;*/

static procedure_node *root = NULL;
static procedure_node *current = NULL;
static int level = 0;

void init_table()
{
		/*malloc for the procedure node (main procedure)*/
		root = malloc(sizeof(struct procedure_node));
		
		if (root!= NULL)
		{
			root->name = NULL;
			root->name = "main";
			root->parent = NULL;
			root->child = NULL;
			root->next = NULL;
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
		level++;
}

struct procedure_node * lookup_proc(char * proc_name)
{
	/*must do an inorder search by name to find the procedure*/
	return current;
}

struct var_node * lookup_var()
{
	/*search thruogh the linked list to find the var*/
}

void insert_proc(struct procedure_node * new, char * name, int proc_level)
{
	procedure_node *temp = root;
	
	new = malloc(sizeof(struct procedure_node));
	
	if(new != NULL)
	{
		new->name = name;
		
		if(root!= NULL)
		{
			if(proc_level == level)
			{
				while(root->child != NULL)
				{
					root = root->child;
				}
				while(root->next != NULL)
				{
					root = root->next;
				}
				
				root->next = new;
				new->prev = root;
				new->next = NULL;
				new->parent = NULL;
				new->child = NULL;
				new->var_list = NULL;
				new->array_node = NULL;
				
				
			}
			else if (proc_level > level)
			{
				while(root->child != NULL) 
				{
					root = root->child;
				}
				new->parent = root;
				new->var_list = NULL;
				/*must complete this*/
				root->child = new;
			}
			else
			{
				printf("Impossible: wrong level\n");
				exit(EXIT_FAILURE);
			}
		}
		root = temp;
	}
	else
	{
		printf("Unable to create root node of tree structure! no space or summin!!\n");
	}
}

void insert_var(char * proc_name, char * var_name, char type,int value,var_node * next)
{
	struct var_node * new_var = malloc(sizeof(struct var_node));
	
	if(new_var != NULL)
	{
		new_var->type = type;
		new_var->name = var_name;
		new_var->value = value;
		new_var->next = NULL;
	}
	else
	{
		printf("cant malloc - no mem or error\n");
	}
	
	current = lookup_proc(proc_name);
	
	while(current->var_list != NULL)
	{
		current->var_list = current->var_list->next;
	}
	
	current->var_list = new_var;
}

void remove_proc()
{

}


/*should be recursive*/
void print_tree()
{
	procedure_node *print_node = root;
	procedure_node *temp = root;
	
	if (print_node!= NULL)
	{
		printf("\t\t\tname[%s]",print_node->name);
		
		while(print_node->next != NULL)
		{
			//printf("here1\n");
			print_node = print_node->next;
			printf("  ->  [%s]",print_node->name);
		}
		printf("\n");
		print_node = temp;
		
		while(print_node->child != NULL)
		{
			printf("\t\t      |\n");
			printf("\t\t      v\n");
			print_node = print_node->child;
			printf("\t\t[%s]",print_node->name);
			temp = print_node;
			
			while(print_node->next != NULL)
			{
				print_node = print_node->next;
				printf("  ->  [%s]",print_node->name);
			}
			
			printf("\n");
			print_node = temp;
		}
	}
}

/* wont need this. just using for creating and testing*/
int main()
{
	struct procedure_node * new;
	init_table();
	insert_proc(new,"first",2);
	level++;
	insert_proc(new,"second",2);
	insert_proc(new,"third",2);
	insert_proc(new,"fourth",2);
	insert_proc(new,"fifth",3);
	insert_proc(new,"sixth",3);
	insert_proc(new,"seventh",3);
	insert_proc(new,"eigth",3);
	level++;
	insert_proc(new,"nineth",3);
	insert_proc(new,"tenth",3);
	print_tree();
}
