#ifndef PARSER_H
#define PARSER_H
/********************************************************
 * 	Parser.h					*
 *							*
 * 	Written by Abrie Greeff				*
 *							*
 * 	Part of Software design: Compiler project (2005)*
 *							*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer	*
 ********************************************************/
 
 
 

/*
 * parse()
 *
 * Description: This function sets us up for parsing and then parses the file
 * an error is returned when there's a problem
 * Input: NONE
 * Output: Did the program parse
 */
int parse();

/*
 * expect_err()
 *
 * Description: Returns what was expected for the last error
 *
 * Input: None
 * Output: the last error
 */
char* expect_err();

/*
 * found()
 *
 * Description: Return what was found for the last error
 *
 * Input: None
 * Output: the last symbol found
 */
char* found();

/*
 * clearList()
 *
 * Description: clear the list of items used in assignments, writes and reads.
 * 
 * Input: The list to be cleared
 * Output: None
 */
void clear_list();

/* 
 * Program()
 *
 * Description: The main calling function of the parser. All programs that we parse must be parsed from this function.
 * The parser will descend recursively in this function. We also check if the end of the file was found after the last fullstop
 *
 * Production rule: Program = Block "."
 * Input: None
 * Output: If the program parsed sucessfully
 */
int Program();

//implicit declarations


/*
 * Block()
 *
 * Description: Every procedure and the main program always contains this block. Everything in a block happens between a begin
 * and an end.
 *
 * Production rule: Block = "begin" DefinitionPart StatementPart "end"
 * Input: None
 * Output: return if this rule parsed sucessfully
 */
int Block();

/*
 * DefinitionPart()
 *
 * Description: All definitions in a program is handled by this production rule. Because there's curl brackets in the 
 * rule this means there may by zero or more definitions.
 *
 * Production rule: DefinitionPart = {Definition ";"}
 * Input: None
 * Output: Returns if this function parsed sucessfully
 */
int DefinitionPart();

/*
 * Definition()
 *
 * Description: Checks which of the 3 types of definitions we have
 *
 * Production rule: Definition = ConstantDefinition | VariableDefinition | ProcedureDefinition
 * Input: None
 * Output: returns if the function parsed sucessfully
 */
int Definition();

/*
 * ConstantDefinition()
 *
 * Description: A constant is being defined
 *
 * Production rule: ConstantDefinition = "const" ConstantName "=" Constant
 * Input: None
 * Output: Returns if the function parsed sucessfully
 */
int ConstantDefinition();

/*
 * VariableDefinition()
 *
 * Description: A Variable is being declared
 *
 * Production rule: VariableDefinition = TypeSymbol VariableList | 
 * 					TypeSymbol "array" VariableList "[" Constant "]"
 * Input: None
 * Output: Returns if the function parsed succesfully
 */
int VariableDefinition();

/* 
 * TypeSymbol()
 *
 * Description: We expect a type of symbol integer or boolean
 *
 * Production rule: TypeSymbol = "integer" | "boolean"
 * Input: None
 * Output: Returns if the file parsed sucessfully
 */
int TypeSymbol();

/*
 * VariableList()
 *
 * Description: A list of one or more variables is expected. Variables are seperated by commas
 *
 * Production rule: VariableName {, VariableName}
 * Input: None
 * Output: Returns if the function parsed sucessfully
 */
int VariableList();

/*
 * ProcedureDefinition()
 *
 * Description: This is the definition of a procedure
 *
 * Production rule: ProcedureDefinition = "proc" ProcedureName Block
 * Input: None
 * Output: Return is the function parsed sucessfully
 */
int ProcedureDefinition();

/*
 * FormalParameters()
 *
 * Description: If there is any formal parameters defined between two brackets. they will be defined from inside
 *
 * Production rule: FormalParameters = "(" [FPSection {; FPSection }\ ")"
 * Input: None
 * Output: Returns if the function parsed sucesfully
 */
int FormalParameters();

/*
 * FPSection()
 *
 * Description: Formal parameters are defined in this procedure
 *
 * Production rule: FPSection = TypeSymbol ["value"] VariableName
 * Input: None
 * Output: Returns if the function parsed succesfully
 */
int FPSection();

/*
 * StatementPart()
 *
 * Description: This is a block of zero or more statements separated by semi-colons
 * Production rule: StatementPart = {Statement ";"}
 * Input: None
 * Output: returns if the function parsed sucesfully
 */
int StatementPart();

/* 
 * Statement()
 *
 * Description: A statement can be one of the 7 options in the production rule
 *
 * Production rule: Statement =  EmptyStatement || 
				ReadStatement || 
				WriteStatement || 
				AssignmentStatement || 
				ProcedureStatement || 
				IfStatement || 
				DoStatement
 * Input: None
 * Output: returns if the function parsed sucesfully
 */
int Statement();

/* 
 * EmptyStatement()
 *
 * Description: An empty statement that does nothing
 *
 * Production rule: EmptyStatement = "skip"
 * Input: None
 * Output: Returns if the function parsed sucesfully
 */
int EmptyStatement();

/* 
 * ReadStatement()
 *
 * Description: A read statement reads input from the user
 *
 * Production rule: ReadStatement = "read" VariableAccessList
 * Input: None
 * Outpt: returns if the function parsed sucesfully
 */
int ReadStatement();

/* 
 * VariableAccessList()
 *
 * Description: A list of variables that needs to be accessed. It must contain one or more variables separated by commas
 *
 * Production rule: VariableAccessList = VariableAccess { "," VariableAccess }
 * Input:If we need to goback a token if a sym_id was found, but not a variable
 * Output: Returns if the function parsed sucessfully
 */ 
int VariableAccessList(int goback);

/*
 * WriteStatement()
 * 
 * Description: A write statement that writes output to the keyboard
 *
 * Production rule: WriteStatement = "write" ExpressionList
 * Input: None
 * Output: Returns if the function parsed sucessfully
 */
int WriteStatement();

/*
 * ExpressionList()
 *
 * Description: A list of one or more expressions separated by commas
 *
 * Production rule: ExpressionList = Expression {"," Expression }
 * Input: If we need to evaluate a variableAccessList
 * Output: Returns if we parsed the function succesfully
 */
int ExpressionList();

/* 
 * AsignmentStatement()
 *
 * Description:An assignment to variable
 *
 * Production rule: AssignmentStatement = VariableAccessList ":=" ExpressionList
 * Input: If we need to go back if an id was found but it wasn't a variable
 * Output: returns if the function parsed sucessfully
 */
int AssignmentStatement(int goback);

/*
 * ProcedureStatement()
 *
 * Description: A procedure that is called
 *
 * Production rule: ProcedureStatement = ProcedureName [ActualParameters]
 * Input: If we need to goback if an id was found that wasn't a procedure
 * Output: If the function parsed sucessfully
 */
int ProcedureStatement(int *goback);

/*
 * ActualParameters()
 *
 * Description: The parameters to be passed to a called procedure
 *
 * Production rule: ActualParameters = "(" [Expression {"," Expression}] ")"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int ActualParameters();

/* 
 * IfStatement()
 *
 * Description: An if statement
 * Production rule: IfStatement = "if" GuardedCommandList "fi"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int IfStatement();

/* 
 * DoStatement()
 *
 * Description: A do loop statement
 * Production rule: DoStatement = "do" GuardedCommandList "od"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int DoStatement();

/*
 * GuardedCommandList()
 *
 * Description: A list of one or more guarded commands. Refer to the next function's comments
 * to see what a guarded command is
 *
 * Production rule: GuardedCommandList = GuardedCommand { "[]" GuardedCommand }
 * Input: A codegen item and the position of the first branch
 * Output: If the function parsed sucessfully
 */
int GuardedCommandList(cg_item *x, long *L);

/* 
 * GuardedCommand()
 *
 * Description: This is a boolean expression followed by a statement to be executed if evaluated true
 *
 * Production rule: GuardedCommand = Expression "->" StatementPart
 * Input: A codegen item and the position of the first branch
 * Output: If the function parsed sucessfully
 */
int GuardedCommand(cg_item *x, long *L);

/* 
 * Expression()
 *
 * Description: A boolean expression that may be evaluated between a optional AND and OR
 *
 * Production rule: Expression = PrimaryExpression {PrimaryOperator PrimaryExpression}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Expression(cg_item *x);

/* 
 * PrimaryOperator()
 *
 * Description: Primary Operators that may be an AND or an OR
 *
 * Production rule: PrimaryOperator = "&" | "|"
 * Input: The integer the op type must be written to
 * Output: If the function parsed sucessfully
 */
int PrimaryOperator(int *op);

/* 
 * PrimaryExpression()
 *
 * Description: Primary expressions may contain "RelationalOperator SimpleExpression" zero or one times
 *
 * Production rule: PrimaryExpression = SimpleExpression [RelationalOperator SimpleExpression]
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int PrimaryExpression(cg_item *x);

/*
 * RelationalOperator()
 * 
 * Description: Relational Operators can be "<", "=" or ">"
 *
 * Production rule: RelationalOperator = "<" | "=" | ">"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int RelationalOperator();

/*
 * SimpleExpression()
 *
 * Description: A simple expression can be one or more terms added/subtracted
 * from each other. The first term can contain a unary minus
 *
 * Production rule: SimpleExpression = ["-"] Term {AddingOperator Term}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int SimpleExpression(cg_item *x);

/* 
 * AddingOperator()
 *
 * Description: The adding operators, + and -
 *
 * Production rule: AddingOperator = "+" or "-"
 * Input: An integer to return the type of operation
 * Output: If the function parsed sucessfully
 */
int AddingOperator(int * add);

/* 
 * Term()
 *
 * Description: This is factors that may be separated by multiplying operators
 *
 * Production rule: Term = Factor {MultiplyingOperator Term}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Term(cg_item *x);

/*
 * MultiplyingOperator()
 *
 * Description: The multiplying operators
 *
 * Production rule: MultiplyingOperators = "*" | "/" | "\"
 * Input: An integer to set what type of operator we're using
 * Output: If the function parsed sucessfully
 */
int MultiplyingOperator(int * mult);

/*
 * Factor()
 *
 * Description: A factor can be a constant, a variable, a expression or NOT a factor
 *
 * Production rule: Factor = Constant | VariableAccess | "(" Expression ")" | "~" Factor
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Factor(cg_item *x);

/*
 * VariableAccess()
 *
 * Description: Access to a variable
 *
 * Production rule: VariableAccess = VariableName [IndexedSelector]
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int VariableAccess(cg_item *x, int goback);

/* 
 * IndexedSelector()
 *
 * Description:Used to index an array and work out the memory address of that index
 *
 * Production rule: IndexedSelector = "[" Expression "]"
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int IndexedSelector(cg_item *x);

/*
 * Constant()
 *
 * Description: Can be a number or a defined constant
 *
 * Production rule: Constant = Numeral | ConstantName
 * Input: A codegen item and if we need to go back if an id was found that isn't a constant
 * Output: If the function parsed sucessfully
 */
int Constant(cg_item *x, int *goback);

/*
 * Numeral()
 *
 * Description: A number is expected
 *
 * Production rule: Numeral = Digit {Digit}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Numeral(cg_item *x);

/*
 * Name() 
 *
 * Description: A name is expected
 *
 * production rule: Name = Letter {Letter | Digit}
 * Input: A codegen item and the priority we're expecting on the name
 * Output: If the function parsed sucessfully
 */
int Name(cg_item *x,int priority);

#endif
