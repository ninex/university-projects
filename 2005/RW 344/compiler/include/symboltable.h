#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdio.h>





/* This function is called by the main function of the compiler
 * which then proceeds to create space for the root node. The 
 * root node is always the main procedure of the file to be 
 * compiled.
 */
void init_table();

void checkParams(char *name,var_node *Theparams);

/* To lookup if a procedure exists, we start at the root and then move
 * to the last node of the procedure list without any checks for the proc
 * we are looking for. If the the last node in the list is the proc we are
 * looking for then we return a pointer to it. If it was not found then we 
 * check if the lookup was a procedure call. If it was, then we search for 
 * the proc linearly returning 1 if it was found and not retrurning at all
 * if it was not (yet).
 * 
 * If it was not a procedure call then we search up the linked list (parents)
 * until we find the proc. If it is still not found by this time we return
 * null indicating it does not form part of our symboltable.
 */
struct procedure_node * lookup_proc(char * proc_name, int call);

/* We search for the variable by first finding the procedure its in. Once 
 * this has been found then search through the variable list linearly
 * comparing the name of the variable to that of which we are searching.
 * If its not found in that procedure's list of variable we then simply move
 * up the procedure list and repeat the process, until we reach the top of 
 * the list. If it is still not found then we return null, where as we return 
 * a pointer to the variable node if it was found.
 */
struct var_node * lookup_var(char * proc_name, char * var_name);

/* First we must check that the procedure we are about to insert does not exist. If
 * it doesnt then we allocate space for a new node, set all its fields, allocate 
 * space for a new call node and then find the correct position to insert the new 
 * proc node and then also the correct position to insert the call node.
 *
 */
int insert_proc(char * name, int level);

int insert_param(char * proc_name, char * var_name, enum types type,int value,int level,int isValue);
/* This function will find the appropriate position for the node to 
 * be placed. It simply traverses through the list searching for the 
 * bottom node and returns a pointer to it.
 */
struct procedure_node * findPlace();

/* This inserting function checks if the variable exists or if the varaibles 
 * parent procedure 
 * **************************************************************************************
 * leaving this for mia to comment
 */
int insert_var(char * proc_name, char * var_name, enum types type,int value);

/* Mia -> please comment this
 *
 */
void remove_proc(char * name);

/* This function is purely for dubugging. It moves through the symbol 
 * table printing out each procedure and its variables.
 */
void printSymTable();

// debugging procedure to print out existing procedure nodes
void printProcs(char * proc_name);

// debugging procedure to print out existing variable nodes of a procedure
void printProcVars(char * proc_name);

/* This function simply does what it says, starting at the root, moves
 * through the tree removing the last procedure in the list first and 
 * recursing.
 */
void removeAll();

#endif

