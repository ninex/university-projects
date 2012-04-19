/******************************************************************************
 *												
 * FILE: testSymbolTable.c								
 *										
 * PURPOSE: Driver program to test symboltable			  	
 *								
 * 							
 * EXTERNAL VARIABLES:				
 * Source:				
 * Name			Type	I/O		Description	
 * ----			----	---		-----------
 * none							
 * 						
 * ABNORMAL TERMINATION, ERROR AND WARNING MESSAGES:	
 * 						
 *					
 * ASSUMPTIONS, CONSTRAINTS AND RESTRICTIONS:
 *					
 * NOTES:			
 *		 	
 * DEVELOPMENT HISTORY
 * 		
 * Date				Author				Description
 * ----				------				-----------		
 * 												
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "symboltable.h"

#define MAX_ID_LEN 32	/* maximum length of ID */

int main()
{
	printf("Testing the symboltable\n");
	char *name = NULL;
	char *name1 = NULL;
	char *name2 = NULL;
	char *name3 = NULL;
	
	char *var_name1 = NULL;
	char *var_name2 = NULL;
	char *var_name3 = NULL;
	char *var_name4 = NULL;
	char *var_name5 = NULL;
	char *var_name6 = NULL;
	char *var_name7 = NULL;

	init_table();

	
	
	//strcpy(name,"_main");
	// find a way to keep reading in char *'s and then change the if to a while----obviuosly
	//printf("please enter a procedure name to insert\n");
	//scanf("%s",&name);
	
	//printf("name is [%c]\n",name);
	name = "proc";
	name1 = "proc1";
	name2 = "proc2";
	name3 = "proc3";

	insert_proc(name);
	insert_proc(name1);
	insert_proc(name2);
	insert_proc(name3);
	
	insert_proc(name);
	
	var_name1 = "var1";
	var_name2 = "var2";
	var_name3 = "var3";
	var_name4 = "var4";
	var_name5 = "var5";
	var_name6 = "var6";
	var_name7 = "var7";
	
	insert_var(name,var_name1,'a',0);
	insert_var(name,var_name2,'i',10);
	insert_var(name,var_name3,'t',50);
	insert_var(name,var_name1,'a',0);
	//printProcVars(name);
	
	insert_var(name1,var_name1,'i',80);
	insert_var(name2,var_name1,'t',50);
	insert_var(name1,var_name7,'i',66);
	insert_var(name1,var_name2,'i',10);
	insert_var(name1,var_name3,'i',10);
	insert_var(name1,var_name4,'i',10);
	insert_var(name1,var_name5,'i',10);
	insert_var(name1,var_name6,'i',10);

	
	//printProcVars(name1);
	//printProcVars(name2);
	printSymTable();
}
