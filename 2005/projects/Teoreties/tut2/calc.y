%{
#include <stdio.h>
/*all headers*/
#include "sym.h"

#define YYDEBUG 1

/*function prototypes*/

void yyerror(char *msg);

%}

%union { double val;
		  char * name;
		}
%type <val> expr NUMBER PI
%type <name> REGISTER

%token NUMBER QUIT
        REGISTER CLEAR PI ALL
		/* ALL */
		
%left '+' '-'
%left '*' '/'

%right '^' '!'
%right SIN COS TAN
%right UMINUS

/* rules */
%% lines : lines statement '\n' {printf(">>");}
		| /* epsilon */
		| error '\n' {yyerror("Plese re-enter");}

statement: expr { printf ("%.4f\n",$1);}
	   | REGISTER '=' expr { symlook($1)->value = $3;}
	   |CLEAR ALL{ sym_clear_all();}
	   |CLEAR REGISTER { sym_clear($2);}
expr:  NUMBER {$$ = $1;}
	   | expr '+' expr {$$ = $1 + $3;}
	   | expr '-' expr {$$ = $1 - $3;}
	   | expr '/' expr {$$ = $1 / $3;}
	   | expr '*' expr {$$ = $1 * $3;}
	   | SIN '(' expr ')' {$$ = sin($3);}
	   | COS '(' expr ')' {$$ = cos($3);}
	   | TAN '(' expr ')' {$$ = tan($3);}
	   | PI {$$ = 3.141592653589793;}
	   | '-' expr %prec UMINUS { $$ = -$2;}
	   | '(' expr')' {$$ = $2}
	   | REGISTER {$$ = symlook($1)->value;}
%%		 
int main()
{
	printf(">>");
	yydebug = 0;
	yyparse();
}

void yyerror(char *msg) {printf("%s\n",msg);}
