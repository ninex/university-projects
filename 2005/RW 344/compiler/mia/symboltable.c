#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/symboltable.h"

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
			root->name = "root";
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

void insert_proc(char *name, int proc_level)
{
	procedure_node *new = malloc(sizeof(struct procedure_node));
	//we have to check that no procedure with this name exists already...
	//do we have to check for vars as well?
	
	//checkout array_node is still a bit messy
	
	//two options  - insert procedure as child or insert as sibling
	if ((current != NULL) && (new != NULL)){
		new->name = name;
		new->next = NULL;
		new->child = NULL;
		new->var_list = NULL;
		new->array_node = NULL;
		if (proc_level > level){         //we have to insert procedure as a child
			new->parent = current;
			new->prev = NULL;
			current->child = new;
			level++;
		}
		else if (proc_level == level){   //we have to insert the procedure as a sibling
			new->parent = current->parent;
			new->prev = current;
			current->next = new;
		}
		else{
			printf("insert_proc: Cannot insert procedure at a higher level thatn current procedure\n");
			exit(EXIT_FAILURE);
		}
		current = new;
	}
	else
	{
		printf("insert_proc: Unable to create new procedure node...\n");
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
 	insert_proc("first",1);
 	level++;
 	insert_proc("second",2);
// 	insert_proc(new,"third",2);
// 	insert_proc(new,"fourth",2);
// 	insert_proc(new,"fifth",3);
// 	insert_proc(new,"sixth",3);
// 	insert_proc(new,"seventh",3);
// 	insert_proc(new,"eigth",3);
// 	level++;
// 	insert_proc(new,"nineth",3);
// 	insert_proc(new,"tenth",3);
	print_tree();
}
