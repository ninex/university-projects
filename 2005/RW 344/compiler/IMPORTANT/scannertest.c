#include <stddef.h>
#include <stdio.h>
#include "include/scanner.h"
#include "include/parser.h"


void printSym(token_str* token){
	
	switch(token->sym_type){
		case sym_eof: printf(" eof ");
			break;
		case sym_array: printf("array");
			break;
		case sym_begin: printf("begin");
			break;
		case sym_bool: printf("boolean");
			break;
		case sym_call: printf("call");
			break;
		case sym_const: printf("const");
			break;
		case sym_do: printf("do");
			break;
		case sym_end: printf("end");
			break;
		case sym_fi: printf("fi");
			break;
		case sym_if: printf("if");
			break;
		case sym_int: printf("int");
			break;
		case sym_od: printf("od");
			break;
		case sym_proc: printf("proc");
			break;
		case sym_read: printf("read");
			break;
		case sym_skip: printf("skip");
			break;
		case sym_write: printf("write");
			break;
		case sym_id: printf("%s",token->name);
			break;
		case sym_number: printf("%d",token->val);
			break;
		case sym_comma: printf(",");
			break;
		case sym_semicolon: printf(";");
			break;
		case sym_colon: printf(":");
			break;
		case sym_equal: printf("==");
			break;
		case sym_leftblock: printf("[");
			break;
		case sym_rightblock: printf("]");
			break;
		case sym_arrow: printf("->");
			break;
		case sym_and: printf("&");
			break;
		case sym_or: printf("|");
			break;
		case sym_less: printf("<");
			break;
		case sym_assign: printf("=");
			break;
		case sym_greater: printf(">");
			break;
		case sym_plus: printf("+");
			break;
		case sym_minus: printf("-");
			break;
		case sym_guard: printf("[]");
			break;
		case sym_mult: printf("*");
			break;
		case sym_div: printf("/");
			break;
		case sym_intdiv: printf("intdiv");
			break;
		case sym_not: printf("~");
			break;
		case sym_leftbracket: printf("(");
			break;
		case sym_rightbracket: printf(")");
			break;
		case sym_stop: printf(".");
			break;
		case sym_eol: printf("\n");
			break;
		case sym_space: printf(" ");
			break;
		default: printf("default case : not sure what to print\n");
			break;
	}
}

int main (int argc,char** argv){
	int tokens=0;
	char choice ;
	
	if( argc != 2 )
    {
        printf( "Usage: testScanner <filename>\n" );
        printf( "\twhere <filename> is the file to be scanned\n" );
    }
	else
	{
		FILE* fptr = init(argv[1]);
		token_str* token;
		token = getToken();
		
		while (token->sym_type != sym_eof)
		{
			tokens++;
			
			printSym(token);
			token = getToken();
		}
		
		fclose(fptr);
		printf("file closed\n");
		
		printf("Would you like to parse? Y/N :\n");
		scanf("%c",&choice);
		if (choice == 'y')
		{
			printf("status = %d\n",Block());
		}
		else
		{
			printf("Goodbye --- wankers :)\n");
		}
		
	printf("%d tokens\n",tokens);
	}
}


