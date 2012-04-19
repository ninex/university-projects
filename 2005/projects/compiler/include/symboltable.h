/****************************************************************
 * 	symboltable.h						*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdio.h>

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

void init_table();

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

void checkParams(char *name,var_node *Theparams);

/*
 * pushParams()
 *
 * Description : Pushes the parameters onto the stack before branching into
 * a procedure.
 *
 * Input :
 * Output : NONE
 *
*/

void pushParams(var_node* Theparams);

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

void popParams(var_node* Theparams);

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

void setPc(char * name,procedure_node * parent);


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
 
struct procedure_node * lookup_proc(char * proc_name, int call);

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

struct var_node * lookup_var(char * proc_name, char * var_name);

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

int insert_proc(char * name, int level);

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

int insert_param(char * proc_name, char * var_name, enum types type,int value,int level,int isValue);

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

struct procedure_node * findPlace();

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
 
int insert_var(char * proc_name, char * var_name, enum types type,int value);

/*
 * remove_proc()
 *
 * Description : Removes the procedure from the symboltable.
 *
 * Input : The name of the procedure we want to remove.
 * Output : NONE
 *
*/

void remove_proc(char * name);

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

void printSymTable();

/*
 * printProcs()
 *
 * Description : Debugging procedure to print out existing procedure nodes.
 *
 * Input : The procedure name.
 * Output : NONE
 *
*/

void printProcs(char * proc_name);

/*
 * printProcVars()
 *
 * Description : Debugging procedure to print out existing variable nodes of a procedure
 *
 * Input : The procedure name.
 * Output : NONE
 *
*/

void printProcVars(char * proc_name);

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

void removeAll();

#endif

