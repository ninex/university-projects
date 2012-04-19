/********************************************************
 * 	Parser.c					*
 *							*
 * 	Written by Abrie Greeff				*
 *							*
 * 	Part of Software design: Compiler project (2005)*
 *							*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer	*
 ********************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/scanner.h"
#include "include/error.h"
#include "include/symboltable.h"
#include "include/parser.h"


static token_str* token;
static char last_expected[MAX_LEN+1], last_found[MAX_LEN+1], proc[MAX_LEN], def_name[MAX_LEN+1], const_name[MAX_LEN+1];
static int need_priority, level, val;
static enum types type;
//static int express = FALSE;
enum symtype sym_type;
varlist* list;
int isInt;

#ifdef DEBUG_PARSER
	static int spacing, spacing_count;
	#define PRINT_PARSE char func[MAX_LEN];\
		strcpy(func,__FUNCTION__);\
		spacing_count = 0;\
		while (spacing_count < spacing){\
			spacing_count = spacing_count + 1;\
			printf(" ");\
		}\
		printf("enter: %s\n",__FUNCTION__);\
		spacing = spacing + 2;
	#define END_PARSE_PRINT end_parse_print(func)
	#define PRINT_PARSE_NAME spacing_count = 0;\
	while (spacing_count < spacing){\
		spacing_count = spacing_count + 1;\
		printf(" ");\
	}
	/*
	 * ends the parse print
	 */
	int end_parse_print(char* func){
		spacing = spacing - 2;
		spacing_count = 0;
		while (spacing_count < spacing){
			spacing_count = spacing_count + 1;
			printf(" ");
		}
		printf("exit: %s\n",func);
		return FALSE;
	}
#else
	#define PRINT_PARSE
	#define END_PARSE_PRINT FALSE
	#define PRINT_PARSE_NAME
#endif

#ifdef PARSER_PRINTING
	#define print(x, ...) printf(x, ## __VA_ARGS__)
#else
	#define print(x, ...)
#endif


/*
 * The main function of parser.c
 * This function sets us up for parsing
 * and then parses the file
 * an error is returned when there's a problem
 */
int parse(){
	set_count();
	//read the first token
	token = getToken();
	//was a token read?
	if (token != NULL){
		//we need priorities
		need_priority = TRUE;
		#ifdef DEBUG_PARSER
			spacing = 0;
		#endif
		//parse the file
		return Program();
	}else{
		//there was a error
		return FALSE;
	}
}

/*
 * This function is called by the function to tell the parser
 * what token is expected to be read. This function then returns
 * according to what token was found. If a prioritized expect
 * isn't satisfied we must save what was expected and what was found.
 * Because this means that there is a parsing error.
 */ 
int expect(enum symtype sym_type,int priority){
	
	//check if the token expected is this one
	if (token->sym_type == sym_type){
		//if it is then get the next token and return True
		token = getToken();		
		return TRUE;
	}else{
		//it wasn't. so return false
		//if a prioritized expect wasn't satisfied go into the if
		if (priority && need_priority){
			//what was the last expected symbol?
			sprintf(last_expected,"%s",set_error(sym_type));
			//what was the symbol that was found?
			sprintf(last_found,"%s",set_error(token->sym_type));
			//call the error handler
			ERR_HANDLER(5);
			//we don't need to handle a priority case again
			need_priority = FALSE;
		}
		return FALSE;
	}
}

/*
 * Returns what was expected for the last error
 */
char* expect_err(){
	return last_expected;
}

/*
 * Return what was found for the last error
 */
char* found(){
	return last_found;
}

void clear_list()
{
	varlist* temp;	
	while (list != NULL)
	{
		temp = list;
		list = list->next;
		free(temp);
	}
}


/* ====================================================================	*
 * From here every function is a production rule as defined in the EBNF	*
 * If a rule isn't satisfied false is returned, else true		*
 * When expect is called with high priority then that symbol must	*
 * be part of the rule. This corresponds with the follow sets		*
 * when the first and follow sets of the EBNF is computed		*
 * ====================================================================	*/


/* 
 * The main calling function of the parser.
 * All programs that we parse must be parsed from here
 * The parser will descend recursively in this function
 * We also check if the end of the file was found after the last fullstop
 * Production rule: Program = Block "."
 */
int Program(){
	PRINT_PARSE
	level = -1;
	list = NULL;
	sprintf(proc,"_main");
	print("Defining default constants\n");
	insert_var(proc,"true",c_bool,1);
	insert_var(proc,"false",c_bool,0);
	return (Block() && expect(sym_stop,1) && expect(sym_eof,1)) | END_PARSE_PRINT ;
}

/*
 * Every procedure and the main program always contains this
 * block. Everything in a block happens between a begin
 * and an end.
 * Production rule: Block = "begin" DefinitionPart StatementPart "end"
 */
int Block(){
	PRINT_PARSE
	if (expect(sym_begin,1)){
		level = level + 1;       	
		if (DefinitionPart() && StatementPart()){
			level = level - 1;
			//printf("*****************************************************\n");
			//printSymTable();
			remove_proc(proc);
			//printf("=====================================================\n");
			//printSymTable();
			return (expect(sym_end,1)) | END_PARSE_PRINT;
		}else{
			return END_PARSE_PRINT;
		}
	}else{
		return END_PARSE_PRINT;
	}
}

/*
 * All definitions in a program is handled by this
 * production rule. Because there's curl brackets in the 
 * rule this means there may by zero or more definitions.
 * Production rule: DefinitionPart = {Definition ";"}
 */
int DefinitionPart(){
	PRINT_PARSE
	if (Definition()){		
		if (expect(sym_semicolon,1)){
			//a complete definition
			//now look if there's a next one
			return DefinitionPart() | END_PARSE_PRINT ;
		}else{
			//invalid definition
			return END_PARSE_PRINT ;
		}
	}else{
		//no definition
		return TRUE | END_PARSE_PRINT ;
	}	
}

/*
 * Checks which of the 3 types of definitions we have
 * Production rule: Definition = ConstantDefinition | VariableDefinition | ProcedureDefinition
 */
int Definition(){
	PRINT_PARSE
	return (ConstantDefinition() || VariableDefinition() || ProcedureDefinition()) | END_PARSE_PRINT ;
}
/*
 * A constant is being defined
 * production rule: ConstantDefinition = "const" ConstantName "=" Constant
 */
int ConstantDefinition(){
	PRINT_PARSE
	if (expect(sym_const,0)){
		       	if (ConstantName(1) && expect(sym_equal,1)){
				char name[MAX_LEN];
				int temp;
				sprintf(name,"%s",const_name);
				enum types expr_type = no_type;
			       	if (Constant(&expr_type,FALSE,&temp)){
					print("in level %d, procedure %s want to define constant %s to %d\n",level,proc,def_name,val);
					if (insert_var(proc,name,type,val) == FALSE)
					{
						ERR_HANDLER(6);
					}
					
					return TRUE | END_PARSE_PRINT;
				}else{
					return END_PARSE_PRINT ;
				}
			}else{
				return END_PARSE_PRINT;
			}
	}else{
		return FALSE;
	}
}

/*
 * A Variable is being declared
 * production rule: VariableDefinition = TypeSymbol VariableList | 
 * 					TypeSymbol "array" VariableList "[" Constant "]"
 */
int VariableDefinition(){
	PRINT_PARSE
	if (TypeSymbol()){
		if (VariableList()){
			return TRUE | END_PARSE_PRINT ;
		}else{
			enum types expr_type = no_type;
			int temp,temp2;
			temp2 = FALSE;
			if ((expect(sym_array,1) && VariableName(1) && expect(sym_leftblock,1) && Constant(&expr_type,&temp2,&temp) && expect(sym_rightblock,1))){
				print("\tdefine array %s length %d\n",def_name,val);
					int ret = FALSE;
					if (sym_type == 9)
					{
						ret = insert_var(proc,def_name,a_int,val);
					}else if (sym_type == 2)
					{
						ret = insert_var(proc,def_name,a_bool,val);
					}
				if (ret == FALSE)
				{
					ERR_HANDLER(7);
				}
				if (val <= 0)
				{
					ERR_HANDLER(8);
				}
				return TRUE | END_PARSE_PRINT;
			}else{
				return END_PARSE_PRINT;
			}
		}
	}else{
		return END_PARSE_PRINT ;
	}
}

/* 
 * We expect a type of symbol
 * integer or boolean
 * production rule: TypeSymbol = "integer" | "boolean"
 */
int TypeSymbol(){
	PRINT_PARSE
	sym_type = token->sym_type;
	return (expect(sym_int,0) || expect(sym_bool,0)) | END_PARSE_PRINT ;
}
/*
 * A list of one or more variables is expected
 * Variables are seperated by commas
 * production rule: VariableName {, VariableName}
 */
int VariableList(){
	PRINT_PARSE
	if (VariableName(0)){
		int ret = FALSE;
		if (sym_type == 9)
		{
			ret = insert_var(proc,def_name,t_int,0);
		}else if (sym_type == 2)
		{
			ret = insert_var(proc,def_name,t_bool,0);
		}
		if (ret == FALSE)
		{
			ERR_HANDLER(9);
		}
		print("in level %d, procedure %s want to define variable %s of type %d (int is 9 and bool 2)\n",level,proc,def_name,sym_type);
		//while a comma is found look for the next variable
		while (expect(sym_comma,0)){
			return VariableList() | END_PARSE_PRINT ;
		}
		//we parsed the whole list
		return TRUE | END_PARSE_PRINT ;		
	}else{
		return END_PARSE_PRINT ;
	}
}

/*
 * This is the definition of a procedure
 * production rule: ProcedureDefinition = "proc" ProcedureName Block
 */
int ProcedureDefinition(){
	PRINT_PARSE
	if (expect(sym_proc,0) && ProcedureName(1)){
		sprintf(proc,"%s",def_name);
		if (insert_proc(def_name) == FALSE)
		{
			ERR_HANDLER(10);
		}
		print("defining procedure %s at level %d\n",def_name,level);
		
	    return FormalParameters() && Block() | END_PARSE_PRINT ;
	}else{
		return END_PARSE_PRINT;
	}
}

int FormalParameters()
{
	PRINT_PARSE
	if (expect(sym_leftbracket,0))
	{
		if ( FPSection())
		{
			while (expect(sym_semicolon,0))
			{
				if (!(FPSection()))
				{
					ERR_HANDLER(30);
				}
			}
		}
		return (expect(sym_rightbracket,1)) | END_PARSE_PRINT;
	}else
	{
		return TRUE | END_PARSE_PRINT;
	}
	
}

int FPSection()
{
	PRINT_PARSE
	if (TypeSymbol())
	{
		expect(sym_value,0);
		printf("Declaring a parameter\n");
		return (VariableName(1)) | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT;
}

/* This is a block of zero or more statements separated by semi-colons
 * production rule: StatementPart = {Statement ";"}
 */ 
int StatementPart(){
	PRINT_PARSE
	if (Statement()){
		//is the statement closed by a semi-colon?
		if (expect(sym_semicolon,1)){
			//do the next Statement
			return StatementPart() | END_PARSE_PRINT ;
		}else{
			return END_PARSE_PRINT ;
		}
	}else{
		return TRUE | END_PARSE_PRINT ;
	}
}

/* A statement
 * A statement can be one of the 7 options in the production rule
 * production rule: Statement =  EmptyStatement || 
				ReadStatement || 
				WriteStatement || 
				AssignmentStatement || 
				ProcedureStatement || 
				IfStatement || 
				DoStatement
 */
int Statement(){
	PRINT_PARSE
 	int goback = FALSE;
	return (ProcedureStatement(&goback) || 
		EmptyStatement() || 
		ReadStatement() || 
		WriteStatement() || 
		AssignmentStatement(goback) || 
		IfStatement() || 
		DoStatement()) | END_PARSE_PRINT ;
}
/* An empty statement
 * production rule: EmptyStatement = "skip"
 */
int EmptyStatement(){
	PRINT_PARSE
	return expect(sym_skip,0) | END_PARSE_PRINT ;
}
/* A read statement
 * production rule: ReadStatement = "read" VariableAccessList
 */ 
int ReadStatement(){
	PRINT_PARSE
	clear_list();
	return (expect(sym_read,0) && VariableAccessList(FALSE)) | END_PARSE_PRINT ;
}

/* A list of variables that needs to be accessed
 * it must contain one or more variables separated by commas
 * production rule: VariableAccessList = VariableAccess { "," VariableAccess }
 */ 
int VariableAccessList(int goback){
	PRINT_PARSE
	enum types expr_type = no_type;
	int temp;
	if (VariableAccess(&expr_type,goback,&temp)){
		varlist* temp;
		temp = malloc(sizeof(struct varlist));
		temp->type = expr_type;
		temp->next = NULL;
		if (list == NULL)
		{
			list = temp;
		}else
		{
			varlist* cur;
			cur = list;
			while (cur->next != NULL)
			{
				cur = cur->next;
			}
			cur->next = temp;
		}
		//is there another variableaccess?
		if (expect(sym_comma,0)){
			return VariableAccessList(FALSE) | END_PARSE_PRINT ;
		}else{
			return TRUE | END_PARSE_PRINT ;
		}
	}else{
		return END_PARSE_PRINT ;
	}
}

/* A write statement
 * production rule: WriteStatement = "write" ExpressionList
 */
int WriteStatement(){
	PRINT_PARSE
	return (expect(sym_write,0) && ExpressionList(TRUE)) | END_PARSE_PRINT ;
}

/* A list of one or more expressions separated by commas
 * production rule: ExpressionList = Expression {"," Expression }
 */
int ExpressionList(int nolist){
	PRINT_PARSE
	enum types expr_type = no_type;
	int value = 0;
	if (Expression(&expr_type,&value)){
		print("value = %d\n",value);
		if (!nolist)
		{
			
			if (list == NULL)
			{
				ERR_HANDLER(16);
			}else if (list->type == expr_type)
			{
				varlist* temp;
				temp = list;
				list = list->next;
				free(temp);
			}else
			{
				ERR_HANDLER(17);
			}
		}else
		{
			clear_list();
		}
		//is there another expression?
		if (expect(sym_comma,0)){
			return ExpressionList(nolist) | END_PARSE_PRINT ;
		}else{
			if (list != NULL)
			{
				ERR_HANDLER(18);
			}
			return TRUE | END_PARSE_PRINT ;
		}
	}else{
		return END_PARSE_PRINT ;
	}	
}

/* An assignment
 * production rule: AssignmentStatement = VariableAccessList ":=" ExpressionList
 */
int AssignmentStatement(int goback){
	PRINT_PARSE
	clear_list();
	return (VariableAccessList(goback) && expect(sym_assign,1) && ExpressionList(FALSE)) | END_PARSE_PRINT ;
}

/* A procedure that is called
 * production rule: ProcedureStatement = "call" ProcedureName
 */
int ProcedureStatement(int *goback){
	PRINT_PARSE
	if (ProcedureName(0)){
		if (lookup_proc(def_name,TRUE) == NULL)
		{
			*goback = TRUE;
			return END_PARSE_PRINT;
			//ERR_HANDLER(11);
		}
		
		print("Successful call to %s\n",def_name);
		return ActualParameters() | END_PARSE_PRINT;			
	}else{
		return END_PARSE_PRINT;
	}
}

int ActualParameters()
{
	PRINT_PARSE
	if (expect(sym_leftbracket,0))
	{
		enum types expr_type;
		int value;
		if (Expression(&expr_type,&value))
		{
			while (expect(sym_comma,0))
			{
				if (!(Expression(&expr_type,&value)))
				{
					ERR_HANDLER(31);
				}
			}
		}
		return expect(sym_rightbracket,1) | END_PARSE_PRINT;
	}
	return TRUE | END_PARSE_PRINT;
}

/* An if statement
 * production rule: IfStatement = "if" GuardedCommandList "fi"
 */
int IfStatement(){
	PRINT_PARSE
	return (expect(sym_if,0) && GuardedCommandList() && expect(sym_fi,1)) | END_PARSE_PRINT ;
}

/* A do statement
 * production rule: DoStatement = "do" GuardedCommandList "od"
 */
int DoStatement(){
	PRINT_PARSE
	return (expect(sym_do,0) && GuardedCommandList() && expect(sym_od,1)) | END_PARSE_PRINT ;
}
/* a list of one or more guarded commands
 * refer to the next function's comments
 * to see what a guarded command is
 * production rule: GuardedCommandList = GuardedCommand { "[]" GuardedCommand }
 */
int GuardedCommandList(){
	PRINT_PARSE
	if (GuardedCommand()){
		//does another guarded command follow?
		if (expect(sym_guard,0)){
			return GuardedCommandList() || END_PARSE_PRINT ;
		}else{
			return TRUE || END_PARSE_PRINT ;
		}
	}else{
		return FALSE || END_PARSE_PRINT ;
	}	
}

/* a guarded command
 * This is a boolean expression followed by a statement to be executed
 * if true
 * production rule: GuardedCommand = Expression "->" StatementPart
 */
int GuardedCommand(){
	PRINT_PARSE
	enum types expr_type = no_type;
	int temp;
	if (Expression(&expr_type,&temp))
	{
		if (expr_type != t_bool)
		{
			ERR_HANDLER(27);
		}
		return (expect(sym_arrow,1) && StatementPart()) | END_PARSE_PRINT ;
	}
	return END_PARSE_PRINT;	 
}

/* A boolean expression
 * production rule: Expression = PrimaryExpression {PrimaryOperator PrimaryExpression}
 */
int Expression(enum types *expr_type,int* value){
	PRINT_PARSE
	
	if (PrimaryExpression(expr_type,value)){
		enum types temp = *expr_type;
		if (PrimaryOperator()){
			if (Expression(expr_type,value))
			{
				if ((temp == t_int) || (*expr_type == t_int))
				{
					ERR_HANDLER(26);
				}
				if (temp != *expr_type)
				{
					ERR_HANDLER(25);
				}
				*expr_type = t_bool;
				return TRUE | END_PARSE_PRINT ;
			}else
			{
				return END_PARSE_PRINT;
			}
		}else{
			return TRUE | END_PARSE_PRINT ;
		}
	}else{
		return END_PARSE_PRINT ;
	}
}

/* Primary Operators
 * it may be an AND or an OR
 * production rule: PrimaryOperator = "&" | "|"
 */
int PrimaryOperator(){
	PRINT_PARSE
	return (expect(sym_and,0) || expect(sym_or,0)) | END_PARSE_PRINT ;
}

/* Primary expressions
 * may contain "RelationalOperator SimpleExpression" zero or one times
 * production rule: PrimaryExpression = SimpleExpression [RelationalOperator SimpleExpression]
 */
int PrimaryExpression(enum types *expr_type,int* value){
	PRINT_PARSE
	
	if (SimpleExpression(expr_type,value)){
	
		if (RelationalOperator()){
			enum types temp = *expr_type;
			if (SimpleExpression(expr_type,value))
			{
				if (temp != *expr_type)
				{
					ERR_HANDLER(24);
					return END_PARSE_PRINT;
				}else
				{
					*expr_type = t_bool;
					return TRUE  | END_PARSE_PRINT ;
				}
			}
			return END_PARSE_PRINT ;
		}else{
			return TRUE | END_PARSE_PRINT ;
		}
	}else{
		return END_PARSE_PRINT ;
	}	
}

/* Relational Operators
 * can be "<", "=" or ">"
 * production rule: RelationalOperator = "<" | "=" | ">"
 */
int RelationalOperator(){
	PRINT_PARSE
	return (expect(sym_less,0) || expect(sym_equal,0) || expect(sym_greater,0)) | END_PARSE_PRINT ;
}

/* A simple expression
 * Can be one or more terms added/subtracted
 * from each other. The first term can contain a unary minus
 * production rule: SimpleExpression = ["-"] Term {AddingOperator Term}
 */
int SimpleExpression(enum types *expr_type,int* value){
	PRINT_PARSE
	//Does the term have a minus in front
	if (expect(sym_minus,0)){
		if (Term(expr_type,value)){
			if (*expr_type != t_int)
			{
				ERR_HANDLER(20);
			}
			int temp_val = *value * -1;
			int inttemp;
			int add;
			inttemp = isInt;
			while (AddingOperator(&add))
			{
				
				//printf("value in term %d %d\n",temp_val,*value);
				if (!(Term(expr_type,value)))
				{
					return END_PARSE_PRINT ;
				}
				
				if (*expr_type != t_int)
				{
					ERR_HANDLER(21);
				}
				if ((isInt == FALSE) || (inttemp == FALSE)) 
				{
					temp_val = 0;
					isInt = FALSE;
					inttemp = FALSE;
				}else
				{
					if (add)
					{
						temp_val = *value + temp_val;
					}else
					{
						temp_val = temp_val - *value;
					}
				}
			}
			*value = temp_val;
			return TRUE | END_PARSE_PRINT ;
		}else{
			return END_PARSE_PRINT ;
		}
	}else{
	
		if (Term(expr_type,value))
		{
			int temp_val = *value;
			int inttemp;
			int add;
			enum types temp = *expr_type;
			inttemp = isInt;
			while (AddingOperator(&add))
			{
				
				if (!(Term(expr_type,value)))
				{
					return END_PARSE_PRINT ;
				}
				
				
				if ((*expr_type != t_int) || (temp != t_int))
				{
					ERR_HANDLER(21);
				}
				if ((isInt == FALSE) || (inttemp == FALSE))
				{
					isInt = FALSE;
					inttemp = FALSE;
					temp_val = 0;
				}else
				{
					if (add)
					{
						temp_val = *value + temp_val;
					}else
					{
						temp_val = temp_val - *value;
					}
				}
			}
			*value = temp_val;
			return TRUE | END_PARSE_PRINT ;
		}else{
			return END_PARSE_PRINT ;
		}
	}	
}

/* The adding operators, + and -
 * production rule: AddingOperator = "+" or "-"
 */
int AddingOperator(int * add){
	PRINT_PARSE
	
	if (expect(sym_plus,0))
	{
		*add = TRUE;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_minus,0))
	{
		*add = FALSE;
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

/* A term
 * This is factors that may be separated by multiplying operators
 * production rule: Term = Factor {MultiplyingOperator Term}
 */
int Term(enum types *expr_type,int* value){
	PRINT_PARSE
	
	if (Factor(expr_type,value))
	{
		enum types type = *expr_type;
		int temp_val = *value;
		int mult;
		int inttemp = isInt;
		if (MultiplyingOperator(&mult))
		{
			if (Term(expr_type,value))
			{
				if ((*expr_type != t_int) || (type != t_int))
				{
					ERR_HANDLER(22);
				}
				if ((isInt == FALSE) || (inttemp == FALSE))
				{
					isInt = FALSE;
					inttemp = FALSE;
					temp_val = 0;
				}else
				{
					if (mult == 0)
					{
						print("Multiplying\n");
						temp_val = temp_val * *value;
					}
					//if (*value
					if (mult == 1)
					{
						print("Division\n");
						if (*value == 0)
						{
							ERR_HANDLER(29);
						}
						temp_val = temp_val / *value;
					}
					if (mult == 2)
					{
						print("IntDivision\n");
						if (temp_val == 0)
						{
							ERR_HANDLER(29);
						}
						temp_val = *value / temp_val;
					}
				}
				*value = temp_val;
				return TRUE | END_PARSE_PRINT ;
			}
			return END_PARSE_PRINT ;
		}else{
			return TRUE | END_PARSE_PRINT ;
		}
	}else{
		return END_PARSE_PRINT ;
	}
}

/* The multiplying operators
 * production rule: MultiplyingOperators = "*" | "/" | "\"
 */
int MultiplyingOperator(int * mult){
	PRINT_PARSE	
	if (expect(sym_mult,0))
	{
		*mult = 0;
		return TRUE | END_PARSE_PRINT;
	} 
	if (expect(sym_div,0))
	{
		*mult = 1;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_intdiv,0))
	{
		*mult = 2;
		return TRUE | END_PARSE_PRINT;
	}
		 
	return END_PARSE_PRINT ;
}

/*A factor
 * A factor can be a constant, a variable, a expression or not a factor
 * production rule: Factor = Constant | VariableAccess | "(" Expression ")" | "~" Factor
 */
int Factor(enum types *expr_type,int* value){
	PRINT_PARSE
	int goback = TRUE;
	if (Constant(expr_type,&goback,value) || VariableAccess(expr_type,goback,value) ||
		(expect(sym_leftbracket,0) && Expression(expr_type,value) && expect(sym_rightbracket,1)))
	{
		return TRUE | END_PARSE_PRINT;
	}
	if ((expect(sym_not,0) && Factor(expr_type,value)))
	{
		if (*expr_type != t_bool)
		{
			ERR_HANDLER(23);
		}
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

/* VariableAccess
 * access to a variable
 * production rule: VariableAccess = VariableName [IndexedSelector]
 */
int VariableAccess(enum types *expr_type, int goback,int* value){
	PRINT_PARSE	
	if ((goback) || (VariableName(0))){
		var_node* temp = lookup_var(proc,def_name);
		print("Lookup needed for %s in procedure %s at level %d, val %d type %d\n",def_name,proc,level,temp->value,temp->type);
		if (temp == NULL)
		{
			ERR_HANDLER(12);
			return END_PARSE_PRINT;
		}else
		{
			*value = 0;
			type = temp->type;
			if (temp->value != 0)
			{
				if ((temp->type == c_bool) || (temp->type == c_int))
				{
					//expr_type = temp->type;
					ERR_HANDLER(15);
					return TRUE | END_PARSE_PRINT;
				}else
				{
					return IndexedSelector(expr_type) | END_PARSE_PRINT;
				}
			}else
			{
				*expr_type = type;	
				return TRUE | END_PARSE_PRINT;
			}
		}
		
		//return IndexedSelector() | END_PARSE_PRINT ;
	}else{
		return END_PARSE_PRINT ;
	}
}

/* Indexed selector
 * used to index an array
 * production rule: IndexedSelector = "[" Expression "]"
 */
int IndexedSelector(enum types* expr_type){
	PRINT_PARSE
	if (expect(sym_leftblock,1))
	{
		var_node* temp = lookup_var(proc,def_name);
		if ((temp == NULL) || ((temp->type != a_int) && (temp->type != a_bool)) || (temp->value == 0 ))
		{
			ERR_HANDLER(13);
		}
		print("Lookup into array needed in procedure %s at level %d\n",proc,level);
		enum types this_expr_type = no_type;
		int this_value;
		if (Expression(&this_expr_type,&this_value) && expect(sym_rightblock,1))
		{
			if (this_expr_type != t_int)
			{
				ERR_HANDLER(19);
			}
			if ((this_value < 0) || (this_value >= temp->value))
			{
				ERR_HANDLER(28);
			}
			if (temp->type == a_int)
			{print("I\n");
				*expr_type = t_int;
			}else
			{print("I\n");
				*expr_type = t_bool;
			}
			return TRUE | END_PARSE_PRINT;
		}
		return END_PARSE_PRINT ;
	}else{
		return END_PARSE_PRINT;
	}
}

/* a constant
 * can be a number or a defined constant
 * production rule: Constant = Numeral | ConstantName
 */
int Constant(enum types *expr_type, int* goback,int* value){
	PRINT_PARSE	
	if (Numeral())
	{
		*expr_type = t_int;
		*value = val;
		type = c_int;
		return TRUE | END_PARSE_PRINT; 
	}else if (ConstantName(0)) 
	{
		var_node* temp = lookup_var(proc,const_name);
		
		if ((temp == NULL) || ((temp->type != c_bool) && (temp->type != c_int)))
		{
			if (*goback == FALSE)
			{
				ERR_HANDLER(14);
			}
			print("%s not a constant goback = %d\n", const_name, *goback);
			sprintf(def_name,"%s",const_name);
			return END_PARSE_PRINT;
		}else
		{
			*value = temp->value;
			if (temp->type == c_int)
			{
				type = c_int;
				*expr_type = t_int;
			}else
			{
				type = c_bool;
				*expr_type = t_bool;
			}
			
			val = temp->value;
			print("Constant %s defined as %d\n",const_name,val);
			return TRUE | END_PARSE_PRINT;
		}
	}
	*goback = FALSE;
	return END_PARSE_PRINT;
}

/* A number is expected
 * production rule: Numeral = Digit {Digit}
 */
int Numeral(){
	PRINT_PARSE
	val = token->val;
	
	if (expect(sym_number,0))
	{
		isInt = TRUE;
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

/* A name is expected
 * production rule: Name = Letter {Letter | Digit}
 */
int Name(int priority){
	PRINT_PARSE
	 
	if (expect(sym_id,priority)){
		isInt = FALSE;
		return TRUE | END_PARSE_PRINT;
	}else{
		return	END_PARSE_PRINT;
	}
}

int ConstantName(int priority){
	PRINT_PARSE
	sprintf(const_name,"%s",token->name);
	return (Name(priority)) | END_PARSE_PRINT ;
}
int VariableName(int priority){
	PRINT_PARSE
	sprintf(def_name,"%s",token->name);
	return Name(priority) | END_PARSE_PRINT ;
}
int ProcedureName(int priority){
	PRINT_PARSE
	sprintf(def_name,"%s",token->name);
	return Name(priority) | END_PARSE_PRINT ;
}

