/****************************************************************
 * 	compiler.c						*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/types.h"
#include "include/scanner.h"
#include "include/symboltable.h"
#include "include/parser.h"
#include "include/opcodes.h"
#include "include/codegen.h"


/*
 * main()
 *
 * Description : Called to start the process of compiling and executing a program.
 *
 * Input :
 * Output :
 *
*/

int main (int argc,char** argv) {

	int status, debug;
	FILE* fptr;
	debug = FALSE;
	
	if (( argc < 2 ) || (argc > 3)) {
        	printf( "Usage: cpl [-d ]<filename>\n" );
	        printf( "\t<filename> file to be compiled\n");
		printf("\t-d : optional parameter for debug output file\n");
    	}
	else {
		if (argc == 3) {
			if (strcmp(argv[1],"-d") != 0) {
				printf("Invalid parameter %s\n",argv[1]);
				exit(1);
			}
			else {
				printf("Switch on debug output\n");
				debug = TRUE;
			}
		}
		fptr = init(argv[argc-1]);
		init_table();
		initCodeGen(argv[argc-1],debug);
		printf("Compiling...\n");
		status = parse();
		if (status) {
			printf("\nCompiled %s with no errors\n",argv[argc-1]);
		}
		else {
			printf("\nThere were errors\n");
		}
		dumpToFile();
		fclose(fptr);
		clear_list();
	}
	return 1;
}
