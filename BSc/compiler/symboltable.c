/****************************************************************
 * 	symboltable.c						*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/types.h"
#include "include/scanner.h"
#include "include/error.h"
#include "include/opcodes.h"
#include "include/codegen.h"
#include "include/symboltable.h"

//global pointers needed to control positions and of course the root.
static procedure_node *root;
static procedure_node *current;

//debugging output
#ifdef DEBUG_SYM
	#define print(x, ...) printf(x, ## __VA_ARGS__)
#else
	#define print(x, ...)
#endif

/* 
 * init_table()
 *
 * Description : This function is called by the main function of the compiler which
 * then proceeds to create space for the root node. The root node is always the main
 * procedure of the file to be compiled.
 *
 * Input : NONE
 * Output : NONE
 *
*/

void init_table() {

		current = NULL;
		root = malloc(sizeof(struct procedure_node));
		
		if (root!= NULL) {
			sprintf(root->name,"_main");
			root->level = 0;
			root->next =  NULL;
			root->prev = NULL;
			root->array_node = NULL;
			root->var_list = NULL;
			root->call_list = NULL;
			root->params = NULL;
			root->disp = 0       ;

			if (root->name != NULL) {
				print("Root Node created!!!\n");
				current = root;
			}
			else {
				printf("unable to create root node! exiting:\n");
				exit(EXIT_FAILURE);
			}
		}
		else {
			printf("Unable to create root node of tree structure!!!\n");
		}
}

/*
 * checkParams()
 *
 * Description : Checks if the parameter types correspond. Gets the actual parameters
 * and compares them to the list of passed parameters. Will display the corresponding
 * error messages if an error occurs
 *
 * Input : The name of the function, the list of parameters associated with it.
 * Output : NONE
 *
*/

void checkParams(char *name, var_node* Theparams) {

	call_node *current = (call_node*)lookup_proc(name,TRUE+1);
	
	if (current != NULL) {
 		var_node *actual = current->params;
		var_node *params = Theparams;
		print("actual %x, params %x\n",actual,params);
		if ((actual == NULL) && (params != NULL)) {
			ERR_HANDLER(32);
		}
		if ((actual != NULL) && (params == NULL)) {
			ERR_HANDLER(33);
		}
		while (actual != NULL) {
			if (params == NULL) {
				ERR_HANDLER(33);
			}
			if (params->type != actual->type) {
				ERR_HANDLER(34);
			}
			params->isValue = actual->isValue;
			actual = actual->next;
			params = params->next;
		}
		if ((actual == NULL) && (params != NULL)) {
			ERR_HANDLER(32);
		}
		if ((actual != NULL) && (params == NULL)) {
			ERR_HANDLER(33);
		}
	}
	else {
		ERR_HANDLER(11);
	}
}

/*
 * pushParams()
 *
 * Description : Pushes the parameters onto the stack before branching into
 * a procedure.
 *
 * Input : A list of the parameters.
 * Output : NONE
 *
*/

void pushParams(var_node* Theparams) {
	var_node *params;
	if (Theparams != NULL) {
		params = Theparams;
		while (params->next != NULL) {
			params->next->prev = params;
			params = params->next;
		}
		while (params != NULL)
		{
			if (params->x->mode == var) {
				params->x->r = getReg();
				Put(F1_LDW,params->x->r,params->x->baseAddr,params->x->a);
				Put(F1_PSH,params->x->r,30,4);
				clearReg(params->x->r);
			}
			else {
				if (params->x->isArray) {
					print("Array\n");
					int r = getReg();
					Put(F1_LDW,r,params->x->r,0);
					Put(F1_PSH,r,30,4);
					clearReg(r);
				}
				else {
					print("isn't Array\n");
					if (params->isValue == FALSE) {
						ERR_HANDLER(35);
					}
					if (params->x->mode == cons) {
						params->x->r = getReg();
						Put(F2_ADDI,params->x->r,0,params->x->c);
						Put(F1_PSH,params->x->r,30,4);
						clearReg(params->x->r);
					}
					else {
						Put(F1_PSH,params->x->r,30,4);
					}
				}
			}
			
			params = params->prev;	
		}
	}
}

/*
 * popParams()
 *
 * Description : After branching back from a procedure the parameters are popped
 * off the stack
 * 
 * Input : a list of parameters to be popped off the stack
 * Output : NONE
 *
*/

void popParams(var_node* Theparams) {
	var_node* params;
	params = Theparams;
	if (params != NULL) {
		var_node *temp;
		cg_item *item;
		item = malloc(sizeof(struct cg_item));
		item->r = getReg();
		item->mode = reg;
		while (params != NULL) {
			item->type = params->x->type;
			Put(F1_POP,item->r,30,4);
			if (params->isValue == FALSE) {
				op2(F1_STW,params->x,item);
			}
			temp = params;
			params = params->next;
		}
		clearReg(item->r);
		free(item);
	}
}

/*
 * setPc()
 *
 * Description : Sets the program counter in the procedure node. Saving the
 * procedure starts in memory.
 *
 * Input : The name of the procedure, the parent node.
 * Output : NONE
 *
*/

void setPc(char * name,procedure_node * parent) {

	call_node * temp = parent->call_list;
	
	while (temp != NULL) {
		print("name = %s, proc_name %s\n",name,temp->proc_name);
		if (strcmp(name,temp->proc_name) == 0) {
			print("Setting pc\n");
			temp->pc = parent->next->pc;
			return;
		}
		temp = temp->next;
	}
}

/*
 * lookup_proc()
 *
 * Description : To lookup if a procedure exists, we start at the root and then move to the
 * last node of the procedure list without any checks for the proc we are looking for.
 * If the the last node in the list is the proc we are looking for then we return a pointer
 * to it. If it was not found then we check if the lookup was a procedure call. If it was,
 * then we search for the proc linearly returning 1 if it was found and not retrurning at all
 * if it was not (yet).
 * If it was not a procedure call then we search up the linked list (parents)
 * until we find the proc. If it is still not found by this time we return
 * null indicating it does not form part of our symboltable.
 *
 * Input : The procedure name we are looking for, and a boolean value indicating if it was a call or not.
 * Output : Returns the procedure node we are looking for. 
 *
*/

struct procedure_node * lookup_proc(char * proc_name,int call) {

	if(root != NULL) {
		current = root;
		while (current->next != NULL) {
			current = current->next;
		}
		if(strcmp(proc_name,current->name) == 0) {
			//printf("Vind hier\n");
			return current;
		}
		if(call > FALSE) {
			call_node* temp = current->call_list;
			while (temp != NULL) {
				if (strcmp(temp->proc_name,proc_name) == 0) {
					if (call == TRUE) {
						return (void*)temp;
					}
					else {
						return (void*)temp;
					}
				}
				temp = temp->next;
			}
		}
		
		
		while(current != NULL) {
			if(strcmp(proc_name,current->name) == 0) {
				return current;
			}
			current = current->prev;
			
		}
		
		print("procedure [%s] does not exist.\n",proc_name);
		return NULL;	
	}
	else {
		printf("Error : Root is NULL\n");
		exit(EXIT_FAILURE);
		return NULL;
	}
}

/*
 * lookup_var()
 *
 * Description : We search for the variable by first finding the procedure its in. Once this
 * has been found then search through the variable list linearly comparing the name of the
 * variable to that of which we are searching. If its not found in that procedure's list of
 * variable we then simply move up the procedure list and repeat the process, until we reach
 * the top of the list. If it is still not found then we return null, where as we return a 
 * pointer to the variable node if it was found.
 *
 * Input : The procedure name where the variable resides, the variable name we are looking for.
 * Output : returns the variable node we were looking for or null for an error.
 *
*/
 
struct var_node * lookup_var(char * proc_name, char * var_name) {

	var_node * temp_var_node;
	temp_var_node = NULL;
	
	current = lookup_proc(proc_name,FALSE);

	while(current != NULL) {
		if(current->var_list != NULL) {
			temp_var_node = current->var_list;
			
			while(temp_var_node != NULL) {
				if(strcmp(var_name,temp_var_node->name) == 0) {
					print("Found variable [%s] in procedure [%s]\n",var_name,current->name);
					return temp_var_node;
					
				}
				
				temp_var_node = temp_var_node->next;
				
			}
			
			print("Variable [%s] is not part of proc [%s]'s variables.\n",var_name,current->name);
		}
		if (current->params != NULL) {
			temp_var_node = current->params;
			
			while(temp_var_node != NULL) {
				if(strcmp(var_name,temp_var_node->name) == 0) {
					print("Found variable [%s] in procedure [%s]\n",var_name,current->name);
					return temp_var_node;
					
				}
				
				temp_var_node = temp_var_node->next;
				
			}
			
			print("Variable [%s] is not part of proc [%s]'s variables.\n",var_name,current->name);
			current = current->prev;
		}
		else {
			print("Procedure [%s] does not have any variables\n",current->name);
			current = current->prev;
		}
	}
	
	print("Error: unable to find variable\n");
	return NULL;
}

/* 
 * insert_proc()
 *
 * Description : First we must check that the procedure we are about to insert does not exist. If
 * it doesnt then we allocate space for a new node, set all its fields, allocate space for a new
 * call node and then find the correct position to insert the new proc node and then also the
 * correct position to insert the call node.
 *
 * Input : The name of the procedure we want to insert, The level where we currently reside.
 * Output : NONE
 *
*/
 
int insert_proc(char * name, int level) {

	procedure_node *new;
	new = NULL;
	
	if(lookup_proc(name,TRUE) == NULL) {
		new = malloc(sizeof(struct procedure_node));
		if((new != NULL) && (root!= NULL)) {
			sprintf(new->name,"%s",name);
			// must find the postion to insert here
			current = findPlace();
			new->next = NULL;
			new->prev = current;
			current->next = new;
			new->array_node = NULL;
			new->var_list = NULL;
			new->call_list = NULL;
			new->params = NULL;
			new->disp = 0;
			new->paramDisp = 4;
			new->pc = 0;
			new->level = level;
			print("\t\t\tLEVEL %d\n",level);
			call_node* new_call;
			new_call = malloc(sizeof (struct call_node));
			new->call = new_call;
			new->call->params = NULL;
			new_call->next = NULL;
			
			sprintf(new_call->proc_name,"%s",name);
			if (current->call_list == NULL) {
				current->call_list = new_call;
			}
			else {
				call_node* temp;
				temp = current->call_list;
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = new_call;
			}
			print("Procedure [%s] inserted\n",name);
			return TRUE;
		}
		else {
			printf("Unable to insert procedure: No space or root NULL\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		print("Unable to insert, procedure [%s] already exists\n",name);
		return FALSE;
	}
}

/*
 * insert_param()
 *
 * Description : Inserts a parameter in the procedure node.
 *
 * Input : The procedre name, the variable name, the type, the value,
 * the current level and if it is a value or not.
 * Output : Returns an integer idicating if the operation was succesful or not.
 *
*/

int insert_param(char * proc_name, char * var_name, enum types type,int value,int level,int isValue) {
	
	var_node * new_var;
	var_node * temp;
	var_node * temp_var_node;
	
	new_var = NULL;
	temp = NULL;
	temp_var_node = lookup_var(proc_name,var_name);
	
	if ((temp_var_node == NULL) || (temp_var_node->parent != (lookup_proc(proc_name,FALSE)))) {
		current = lookup_proc(proc_name,FALSE);

		if(current != NULL) {
			new_var = malloc(sizeof(struct var_node));
			if(new_var != NULL) {
				sprintf(new_var->name,"%s",var_name);
				new_var->type = type;
				new_var->value = value;
				new_var->next = NULL;
				new_var->prev = NULL;
				new_var->parent = current;
				current->paramDisp = current->paramDisp + 4;
				new_var->disp = current->paramDisp;
				new_var->level = level;
				new_var->isValue = isValue;
			
				if (current->params == NULL) {
					current->params = new_var;
					if (current->call != NULL) {
						current->call->params  = new_var;
					}
				}
				else {
					temp = current->params;
					temp_var_node = current->params;
					while(temp != NULL) {
						temp_var_node = temp;
						temp = temp->next;
					}
					temp_var_node->next = new_var;
					new_var->prev = temp_var_node;
					
				}
				print("Paramater [%s] has been inserted into procedure [%s]\n",var_name,proc_name);
				return TRUE;
			}
			else {
				printf("Cannot create node or variable\n");
				exit(EXIT_FAILURE);
			}
		}
		else {
			print("Procedure [%s] does not exist\n",proc_name);
			return FALSE;
		}
	}
	else {
		print("Variable [%s] already exists\n",var_name);
		return FALSE;		
	}
}

/* 
 * printSymTable()
 *
 * Description : This function is purely for dubugging. It moves through the symbol table
 * printing out each procedure and its variables.
 *
 * Input : NONE
 * Output : NONE
 *
*/
 
void printSymTable() {

	procedure_node *print;
	
	print = root;
	
	while(print != NULL) {
		printf("		[%s]\n",print->name);
		printf("			----->\n");
	
		if ((print->var_list != NULL) || (print->params != NULL)) {
			printProcVars(print->name);
		}
		
		if(print->next != NULL) {
			printf("		|   \n");
			printf("		|   \n");
			printf("   		V   \n");
		}
		print = print->next;
		
	}
	
}

/*
 * printProcs()
 *
 * Description : Debugging procedure to print out existing procedure nodes.
 *
 * Input : The procedure name.
 * Output : NONE
 *
*/

void printProcs(char * proc_name) {

	procedure_node *print;
	call_node * print_var;

	print = lookup_proc(proc_name,FALSE);
	print_var = NULL;
	
	if(print != NULL) {
		print_var = print->call_list;
		while(print_var != NULL) {
			printf("\t\t\t\tcall name[%s]",print_var->proc_name);
			if(print_var->next != NULL) {
				printf("next[hasNext]\n");
			}
			else {
				printf("[NULL]\n");
			}
			print_var = print_var->next;
		}
	}
	else {
		printf("procedure [%s] does not exist.\n",proc_name);
	}
}

/*
 * printProcVars()
 *
 * Description : Debugging procedure to print out existing variable nodes of a procedure
 *
 * Input : The procedure name.
 * Output : NONE
 *
*/

void printProcVars(char * proc_name) {

	procedure_node *print;
	var_node * print_var;

	print_var = NULL;
	
	print = lookup_proc(proc_name,FALSE);
	
	if(print != NULL) {
		print_var = print->var_list;
		
		while(print_var != NULL) {
			printf("\t\t\t\tname[%s] type[%d] value[%d] disp[%d] ",print_var->name,print_var->type,print_var->value,print_var->disp);
			if(print_var->next != NULL) {
				printf("next[hasNext]\n");
			}
			else {
				printf("[NULL]\n");
			}
			print_var = print_var->next;
		}
		print_var = print->params;
		
		while(print_var != NULL) {
			printf("\t\t\t\tparam_name[%s] type[%d] value[%d] disp[%d] ",print_var->name,print_var->type,print_var->value,print_var->disp);
			if(print_var->next != NULL) {
				printf("next[hasNext]\n");
			}
			else {
				printf("[NULL]\n");
			}
			print_var = print_var->next;
		}
	}
	else {
		printf("procedure [%s] does not exist.\n",proc_name);
	}
	printf("\n");
	printProcs(proc_name);
}


/*
 * findPlace()
 *
 * Description : This function will find the appropriate position for the node to be placed.
 * It simply traverses through the list searching for the bottom node and returns a pointer to it.
 *
 * Input : NONE
 * Output : Returns a pointer to the correct procedure node.
 *
*/

struct procedure_node * findPlace() {

	procedure_node *temp;
	
	temp = NULL;
	
	if(root == NULL) {
		printf("Root was found to be NULL\n");
		exit(EXIT_FAILURE);
	}
	temp = root;
	while(temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}

/* 
 * insert_var()
 *
 * Description : This inserting function checks if a variable can be added to a variable list.
 * The name is checked against existing variables to establish the validity of the variable's name.
 * The procedure returns TRUE if the variable has been successfully inserted, else FALSE is
 * returned if insertion has been aborted.
 *
 * Input : The procedure name, variable name, the type of the varaible, and its value.
 * Output : Returns Success or failure depending.
 *
*/
 
int insert_var(char * proc_name, char * var_name, enum types type,int value) {

	var_node * new_var;
	var_node * temp;
	var_node * temp_var_node;
	
	new_var = NULL;
	temp = NULL;
	temp_var_node = lookup_var(proc_name,var_name);

	if ((temp_var_node == NULL) || (temp_var_node->parent != (lookup_proc(proc_name,FALSE)))) {
		current = lookup_proc(proc_name,FALSE);

		if(current != NULL) {
			new_var = malloc(sizeof(struct var_node));
			if(new_var != NULL) {
				sprintf(new_var->name,"%s",var_name);
				new_var->type = type;
				new_var->value = value;
				new_var->next = NULL;
				new_var->parent = current;
				print("\t\t\tDECLARING level %d\n",current->level);
				new_var->level = current->level;
				if ((type == a_int) || (type == a_bool)) {
					current->disp = current->disp -(4*value);
					new_var->disp = current->disp;
				}
				else {
					if ((type == c_int) || (type == c_bool)) {
						new_var->disp = current->disp;
					}
					else {
						current->disp = current->disp -4;
						new_var->disp = current->disp;
					}
				}
			
				if (current->var_list == NULL) {
					current->var_list = new_var;
				}
				else {
					temp = current->var_list;
					temp_var_node = current->var_list;
					while(temp != NULL) {
						temp_var_node = temp;
						temp = temp->next;
					}
					temp_var_node->next = new_var;
				}
				print("Variable [%s] has been inserted into procedure [%s]\n",var_name,proc_name);
				return TRUE;
			}
			else {
				printf("Cannot create node or variable\n");
				exit(EXIT_FAILURE);
			}
		}
		else {
			print("Procedure [%s] does not exist\n",proc_name);
			return FALSE;
		}
	}
	else {
		print("Variable [%s] already exists\n",var_name);
		return FALSE;
	}
}

/*
 * remove_proc()
 *
 * Description : Removes the procedure from the symboltable.
 *
 * Input : The name of the procedure we want to remove.
 * Output : NONE
 *
*/
 
void remove_proc(char* name) {

	print("Removing %s\n",name);
	//current = lookup_proc(name,FALSE);
	
	struct procedure_node *temp;
	struct var_node *current_var;
	struct var_node *next_var;
	struct call_node *current_call;
	struct call_node *next_call;
	struct var_node *current_param;
	struct var_node *next_param;
	
	current = lookup_proc(name,FALSE);
	temp = current;
	current_var = NULL;
	next_var = NULL;
	current_call = NULL;
	next_call = NULL;
	current_param = NULL;
	next_param = NULL;

	//remove procedure from procedure tree
	if (current != NULL) {
		//Does procedure have any children?
		if (current->next != NULL) {
			printf("remove_proc: cannot delete procedure with children\n");
		}
		//if no, remove procedure
		else {
			if (current->prev != NULL) {
				setPc(name,current->prev);
			}
			print("varlist\n");
			//walk through varlist
 			if (current->var_list != NULL) {
				
				current_var = current->var_list;
				while (current_var != NULL) {
					next_var = current_var;
					current_var = current_var->next;
					print("removing var %s\n",next_var->name);
					free(next_var);	
				}
				current->var_list = NULL;	
			}
			print("na var\n");
			//walk through call list
 			if (current->call_list != NULL) {
				current_call = current->call_list;
				while (current_call != NULL) {
					print("voor params. current_call = %x\n", current_call);
					
					//walk through parameter list
					if (current_call->params != NULL) {
					print("in if\n");
						current_param = current_call->params;
						while (current_param != NULL) {
							next_param = current_param->next;
							free(current_param);
							current_param = next_param;
						}
						current_call->params = NULL;	
					}
					print("na params %x\n",current_call->next);
					next_call = current_call->next;
					free(current_call);
					current_call = next_call;
				}
				current->call_list = NULL;	
			}
			print("na call\n");
			if (current->prev != NULL) {
				print("parent %s\n",current->prev->name);
				sprintf(name,"%s",current->prev->name);
				current->prev->next = NULL;
			}
			else {
				root = NULL;
			}
			current = current->prev;	
			print("Removing %s\n",temp->name);
			free(temp);
		}
	}
	else {
		printf("remove_proc: pointer to current procedure is NULL\n");
		exit(EXIT_FAILURE);
	}

}

/*
 * removeAll()
 *
 * Description : This function simply does what it says, starting at the root, moves through
 * the tree removing the last procedure in the list first and recursing.
 *
 * Input : NONE
 * Output : NONE
 *
*/
 
void removeAll() {

	if (root != NULL) {
		procedure_node* temp;
		temp = root;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		char name[MAX_LEN+1];
		sprintf(name,"%s",temp->name);
		print("Remove All: removing %s\n",name);
		remove_proc(name);
		removeAll();
	}
}
