%{
  #include<stdio.h>
  int upper = 0, lower = 0, syfer = 0, length = 0;
%}


upteks [A-Z]
loteks [a-z]
syfteks [0-9]
word [^ \n\t]+
eol  \n

%%
{upteks} {printf("hoesit\n");upper++;length++;}
{loteks} {printf("hoesit2\n");lower++;length++;}
{syfteks} {printf("hoesit3\n");syfer++;length++;}
. {length++;}

%%
int main(int argc,char** argv)
{
if (argc==2)
{
  yyin=fopen(argv[1],"r");  
}else
{yyin = stdin;
}
	yylex();
	printf("%d\n",length);
	if ((length < 8) || ((upper + lower + syfer) < 3))
	{
		printf("invalid password\n");
	}else
	{
		printf("Valid password\n");
	}
  fclose(yyin);
}
