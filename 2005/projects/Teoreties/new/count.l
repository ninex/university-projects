%{
#include <string.h>
int count = 0;
%}

 
%x COMMENT


%%

[ \ta-zA-Z0-9";{}=_%-<>"|"#]*\n {printf("%d %s",count,yytext);count++;}

"/*" {BEGIN COMMENT;}
<COMMENT>.|\n ;
<COMMENT>"*/" {BEGIN INITIAL;}

"//" ;
    /*. {printf("problem with %s",yytext);}*/
%%
int main(int argc,char** argv)
{
	yyin = fopen(argv[1],"r");
	yylex();
	fclose(yyin);
}