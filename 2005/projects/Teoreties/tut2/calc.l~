%{
#include <stdio.h>
#include <string.h>
#include "calc.tab.h"
%}


%%
	/* remarks */
%.* ;

	/*lines that flow over*/
	
	/*Mathematical functions*/
	
	sin { return SIN;}
	cos { return COS;}
	tan { return TAN;}
	pi {return PI;}
	
	/* commands */
	
	quit { return 0;}
	clear { return CLEAR;}
	all {return ALL;}
	/* Register names */
	[a-zA-Z][a-zA-Z0-9]* {
		yylval.name = strdup(yytext);
		return REGISTER;
		}
		
	/*operators*/
	
	[-"!"+=/*()"^"\n] { 
			return yytext[0];
			}
	
	/*Skip whitespaces*/
	[ \t] ;
	
	/*anything else*/
	[0-9]+[.]?[0-9]* {
			 yylval.val = atof(yytext);
			 return NUMBER;}
	
	. yyerror ("Invalid character\n");
