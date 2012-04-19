#include <stdlib.h>
#include <stdio.h>
#include "include/types.h"
#include "include/scanner.h"
#include "include/symboltable.h"
#include "include/parser.h"
#include "include/error.h"

static int line_num, col_num;

/********************************************************
* Error handling function - used to explain errors  	*
* But also to keep all printed errors in one place	*
* More errors will be handled and added when found  	*
********************************************************/
void ERR_HANDLER(int error){

	switch(error) {
	
	case 0 : 
		printf("Error: line [%d], col [%d] : Unknown Symbol\n", line_num, col_num);
		break;
	case 1 : 
		printf("Error : No file open or file does not exist\n");
		break;
	case 2 : 
		printf("Error: line [%d], col [%d] : Maximum identifier length exceeded\n", line_num, col_num);
		break;
	case 3 : 
		printf("Error: line [%d], col [%d] : Comment not closed\n", line_num, col_num);
		break;
	case 4 :
	       	printf("Error: line [%d], col [%d] : Integer value out of range\n", line_num, col_num);
		break;
	case 5 : 
		printf("Parsing error before [%d,%d]\n\t%s expected but found %s\n", line_num, col_num, expect_err(),found());
		break;
	case 6 : 
		printf("Error before [%d,%d]\n\tConstant: Duplicate identifier\n", line_num, col_num);
		break;
	case 7 : 
		printf("Error before [%d,%d]\n\tArray: Duplicate identifier\n", line_num, col_num);
		break;
	case 8 : 
		printf("Error before [%d,%d]\n\tArray index must be positive\n", line_num, col_num);
		break;
	case 9 : 
		printf("Error before [%d,%d]\n\tVariable: Duplicate identifier\n", line_num, col_num);
		break;
	case 10 : 
		printf("Error before [%d,%d]\n\tProcedure already declared\n", line_num, col_num);
		break;
	case 11 : 
		printf("Error before [%d,%d]\n\tProcedure not declared\n", line_num, col_num);
		break;
	case 12 : 
		printf("Error before [%d,%d]\n\tVariable not declared\n", line_num, col_num);
		break;
	case 13 : 
		printf("Error before [%d,%d]\n\tArray not declared\n", line_num, col_num);
		break;
	case 14 : 
		printf("Error before [%d,%d]\n\tConstant not declared\n", line_num, col_num);
		break;
	case 15 : 
		printf("Error before [%d,%d]\n\tVariable expected not constant\n", line_num, col_num);
		break;
	case 16 : 
		printf("Error before [%d,%d]\n\tToo many variables in assignment\n", line_num, col_num);
		break;	
	case 17 : 
		printf("Error before [%d,%d]\n\tIncompatible types in assignment\n", line_num, col_num);
		break;
	case 18 : 
		printf("Error before [%d,%d]\n\tToo few variables in assignment\n", line_num, col_num);
		break;
	case 19 : 
		printf("Error before [%d,%d]\n\tMust index array with an integer\n", line_num, col_num);
		break;
	case 20 : 
		printf("Error before [%d,%d]\n\tUnable to perform arithmetic operations on a boolean\n", line_num, col_num);
		break;
	case 21 : 
		printf("Error before [%d,%d]\n\tIllegal to add/subtract a boolean\n", line_num, col_num);
		break;	
	case 22 : 
		printf("Error before [%d,%d]\n\tIllegal to multiply/divide a boolean\n", line_num, col_num);
		break;	
	case 23 : 
		printf("Error before [%d,%d]\n\tIllegal to 'not' an integer\n", line_num, col_num);
		break;	
	case 24 : 
		printf("Error before [%d,%d]\n\tIllegal to compare different types\n", line_num, col_num);
		break;	
	case 25 : 
		printf("Error before [%d,%d]\n\tIllegal to and/or different types\n", line_num, col_num);
		break;		
	case 26 : 
		printf("Error before [%d,%d]\n\tIllegal to and/or integers\n", line_num, col_num);
		break;	
	case 27 : 
		printf("Error before [%d,%d]\n\tBoolean expression expected\n", line_num, col_num);
		break;
	case 28 : 
		printf("Error before [%d,%d]\n\tArray index out of bounds\n", line_num, col_num);
		break;
	case 29 : 
		printf("Error before [%d,%d]\n\tDivision by zero\n", line_num, col_num);
		break;
	case 30 : 
		printf("Error before [%d,%d]\n\tParameter definition expected\n", line_num, col_num);
		break;
	case 31 : 
		printf("Error before [%d,%d]\n\tParameter expression expected\n", line_num, col_num);
		break;
	case 32 : 
		printf("Error before [%d,%d]\n\tToo many parameters passed to procedure\n", line_num, col_num);
		break;
	case 33 : 
		printf("Error before [%d,%d]\n\tToo few parameters passed to procedure\n", line_num, col_num);
		break;
	case 34 : 
		printf("Error before [%d,%d]\n\tIncompatible parameter type passed to procedure\n", line_num, col_num);
		break;
	case 35 : 
		printf("Error before [%d,%d]\n\tValue parameter passed to procedure instead of reference parameter\n", line_num, col_num);
		break;
	default: 
		printf("Error Default: line [%d], col [%d] Undefined\n",line_num, col_num);
		 break;
	}
	printf("There were errors compiling.\n");
	close();
	clear_list();
	removeAll();
	system("rm -f outa.txt");
	exit(EXIT_FAILURE);
}


void set_count(){
	line_num = 1;
	col_num = 1;
}

void inc_line(){
	line_num = line_num + 1;
	col_num = 1;
}

void inc_col(){
	col_num = col_num + 1;
}

char* set_error(enum symtype sym_type){
	switch(sym_type){
		case sym_array:
				return("array");
				break;
		case sym_begin:
				return("begin");
				break;
		case sym_bool:
				return("boolean");
				break;
		case sym_call: 
				return("call");
				break;
		case sym_const: 
				return("const");
				break;
		case sym_do: 
				return("do");
				break;
		case sym_end: 
				return("end");
				break;
		case sym_fi: 
				return("fi");
				break;
		case sym_if: 
				return("if");
				break;
		case sym_int: 
				return("int");
				break;
		case sym_od: 
				return("od");
				break;
		case sym_proc: 
				return("proc");
				break;
		case sym_read: 
				return("read");
				break;
		case sym_skip: 
				return("skip");
				break;
		case sym_write: 
				return("write");
				break;
		case sym_id: 
				return("id");
				break;
		case sym_number: 
				return("value");
				break;
		case sym_comma: 
				return(",");
				break;
		case sym_semicolon: 
				return(";");
				break;
		case sym_colon: 
				return(":");
				break;
		case sym_equal: 
				return("=");
				break;
		case sym_leftblock: 
				return("[");
				break;
		case sym_rightblock: 
				return("]");
				break;
		case sym_arrow: 
				return("->");
				break;
		case sym_and: 
				return("&");
				break;
		case sym_or: 
				return("|");
				break;
		case sym_less: 
				return("<");
				break;
		case sym_assign: 
				return(":=");
				break;
		case sym_greater: 
				return(">");
				break;
		case sym_plus: 
				return("+");
				break;
		case sym_minus: 
				return("-");
				break;
		case sym_guard: 
				return("[]");
				break;
		case sym_mult: 
				return("*");
				break;
		case sym_div: 
				return("/");
				break;
		case sym_mod: 
				return("\\");
				break;
		case sym_not: 
				return("~");
				break;
		case sym_leftbracket: 
				return("(");
				break;
		case sym_rightbracket: 
				return(")");
				break;
		case sym_stop: 
				return(".");
				break;
		case sym_eof: 
				return("End of file");
				break;
		default: 
				return("unknown character");
				break;
	}
}

