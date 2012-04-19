%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntaxtree.h"
#define YYDEBUG 1

extern FILE* yyin;
//The syntaxtree that the parser builds
TreeNode* tree=NULL; 
//lineno in source 
int lineno;

TreeNode* newStmtNode(StmtKind kind);
TreeNode* newExpNode(ExpKind kind);
void printTree(TreeNode * tree);
void yyerror(char * message);



%}
%union{
int val;
char* name;
TreeNode* tree;
}

%token IF THEN ELSE END REPEAT UNTIL SCAN PRINT ID NUM EQUAL FOR WHILE RETURN
%type <val> NUM
%type <name> ID
%type <tree> stmt_seq stmt if_stmt repeat_stmt for_stmt while_stmt assign_stmt
      read_stmt write_stmt exp simple_exp term factor


%%
// Your Part 

program : stmt_seq {tree = $1;}

stmt_seq: stmt_seq stmt ';'	{
					TreeNode *t = $1;
					while(t->sibling != NULL){
						t=t->sibling;
					}

						t->sibling=$2;
						$$=$1;
				
				}

| stmt ';' {$$ = $1;}

stmt: if_stmt {$$ = $1;}
| repeat_stmt {$$ = $1;}
| assign_stmt {$$ = $1;}
| read_stmt {$$ =  $1;}
| write_stmt {$$ = $1;}
| for_stmt {$$ = $1;}
| while_stmt {$$ = $1;}

if_stmt: IF exp stmt_seq END {$$=newStmtNode(IfK);
       $$->child[0]=$2;$$->child[1]=$3;}
| IF exp stmt_seq ELSE stmt_seq END {$$=newStmtNode(IfK);
       $$->child[0]=$2;$$->child[1]=$3;$$->child[3]=$5;}

repeat_stmt: REPEAT stmt_seq UNTIL '(' exp ')'{$$=newStmtNode(RepeatK);$$->child[0]=$2;$$->child[1]=$5;}

for_stmt: FOR '(' assign_stmt ',' exp ',' assign_stmt ')' stmt_seq END {$$=newStmtNode(ForK);
	$$->child[0]=$3;$$->child[1]=$5;$$->child[2]=$7;$$->child[3]=$9;}

while_stmt: WHILE '(' exp ')' stmt_seq END{$$=newStmtNode(WhileK);$$->child[0]=$3;$$->child[1]=$5;}

write_stmt : PRINT '(' exp ')' {$$=newStmtNode(WriteK);$$->child[0]=$3;}

assign_stmt: ID '=' exp{$$=newStmtNode(AssignK);$$->child[0] = $3;$$->attr.name = $1;}

read_stmt: SCAN '('ID ')'{$$=newStmtNode(ReadK);$$->attr.name = $3;}

exp: simple_exp '<' simple_exp{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op="<";}
| simple_exp '>' simple_exp{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op=">";}
| simple_exp EQUAL simple_exp{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op= "==";}
| simple_exp{$$=$1;}

simple_exp : simple_exp '+' term{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op="+";}
| simple_exp '-' term{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op="-";}
| term{$$=$1;}

term: term '*' factor{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op="*";}
| term '/' factor{$$=newExpNode(OpK);$$->child[0]=$1;$$->child[1]=$3;$$->attr.op="/";}
| factor{$$=$1;}


factor: '(' exp ')'{$$ = $2;}
| NUM {$$ = newExpNode(ConstK);$$->attr.val=$1;}
| ID{$$=newExpNode(IdK);$$->attr.name=$1;}
%%

int main(int argc,char ** argv){
  yydebug = 0;
  yyin = fopen(argv[1],"r");
  yyparse();
  printf("\n");
  printTree(tree);
  printf("\n");
}
 

void yyerror(char * message){
  printf("%s: line %d\n",message,lineno);
}


void printTree(TreeNode * tree ){
  static int indentno=0;
  int i;
  indentno+=2;  
  while (tree != NULL){
    for(i=0;i<indentno;i++)
      printf(" ");
    if (tree->nodekind==StmtK){
      switch (tree->kind.stmt){
        case IfK:
          printf("If\n");
          break;
        case RepeatK:
          printf("Repeat\n");
          break;
        case ForK:
          printf("For\n");
          break;
        case WhileK:
          printf("While\n");
          break;
        case AssignK:
          printf("Assign to: %s\n",tree->attr.name);
          break;
        case ReadK:
          printf("scan: %s\n",tree->attr.name);
          break;
        case WriteK:
          printf("print\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK){
      switch(tree->kind.exp){
        case OpK:
          printf("Op: %s\n",tree->attr.op);
          break;
        case ConstK:
          printf("const: %d\n",tree->attr.val);
          break;
        case IdK:
          printf("Id: %s\n",tree->attr.name);
          break;
      }
    }
    for(i=0;i<4;i++){
      if ((tree->child[1] != NULL) && (tree->child[2] != NULL) && (tree->child[i] != NULL))
        printf("\n"); 
      printTree(tree->child[i]);
    }
    tree = tree->sibling;
  }
  indentno-=2;;
}

TreeNode * newStmtNode(StmtKind kind){
  TreeNode * t = malloc(sizeof(TreeNode));
  int i;
  for(i=0;i<4;i++)
    t->child[i]=NULL;    
  t->sibling=NULL;
  t->nodekind=StmtK;
  t->kind.stmt = kind;
  return t;
}


TreeNode * newExpNode(ExpKind kind){
  TreeNode * t = malloc(sizeof(TreeNode));
  int i;
  for(i=0;i<4;i++)
    t->child[i] = NULL;    
  t->sibling = NULL;
  t->nodekind = ExpK;
  t->kind.exp = kind;
  return t;
}








