#ifndef PARSER_H
#define PARSER_H


// typedef struct item
// {
// 	enum types type;
// 	int val, isInt, a;
// }item;

int parse();

char* expect_err();

char* found();

void clear_list();

int Program();

//implicit declarations
int Block();
int DefinitionPart();
int Definition();
int ConstantDefinition();
int VariableDefinition();
int TypeSymbol();
int VariableList();
int ProcedureDefinition();
int FormalParameters();
int FPSection();
int StatementPart();
int Statement();
int EmptyStatement();
int ReadStatement();
int VariableAccessList(int goback);
int WriteStatement();
int ExpressionList();
int AssignmentStatement(int goback);
int ProcedureStatement(int *goback);
int ActualParameters();
int IfStatement();
int DoStatement();
int GuardedCommandList();
int GuardedCommand();
// int Expression(enum types* expr_type, int* value);
int Expression(cg_item *x);
int PrimaryOperator();
// int PrimaryExpression(enum types *expr_type,int* value);
int PrimaryExpression(cg_item *x);
int RelationalOperator();
// int SimpleExpression(enum types *expr_type,int* value);
int SimpleExpression(cg_item *x);
int AddingOperator(int * add);
// int Term(enum types *expr_type,int* value);
int Term(cg_item *x);
int MultiplyingOperator(int * mult);
// int Factor(enum types *expr_type,int* value);
int Factor(cg_item *x);
// int VariableAccess(enum types *expr_type, int goback,int* value);
int VariableAccess(cg_item *x, int goback);
int IndexedSelector(cg_item *x);
// int Constant(enum types *expr_type, int *goback,int* value);
int Constant(cg_item *x, int *goback);
int Numeral(cg_item *x);
int Name(cg_item *x,int priority);
int ConstantName(cg_item *x,int priority);
int VariableName(cg_item *x,int priority);
int ProcedureName(cg_item *x,int priority);
#endif
