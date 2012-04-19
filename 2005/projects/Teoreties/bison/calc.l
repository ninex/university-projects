%{
#include <stdio.h>
#include <string.h>
#include "calc.tab.h"
%}
%%
	 
%.* ;

sin {return SIN;}
cos {return COS;}
tan {return TAN;}

	
[a-zA-Z][a-zA-Z0-9]* 	{
				yylval.name = strdup(yytext);
				return REGISTER;
			}

[0-9]+[.]?[0-9]*	{
				yylval.val = atof(yytext);
				return NUMBER;
			}


[-=+*/"^""!"()\n]	{
				return yytext[0];
			}

[ \t] ;


. {printf("error\n");}