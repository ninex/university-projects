#ifndef PARSER_H
#define PARSER_H

int Program();
int Block();
int DefinitionPart();
int Definition();
int ConstantDefinition();
int VariableDefinition();
int TypeSymbol();
int VariableList();
int ProcedureDefinition();
int StatementPart();
int Statement();
int EmptyStatement();
int ReadStatement();
int VariableAcessList();
int WriteStatement();
int ExpressionList();
int AssignmentStatement();
int ProcedureStatement();
int IfStatement();
int DoStatement();
int GuardedCommandList();
int GuardedCommand();
int Expression();
int PrimaryOperator();
int PrimaryExpression();
int RelationalOperator();
int SimpleExpression();
int AddingOperator();
int Term();
int MultiplyingOperator();
int Factor();
int VariableAccess();
int IndexedSelector();
int Constant();
int Numeral();
int Name();


#endif
