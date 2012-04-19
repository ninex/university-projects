%{
#include <stdio.h>
#include "sym.h"
#define YYDEBUG 1
void yyerror(char*msg);

%}

%token NUMBER

%union {double val;char*name;}
%type <val> expr NUMBER
%type <name> REGISTER

%token NUMBER REGISTER

%left '+' '-'
%left '*' '/'

%right '^' '!'
%right SIN COS TAN
%right UMINUS

%%
lines:lines statement '\n' {printf(">>");}
	| /*epsilon*/
	| error '\n' {printf("errorererdfosdf\n");}

statement:expr {printf("%.3f\n",$1);}
	|REGISTER '=' expr {sym_look($1)->val=$3;}

expr:NUMBER {$$=$1;}
	| expr '+' expr {$$=$1+$3;}
	| expr '-' expr {$$=$1-$3;}
	| TAN '('expr ')' {$$=tan($3);}
	| REGISTER {$$=sym_look($1)->val;}
%%
int main()
{
	printf(">>");
	yydebug = 0;
	yyparse();
}
void yyerror(char*msg){printf("%s\n",msg);}
