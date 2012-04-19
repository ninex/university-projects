%{
  #include<stdio.h>
  int charCount = 0, wordCount = 0, lineCount = 0;
%}

word [^ \t\n]+
eol  \n

%%
{word}  {wordCount++; charCount += yyleng;}
{eol}   {charCount++; lineCount++;}
.         charCount++; 

%%
int main(int argc,char** argv)
{
  yyin=fopen(argv[1],"r");
	yylex();
	printf( "%d %d %d\n", lineCount, wordCount, charCount);
  fclose(yyin);
}
