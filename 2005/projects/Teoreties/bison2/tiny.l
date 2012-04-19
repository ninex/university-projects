%{
#include "syntaxtree.h"
#include "tiny.tab.h"
#include "string.h"
extern int lineno;

%}

%x COMMENT

%%
"/*" {BEGIN COMMENT;}
<COMMENT>(.|\n) ;
<COMMENT>"*/" BEGIN INITIAL; 
"//".* ; 
"if" {return IF;}
"then" {return THEN;}
"else" {return ELSE;}
"end" {return END;}
"repeat" {return REPEAT;}
"until" {return UNTIL;}
"scan" {return SCAN;}
"print" {return PRINT;}
"for" {return FOR;}
"while" {return WHILE;}
"return" {return RETURN;}
== {return EQUAL;}
[0-9]+ {yylval.val = atoi(yytext); return NUM;}
[a-zA-Z]+ {yylval.name = strdup(yytext); return  ID;}
"\n" {lineno++;}
[-=<>+*/();,] {return yytext[0];}
[ \t]+ ;
. {printf("Symbol %c not recognized in line: %d\n",yytext[0],lineno);}
