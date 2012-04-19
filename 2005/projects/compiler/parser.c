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
#include "include/types.h"
#include "include/scanner.h"
#include "include/error.h"
#include "include/symboltable.h"
#include "include/opcodes.h"
#include "include/codegen.h"                             
#include "include/parser.h"

static token_str* token;
static char last_expected[MAX_LEN+1], last_found[MAX_LEN+1], proc[MAX_LEN], def_name[MAX_LEN+1], const_name[MAX_LEN+1],call_name[MAX_LEN+1];
static int need_priority, level, val, elseIf, do_od;
static enum types type;
enum symtype sym_type;
enum opcodes op;
varlist* list;
var_node *params;

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
	// ends the parse print 
	int end_parse_print(char* func) {
		spacing = spacing - 2;
		spacing_count = 0;
		while (spacing_count < spacing) {
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
 * parse()
 *
 * Description: This function sets us up for parsing and then parses the file
 * an error is returned when there's a problem
 * Input: NONE
 * Output: Did the program parse
 */
int parse() {
	set_count();
	//read the first token
	token = getToken();
	//was a token read?
	if (token != NULL) {
		//we need priorities
		need_priority = TRUE;
		#ifdef DEBUG_PARSER
			spacing = 0;
		#endif
		//parse the file
		return Program();
	}else{
		//there was an unknown error
		return FALSE;
	}
}

/*
 * expect()
 *
 * Description: This function is called by the function to tell the parser what token is expected to be read. This function then returns
 * according to what token was found. If a prioritized expect isn't satisfied we must save what was expected and what was found.
 * Because this means that there is a parsing error.
 *
 * Input: The type of symbol expected and if it has a priority
 * Output: Return if the symbol was found
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
 * expect_err()
 *
 * Description: Returns what was expected for the last error
 *
 * Input: None
 * Output: the last error
 */
char* expect_err() {
	return last_expected;
}

/*
 * found()
 *
 * Description: Return what was found for the last error
 *
 * Input: None
 * Output: the last symbol found
 */
char* found() {
	return last_found;
}

/*
 * closeItem()
 *
 * Description: closes an item. If clear is set the register assigned to this item will be marked as available
 * 
 * Input: The item to be cleared
 * Output: None
 */
void closeItem(cg_item *x,int clear) {
	if ((x->mode == reg) && (clear))
	{
		clearReg(x->r);
	}
	free(x);
}

/*
 * clearList()
 *
 * Description: clear the list of items used in assignments, writes and reads.
 * 
 * Input: The list to be cleared
 * Output: None
 */
void clear_list() {
	varlist* temp;
	while (list != NULL)
	{
		temp = list;
		list = list->next;
		closeItem(temp->x,TRUE);
		free(temp);
	}
}

/*
 * newItem()
 *
 * Description: creates a new item that is used for code generation.
 * 
 * Input: None
 * Output: The newly created item.
 */
cg_item* newItem() {
	cg_item *x;
	x = malloc(sizeof(struct cg_item));
	x->type = no_type;
	x->a = 0;
	x->c = 0;
	x->mode = nomode;
	x->baseAddr = 0;
	x->isArray = FALSE;
	x->varIndex = FALSE;
	return x;
}

/*
 * newParam()
 * 
 * Description: creates a new parameter variable for insertion into the symboltable
 * 
 * Input: the item it must be created from
 * Output: the created item
 */
var_node* newParam(cg_item *x) {
	var_node *params;
	params = malloc(sizeof (struct var_node));
	params->next = NULL;
	params->prev = NULL;
	params->type = x->type;
	params->value = x->c;
	
	params->x = malloc(sizeof (struct cg_item));
	memcpy(params->x,x,sizeof (struct cg_item));
	x->isArray = FALSE;
	return params;
}

/*
 * ConstantName() 
 *
 * Description: A defined constant is expected
 *
 * Input: A codegen item and the priority we're expecting on the item
 * Output: If the function parsed sucessfully
 */
int ConstantName(cg_item *x, int priority){
	PRINT_PARSE
	sprintf(const_name,"%s",token->name);
	return (Name(x,priority)) | END_PARSE_PRINT ;
}

/*
 * VariableName() 
 *
 * Description: A defined variable is expected
 *
 * Input: A codegen item and the priority we're expecting on the item
 * Output: If the function parsed sucessfully
 */
int VariableName(cg_item *x, int priority){
	PRINT_PARSE
	sprintf(def_name,"%s",token->name);
	return Name(x,priority) | END_PARSE_PRINT ;
}

/*
 * ProcedureName() 
 *
 * Description: A defined procedure is expected
 *
 * Input: A codegen item and the priority we're expecting on the item
 * Output: If the function parsed sucessfully
 */
int ProcedureName(cg_item *x, int priority){
	PRINT_PARSE
	sprintf(def_name,"%s",token->name);
	return Name(x,priority) | END_PARSE_PRINT ;
}

/* ====================================================================	*
 * From here every function is a production rule as defined in the EBNF	*
 * If a rule isn't satisfied false is returned, else true		*
 * When expect is called with high priority then that symbol must	*
 * be part of the rule. This corresponds with the follow sets		*
 * when the first and follow sets of the EBNF is computed		*
 * ====================================================================	*/


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
int Program() {
	PRINT_PARSE
	procedure_node * temp;
	level = 0;
	list = NULL;
	//set the procedure as the main program
	sprintf(proc,"_main");
	print("Defining default constants\n");
	temp = lookup_proc(proc,FALSE);
	temp->level = 1;
	insert_var(proc,"true",c_bool,1);
	insert_var(proc,"false",c_bool,0);
	return (Block() && expect(sym_stop,1) && expect(sym_eof,1)) | END_PARSE_PRINT ;
}

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
int Block() {
	PRINT_PARSE
	procedure_node * temp;
	int disp;
	if (expect(sym_begin,1)) {
		//increase the level of the scope
		level = level + 1;
		//all definitions that needs to be declared for this procedure
		if (DefinitionPart()) {
			//get the procedure item from the symboltable. it's not in the call list
			temp = lookup_proc(proc,FALSE);
			disp = temp->disp;
			print("final displacement for %s is %d\n",proc,disp);
			//if this procedure isn't the main program it needs to be set up correctly before entering
			if (strcmp("_main",proc) != 0) {
				temp->pc = procEnter(-disp);
				print("starts at %lu\n",temp->pc);
			}
			else {
				start(-disp);
			}
			//all the statements in this procedure's block
			if  (StatementPart()) {
				//decrease the level of the scope
				level = level - 1;
				//if this procedure isn't the main program it needs to be set up correctly before exiting
				if (strcmp("_main",proc) != 0) {
					print("\tProcedure %s disp %d\n",proc,-disp);
					procReturn();
				}
				else {
					print("proc %s\n",proc);
					//the end of the main program has been reached. all cleanup must be done
					Close();
				}
				//remove this procedure from the list of procedures. it remains in it's parent's call list
				remove_proc(proc);
				return (expect(sym_end,1)) | END_PARSE_PRINT;
			}
			else {
				return END_PARSE_PRINT;
			}
		}
		else {
			return END_PARSE_PRINT;
		}
	}
	else {
		return END_PARSE_PRINT;
	}
}

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
int DefinitionPart() {
	PRINT_PARSE
	if (Definition()) {		
		if (expect(sym_semicolon,1)) {
			//a complete definition
			//now look if there's another definition
			return DefinitionPart() | END_PARSE_PRINT ;
		}
		else {
			//invalid definition
			return END_PARSE_PRINT ;
		}
	}
	else {
		//no definition
		return TRUE | END_PARSE_PRINT ;
	}	
}

/*
 * Definition()
 *
 * Description: Checks which of the 3 types of definitions we have
 *
 * Production rule: Definition = ConstantDefinition | VariableDefinition | ProcedureDefinition
 * Input: None
 * Output: returns if the function parsed sucessfully
 */
int Definition() {
	PRINT_PARSE
	return (ConstantDefinition() || VariableDefinition() || ProcedureDefinition()) | END_PARSE_PRINT ;
}

/*
 * ConstantDefinition()
 *
 * Description: A constant is being defined
 *
 * Production rule: ConstantDefinition = "const" ConstantName "=" Constant
 * Input: None
 * Output: Returns if the function parsed sucessfully
 */
int ConstantDefinition() {
	PRINT_PARSE
	char name[MAX_LEN];
	cg_item *x;
	if (expect(sym_const,0)) {
		
		if (ConstantName(NULL,1) && expect(sym_equal,1)) {
			x = newItem();
			sprintf(name,"%s",const_name);
			if (Constant(x,FALSE)) {
				print("in level %d, procedure %s want to define constant %s to %d\n",level,proc,def_name,x->c);
				if (insert_var(proc,name,type,val) == FALSE) {
					ERR_HANDLER(6);
				}
				closeItem(x,TRUE);
				return TRUE | END_PARSE_PRINT;
			}
			else {
				return END_PARSE_PRINT ;
			}
		}
		else {
			return END_PARSE_PRINT;
		}
	}
	else {
		return FALSE;
	}
}

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
int VariableDefinition() {
	PRINT_PARSE
	cg_item *x;
	int temp;
	int ret;
	if (TypeSymbol()) {
		if (VariableList()) {
			return TRUE | END_PARSE_PRINT ;
		}else{
			
			x = newItem();
			temp = FALSE;
			if ((expect(sym_array,1) && VariableName(x,1) && expect(sym_leftblock,1) && Constant(x,&temp) && expect(sym_rightblock,1))) {
				print("\tdefine array %s length %d\n",def_name,val);
				ret = FALSE;
				if (sym_type == sym_int) {
					ret = insert_var(proc,def_name,a_int,val);
				}
				else {
				 	if (sym_type == sym_bool) {
						ret = insert_var(proc,def_name,a_bool,val);
					}
				}
				if (ret == FALSE) {
					ERR_HANDLER(7);
				}
				if (val <= 0) {
					ERR_HANDLER(8);
				}
				closeItem(x,TRUE);
				return TRUE | END_PARSE_PRINT;
			}
			else {
				closeItem(x,TRUE);
				return END_PARSE_PRINT;
			}
		}
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/* 
 * TypeSymbol()
 *
 * Description: We expect a type of symbol integer or boolean
 *
 * Production rule: TypeSymbol = "integer" | "boolean"
 * Input: None
 * Output: Returns if the file parsed sucessfully
 */
int TypeSymbol() {
	PRINT_PARSE
	sym_type = token->sym_type;
	return (expect(sym_int,0) || expect(sym_bool,0)) | END_PARSE_PRINT ;
}

/*
 * VariableList()
 *
 * Description: A list of one or more variables is expected. Variables are seperated by commas
 *
 * Production rule: VariableName {, VariableName}
 * Input: None
 * Output: Returns if the function parsed sucessfully
 */
int VariableList() {
	PRINT_PARSE
	int ret;
	if (VariableName(NULL,0)) {
		
		ret = FALSE;
		if (sym_type == sym_int) {
			ret = insert_var(proc,def_name,t_int,0);
		}else if (sym_type == sym_bool) {
			ret = insert_var(proc,def_name,t_bool,0);
		}
		if (ret == FALSE) {
			ERR_HANDLER(9);
		}
		print("in level %d, procedure %s want to define variable %s of type %d (int is 9 and bool 2)\n",level,proc,def_name,sym_type);
		//when a comma is found look for the next variable
		while (expect(sym_comma,0)) {
			return VariableList() | END_PARSE_PRINT ;
		}
		//we parsed the whole list
		return TRUE | END_PARSE_PRINT ;
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/*
 * ProcedureDefinition()
 *
 * Description: This is the definition of a procedure
 *
 * Production rule: ProcedureDefinition = "proc" ProcedureName Block
 * Input: None
 * Output: Return is the function parsed sucessfully
 */
int ProcedureDefinition() {
	PRINT_PARSE
	
	if (expect(sym_proc,0) && ProcedureName(NULL,1)) {
	
		sprintf(proc,"%s",def_name);
		if (insert_proc(def_name,level+1) == FALSE) {
			ERR_HANDLER(10);
		}
		print("defining procedure %s at level %d\n",def_name,level);
		return FormalParameters() && Block() | END_PARSE_PRINT ;
	}
	else {
		return END_PARSE_PRINT;
	}
}

/*
 * FormalParameters()
 *
 * Description: If there is any formal parameters defined between two brackets. they will be defined from inside
 *
 * Production rule: FormalParameters = "(" [FPSection {; FPSection }\ ")"
 * Input: None
 * Output: Returns if the function parsed sucesfully
 */
int FormalParameters() {
	PRINT_PARSE
	if (expect(sym_leftbracket,0)) {
		if ( FPSection()) {
			while (expect(sym_semicolon,0)) {
				if (!(FPSection())) {
					ERR_HANDLER(30);
				}
			}
		}
		return (expect(sym_rightbracket,1)) | END_PARSE_PRINT;
	}
	else {
		return TRUE | END_PARSE_PRINT;
	}
}

/*
 * FPSection()
 *
 * Description: Formal parameters are defined in this procedure
 *
 * Production rule: FPSection = TypeSymbol ["value"] VariableName
 * Input: None
 * Output: Returns if the function parsed succesfully
 */
int FPSection() {
	PRINT_PARSE
	int isValue,ret;
	if (TypeSymbol()) {
		isValue = FALSE;
		if (expect(sym_value,0)) {
			isValue = TRUE;
		}
		print("Declaring a parameter\n");
		if (!(VariableName(NULL,1))) {
			return END_PARSE_PRINT;
		}
		if (sym_type == sym_int) {
			ret = insert_param(proc,def_name,t_int,0,level+1,isValue);
		}
		else {
			if (sym_type == sym_bool) {
				ret = insert_param(proc,def_name,t_bool,0,level+1,isValue);
			}
		}
		if (ret == FALSE) {
			ERR_HANDLER(7);
		}
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT;
}

/*
 * StatementPart()
 *
 * Description: This is a block of zero or more statements separated by semi-colons
 * Production rule: StatementPart = {Statement ";"}
 * Input: None
 * Output: returns if the function parsed sucesfully
 */ 
int StatementPart() {
	PRINT_PARSE
	if (Statement()) {
		//is the statement closed by a semi-colon?
		if (expect(sym_semicolon,1)) {
			//do the next Statement
			return StatementPart() | END_PARSE_PRINT ;
		}
		else {
			return END_PARSE_PRINT ;
		}
	}else {
		return TRUE | END_PARSE_PRINT ;
	}
}

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
int Statement() {
	PRINT_PARSE
 	int goback;
	goback = FALSE;
	return (ProcedureStatement(&goback) || 
		EmptyStatement() || 
		ReadStatement() || 
		WriteStatement() || 
		AssignmentStatement(goback) || 
		IfStatement() || 
		DoStatement()) | END_PARSE_PRINT ;
}

/* 
 * EmptyStatement()
 *
 * Description: An empty statement that does nothing
 *
 * Production rule: EmptyStatement = "skip"
 * Input: None
 * Output: Returns if the function parsed sucesfully
 */
int EmptyStatement() {
	PRINT_PARSE
	return expect(sym_skip,0) | END_PARSE_PRINT ;
}

/* 
 * ReadStatement()
 *
 * Description: A read statement reads input from the user
 *
 * Production rule: ReadStatement = "read" VariableAccessList
 * Input: None
 * Outpt: returns if the function parsed sucesfully
 */ 
int ReadStatement(){
	PRINT_PARSE
	varlist *temp;
	int r;
	clear_list();
	if (expect(sym_read,0) && VariableAccessList(FALSE)) {
		temp = list;
		while (temp != NULL) {
			if (temp->x->isArray) {
				print("is array\n");
				r = getReg();
				Put(F1_RD,r,temp->x->r,0);
				clearReg(r);
				temp->x->isArray = FALSE;
				clearReg(temp->x->r);
			}
			else {
				op1(F1_RD,temp->x);
			}
			temp = temp->next;
		}
		return TRUE | END_PARSE_PRINT ;
	}
	return END_PARSE_PRINT ;
}




/* 
 * VariableAccessList()
 *
 * Description: A list of variables that needs to be accessed. It must contain one or more variables separated by commas
 *
 * Production rule: VariableAccessList = VariableAccess { "," VariableAccess }
 * Input:If we need to goback a token if a sym_id was found, but not a variable
 * Output: Returns if the function parsed sucessfully
 */ 
int VariableAccessList(int goback) {
	PRINT_PARSE
	cg_item *x;
	varlist* temp;
	varlist* cur;
	x = newItem();
	
	if (VariableAccess(x,goback)) {
		temp = malloc(sizeof(struct varlist));
		temp->x = malloc(sizeof(struct cg_item));
		memcpy(temp->x,x,sizeof(struct cg_item));
		temp->next = NULL;
		
		if (list == NULL) {
			list = temp;
		}
		else {
			cur = list;
			while (cur->next != NULL) {
				cur = cur->next;
			}
			cur->next = temp;
		}
		x->mode = nomode;
		
		closeItem(x,FALSE);
		//is there another variable access?
		if (expect(sym_comma,0)) {
			return VariableAccessList(FALSE) | END_PARSE_PRINT ;
		}
		else {
			return TRUE | END_PARSE_PRINT ;
		}
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/*
 * WriteStatement()
 * 
 * Description: A write statement that writes output to the keyboard
 *
 * Production rule: WriteStatement = "write" ExpressionList
 * Input: None
 * Output: Returns if the function parsed sucessfully
 */
int WriteStatement() {
	PRINT_PARSE
	return (expect(sym_write,0) && ExpressionList(TRUE)) | END_PARSE_PRINT ;
}

/*
 * ExpressionList()
 *
 * Description: A list of one or more expressions separated by commas
 *
 * Production rule: ExpressionList = Expression {"," Expression }
 * Input: If we need to evaluate a variableAccessList
 * Output: Returns if we parsed the function succesfully
 */
int ExpressionList(int nolist) {
	PRINT_PARSE
	cg_item *x;
	varlist* temp;
	x = newItem();
	if (Expression(x)) {
		if (!nolist) {
			if (list == NULL) {
				ERR_HANDLER(16);
			}
			else {
				if (list->x->type == x->type) {
					temp = list;
					list = list->next;
					print("Stoor die antwoord\n");
					
					if (temp->x->mode == var) {
						if (temp->x->level == level) {
							temp->x->baseAddr = resetLevel();
						}
						else {
							temp->x->baseAddr = rootLevel();
						}
					}
					if (x->mode == var) {
						if (x->level == level) {
							x->baseAddr = resetLevel();
						}
						else {
							x->baseAddr = rootLevel();
						}
					}
					
					op2(F1_STW,temp->x,x);
					
					closeItem(temp->x,TRUE);
					
					closeItem(x,TRUE);
					free(temp);
				}else
				{
					ERR_HANDLER(17);
				}
			}
		}else {
			if (x->mode == var) {
				print("Mode = %d var level %d, func level %d\n",x->mode,x->level,level);
				if (x->level == level) {
					x->baseAddr = resetLevel();
				}
				else {
					x->baseAddr = rootLevel();
				}
			}
			print("type = %d\n",x->type);
			if (x->isArray) {
				print("\twant to write array\n");
				Put(F1_LDW,x->r,x->r,0);
			}
			op1(F3_WRD,x);
			closeItem(x,TRUE);
			clear_list();
		}
		//is there another expression?
		if (expect(sym_comma,0)) {
			return ExpressionList(nolist) | END_PARSE_PRINT ;
		}
		else {
			if (list != NULL) {
				ERR_HANDLER(18);
			}
			return TRUE | END_PARSE_PRINT ;
		}
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/* 
 * AsignmentStatement()
 *
 * Description:An assignment to variable
 *
 * Production rule: AssignmentStatement = VariableAccessList ":=" ExpressionList
 * Input: If we need to go back if an id was found but it wasn't a variable
 * Output: returns if the function parsed sucessfully
 */
int AssignmentStatement(int goback) {
	PRINT_PARSE
	clear_list();
	return (VariableAccessList(goback) && expect(sym_assign,1) && ExpressionList(FALSE)) | END_PARSE_PRINT ;
}

/*
 * ProcedureStatement()
 *
 * Description: A procedure that is called
 *
 * Production rule: ProcedureStatement = ProcedureName [ActualParameters]
 * Input: If we need to goback if an id was found that wasn't a procedure
 * Output: If the function parsed sucessfully
 */
int ProcedureStatement(int *goback) {
	PRINT_PARSE
	procedure_node *temp;
	call_node *call;
	temp = NULL;
	if (ProcedureName(NULL,0)){
		temp = lookup_proc(def_name,TRUE);
		if (temp == NULL) {
			*goback = TRUE;
			return END_PARSE_PRINT;
		}
		
		call = (call_node*)temp;
		print("\t\tSuccessful call to %s\n",def_name);
		sprintf(call_name,"%s",def_name);
		if (ActualParameters()) {
			pushParams(params);
			procCall(call);
			popParams(params);
			params = NULL;
			return TRUE | END_PARSE_PRINT;
		}
		return END_PARSE_PRINT;
	}else{
		return END_PARSE_PRINT;
	}
}


/*
 * ActualParameters()
 *
 * Description: The parameters to be passed to a called procedure
 *
 * Production rule: ActualParameters = "(" [Expression {"," Expression}] ")"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int ActualParameters()
{
	PRINT_PARSE
	var_node *temp;
	cg_item *x;
	params = NULL;
	if (expect(sym_leftbracket,0)) {
		x = newItem();
		
		if (Expression(x)) {
			params = newParam(x);
			temp = params;
			
			while (expect(sym_comma,0)) {
				if (!(Expression(x))) {
					ERR_HANDLER(31);
				}
				
				temp->next = newParam(x);
				temp = temp->next;
			}
		}
		checkParams(call_name,params);
		return expect(sym_rightbracket,1) | END_PARSE_PRINT;
	}
	checkParams(call_name,params);
	return TRUE | END_PARSE_PRINT;
}

/* 
 * IfStatement()
 *
 * Description: An if statement
 * Production rule: IfStatement = "if" GuardedCommandList "fi"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int IfStatement() {
	PRINT_PARSE
	cg_item *x;
	long L;
	x = newItem();
	
	if (expect(sym_if,0)) {
		elseIf = FALSE;
		if (GuardedCommandList(x,&L) && expect(sym_fi,1)) {
			//Fix the branch instruction
			L = FixLink(L);
			return TRUE | END_PARSE_PRINT ;
		}
	}
	return END_PARSE_PRINT ;
}

/* 
 * DoStatement()
 *
 * Description: A do loop statement
 * Production rule: DoStatement = "do" GuardedCommandList "od"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int DoStatement() {
	PRINT_PARSE
	cg_item *x;
	x = newItem();
	long L,temp;
	
	if (expect(sym_do,0)) {
		L = procPC();
		if (GuardedCommandList(x,&temp) && expect(sym_od,1)) {
			do_od = TRUE;
			elseIf = TRUE;
			print("Calling BJump\n");
			//set the branch back instruction
			BJump(L);
			//fixes the branch
			FixWith(temp,temp+1);
			//fixes the last branch
			
			FixLink(x->a);
			return TRUE | END_PARSE_PRINT;
		}
	}
	return END_PARSE_PRINT ;
}
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
int GuardedCommandList(cg_item *x, long *L) {
	PRINT_PARSE
	
	
	if (GuardedCommand(x,L)){
		elseIf = TRUE;
		do_od = FALSE;
		//does another guarded command follow?
		while (expect(sym_guard,0)){
			//set the forward jump
			if (do_od){
				*L = FJump(*L - 1);
			}
			else {
				*L = FJump(*L);
			}
			//verander waarnatoe ons branch op 'n false
			fix(x->a,x->a);
			
			if (!(GuardedCommand(x,L))) {
				return END_PARSE_PRINT ;
			}
		}
		
		return TRUE | END_PARSE_PRINT ;
	}
	else {
		return FALSE | END_PARSE_PRINT ;
	}
}

/* 
 * GuardedCommand()
 *
 * Description: This is a boolean expression followed by a statement to be executed if evaluated true
 *
 * Production rule: GuardedCommand = Expression "->" StatementPart
 * Input: A codegen item and the position of the first branch
 * Output: If the function parsed sucessfully
 */
int GuardedCommand(cg_item *x, long *L){
	PRINT_PARSE
	
	if (Expression(x))
	{
		if (x->type != t_bool)
		{
			ERR_HANDLER(27);
		}
		//set the conditional jump
		
		CJump(x);
		if (expect(sym_arrow,1) && StatementPart()) {
			//if it's an if statement set the first branch position to 0
			if (elseIf == FALSE) {
				*L = 0;
			}
			return TRUE | END_PARSE_PRINT;
		}
		return END_PARSE_PRINT ;
	}
	return END_PARSE_PRINT;	 
}

/* 
 * Expression()
 *
 * Description: A boolean expression that may be evaluated between a optional AND and OR
 *
 * Production rule: Expression = PrimaryExpression {PrimaryOperator PrimaryExpression}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Expression(cg_item *x){
	PRINT_PARSE
	cg_item *y;
	int opc;
	if (PrimaryExpression(x)) {
		y = malloc(sizeof(struct cg_item));
		memcpy(y,x,sizeof(struct cg_item));
		
		while (PrimaryOperator(&opc)) {
			
			op1(opc,x);
			
			
			if (PrimaryExpression(y))
			{
				if ((y->type == t_int) || (x->type == t_int)) {
					ERR_HANDLER(26);
				}
				if (y->type != x->type) {
					ERR_HANDLER(25);
				}
				if (x->mode == cons) {
					x->r = getReg();
					Put(F2_ADDI,x->r,0,x->c);
				}
				
				x->type = t_bool;
				op2(opc,x,y);
				x->r = y->r;
				//x->b = y->a;
				
			}
			else {
				return END_PARSE_PRINT;
			}
		}
		return TRUE | END_PARSE_PRINT ;
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/* 
 * PrimaryOperator()
 *
 * Description: Primary Operators that may be an AND or an OR
 *
 * Production rule: PrimaryOperator = "&" | "|"
 * Input: The integer the op type must be written to
 * Output: If the function parsed sucessfully
 */
int PrimaryOperator(int *op){
	PRINT_PARSE
	if (expect(sym_and,0)) {
		*op = F1_AND;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_or,0)) {
		*op = F1_OR;
		return TRUE | END_PARSE_PRINT;
	}
	return  END_PARSE_PRINT ;
}

/* 
 * PrimaryExpression()
 *
 * Description: Primary expressions may contain "RelationalOperator SimpleExpression" zero or one times
 *
 * Production rule: PrimaryExpression = SimpleExpression [RelationalOperator SimpleExpression]
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int PrimaryExpression(cg_item *x) {
	PRINT_PARSE
	cg_item *y;
	if (SimpleExpression(x)) {
		if (RelationalOperator()) {
			y = malloc(sizeof(struct cg_item));
			memcpy(y,x,sizeof(struct cg_item));
			if (SimpleExpression(y)) {
				if (y->type != x->type) {
					ERR_HANDLER(24);
					return END_PARSE_PRINT;
				}
				else {
					x->type = t_bool;
					relation(op,x,y);
					return TRUE  | END_PARSE_PRINT ;
				}
			}
			return END_PARSE_PRINT ;
		}
		else {
			return TRUE | END_PARSE_PRINT ;
		}
	}
	else {
		return END_PARSE_PRINT ;
	}	
}

/*
 * RelationalOperator()
 * 
 * Description: Relational Operators can be "<", "=" or ">"
 *
 * Production rule: RelationalOperator = "<" | "=" | ">"
 * Input: None
 * Output: If the function parsed sucessfully
 */
int RelationalOperator() {
	PRINT_PARSE
	if (expect(sym_less,0)) {
		op = F1_BLT;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_equal,0)) {
		op = F1_BEQ;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_greater,0)) {
		op = F1_BGT;
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

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
int SimpleExpression(cg_item *x){
	PRINT_PARSE
	cg_item *y;
	int add;
	//Does the term have a minus in front
	if (expect(sym_minus,0))
	{
		if (Term(x)) {
			if (x->type != t_int) {
				ERR_HANDLER(20);
			}
			op1(F3_UNARY,x);
		}
		else {
			return END_PARSE_PRINT ;
		}
	}
	else {
		if (!(Term(x))) {
			return END_PARSE_PRINT ;
		}
	}
	y = malloc(sizeof (struct cg_item));
	memcpy(y,x,sizeof (struct cg_item));
	y->isArray = FALSE;
	
	while (AddingOperator(&add)) {
		if (!(Term(y))) {
			return END_PARSE_PRINT ;
		}
		print("\t\t\tis Array %d\n",y->isArray);
		if ((x->type != t_int) || (y->type != t_int)) {
			ERR_HANDLER(21);
		}
		if ((x->isInt == FALSE) || (y->isInt == FALSE)) {
			if ((x->mode != cons) && (y->mode != cons)){
				if (add) {
					op2(F1_ADD,x,y);
				}
				else{
					op2(F1_SUB,x,y);
				}
			}
			else {
				if (add) {
					if (x->mode != cons) {
						op2(F2_ADDI,x,y);
					}
					else {
						op2(F2_ADDI,y,x);
						memcpy(x,y,sizeof(struct cg_item));
					}
				}
				else {
					if (x->mode != cons) {
						op2(F2_SUBI,x,y);
					}
					else {
						op2(F2_SUBI,y,x);
						memcpy(x,y,sizeof(struct cg_item));
						op1(F3_UNARY,x);
					}
				}
			}
			
			x->isInt = FALSE;
			y->isInt = FALSE;
			y->c = 0;
		}
		else {
			if (add) {
				x->c = y->c + x->c;
			}
			else {
				x->c = x->c - y->c;
			}
		}
	}
	y->c = x->c;
	return TRUE | END_PARSE_PRINT ;
}

/* 
 * AddingOperator()
 *
 * Description: The adding operators, + and -
 *
 * Production rule: AddingOperator = "+" or "-"
 * Input: An integer to return the type of operation
 * Output: If the function parsed sucessfully
 */
int AddingOperator(int * add) {
	PRINT_PARSE
	
	if (expect(sym_plus,0)) {
		*add = TRUE;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_minus,0)) {
		*add = FALSE;
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT;
}

/* 
 * Term()
 *
 * Description: This is factors that may be separated by multiplying operators
 *
 * Production rule: Term = Factor {MultiplyingOperator Term}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Term(cg_item *x){
	PRINT_PARSE
	cg_item *y;
	int mult;
	if (Factor(x)) {
		y = malloc(sizeof (struct cg_item));
		memcpy(y,x,sizeof (struct cg_item));
		
		while (MultiplyingOperator(&mult)) {
			if (Factor(y)) {
				if ((x->type != t_int) || (y->type != t_int)) {
					ERR_HANDLER(22);
				}
				if ((x->isInt == FALSE) || (y->isInt == FALSE)) {
					if ((x->mode != cons) && (y->mode != cons)) {
						switch (mult) {
							case 0:
								op2(F1_MUL,x,y);
								break;
							case 1:
								op2(F1_DIV,x,y);
								break;
							case 2:
								op2(F1_MOD,x,y);
								break;
							
						}
					
					}
					else {
						if (x->mode != cons) {
							switch (mult) {
								case 0:
									op2(F2_MULI,x,y);
									break;
								case 1:
									op2(F2_DIVI,x,y);
									break;
								case 2:
									op2(F2_MODI,x,y);
									break;
							}
						}
						else {
							switch (mult) {
								case 0:
									op2(F1_MUL,x,y);
									break;
								case 1:
									op2(F1_DIV,x,y);
									break;
								case 2:
									op2(F1_MOD,x,y);
									break;
							}
						}
					}
					
					x->isInt = FALSE;
					y->isInt = FALSE;
					x->c = 0;
				}
				else {
					if (mult == 0) {
						x->c = y->c * x->c;
					}
					if (mult == 1) {
						if (y->c == 0) {
							ERR_HANDLER(29);
						}
						x->c = x->c / y->c;
					}
					if (mult == 2) {
						if (x->c == 0) {
							ERR_HANDLER(29);
						}
						x->c = x->c % y->c;
					}
				}
				y->c = x->c;
			}else {
				return END_PARSE_PRINT ;
			}
		}
		return TRUE | END_PARSE_PRINT ;
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/*
 * MultiplyingOperator()
 *
 * Description: The multiplying operators
 *
 * Production rule: MultiplyingOperators = "*" | "/" | "\"
 * Input: An integer to set what type of operator we're using
 * Output: If the function parsed sucessfully
 */
int MultiplyingOperator(int *mult) {
	PRINT_PARSE	
	if (expect(sym_mult,0)) {
		*mult = 0;
		return TRUE | END_PARSE_PRINT;
	} 
	if (expect(sym_div,0)) {
		*mult = 1;
		return TRUE | END_PARSE_PRINT;
	}
	if (expect(sym_mod,0)) {
		*mult = 2;
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

/*
 * Factor()
 *
 * Description: A factor can be a constant, a variable, a expression or NOT a factor
 *
 * Production rule: Factor = Constant | VariableAccess | "(" Expression ")" | "~" Factor
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Factor(cg_item *x) {
	PRINT_PARSE
	int goback;
	goback = TRUE;
	if (Constant(x,&goback) || VariableAccess(x,goback) ||
		(expect(sym_leftbracket,0) && Expression(x) && expect(sym_rightbracket,1))) {
		return TRUE | END_PARSE_PRINT;
	}
	if ((expect(sym_not,0) && Factor(x))) {
		if (x->type != t_bool) {
			ERR_HANDLER(23);
		}
		op1(F3_NOT,x);
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

/*
 * VariableAccess()
 *
 * Description: Access to a variable
 *
 * Production rule: VariableAccess = VariableName [IndexedSelector]
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int VariableAccess(cg_item *x, int goback){
	PRINT_PARSE
	var_node* temp;
	if ((goback) || (VariableName(x,0))){
		temp = lookup_var(proc,def_name);
		print("Lookup needed for %s in procedure %s at level %d, val %d type %d\n",def_name,proc, level, temp->value, temp->type);
		if (temp == NULL) {
			ERR_HANDLER(12);
			return END_PARSE_PRINT;
		}
		else {
			x->c = 0;
			x->a = temp->disp;
			type = temp->type;
			x->mode = var;
			if (temp->value != 0) {
				if ((temp->type == c_bool) || (temp->type == c_int)) {
					ERR_HANDLER(15);
					return TRUE | END_PARSE_PRINT;
				}
				else {
					if (temp->level == level) {
						x->baseAddr = resetLevel();
					}
					else {
						x->baseAddr = rootLevel();
					}
					x->level = temp->level;
					return IndexedSelector(x) | END_PARSE_PRINT;
				}
			}
			else {
				x->type = type;
				if (temp->level == level) {
					x->baseAddr = resetLevel();
				}
				else {
					x->baseAddr = rootLevel();
				}
				x->level = temp->level;
				return TRUE | END_PARSE_PRINT;
			}
		}
	}
	else {
		return END_PARSE_PRINT ;
	}
}

/* 
 * IndexedSelector()
 *
 * Description:Used to index an array and work out the memory address of that index
 *
 * Production rule: IndexedSelector = "[" Expression "]"
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int IndexedSelector(cg_item *x){
	PRINT_PARSE
	var_node* temp;
	cg_item *y,*x2,*z,*z2;
	if (expect(sym_leftblock,1)) {
		temp = lookup_var(proc,def_name);
		if ((temp == NULL) || ((temp->type != a_int) && (temp->type != a_bool)) || (temp->value == 0 )) {
			ERR_HANDLER(13);
		}
		print("Lookup into array needed in procedure %s at level %d\n",proc,level);
		y = newItem();
		if (Expression(y) && expect(sym_rightblock,1)) {
			if (y->type != t_int) {
				ERR_HANDLER(19);
			}
			if ((y->c < 0) || (y->c >= temp->value)) {
				ERR_HANDLER(28);
			}
			x->isArray = TRUE;
			if (temp->type == a_int) {
				x->type = t_int;
			}
			else {
				x->type = t_bool;
			}
			/*
			y is the index in the array
			z is the size of the array
			x is the address of the array
			*/
			
			//get size of array
			z = newItem();
			z->mode = cons;
			z->c = temp->value;
			
			//load address of x
			x2 = newItem();
			x2->mode = cons;
			x2->c = x->a;
			x2->baseAddr = x->baseAddr;
			
			//size of int/boolean
			z2 = newItem();
			z2->mode = cons;
			z2->c = 4;
			
			if (y->isArray) {
				Put(F1_LDW,y->r,y->r,0);
				y->isArray = FALSE;
			}
			else {
				load(y);
			}
			
			//check bounds
			op2(F2_CHKI,y,z);
			
			load(x2);
			
			//index * size
			op2(F2_MULI,y,z2);
			closeItem(z2,TRUE);
			
			//get index's offset
			Put(F1_ADD,x2->r,x2->r,y->r);
			
			//save into register the value at that index
			if (x->mode != reg) {
				x->r = getReg();
			}
			
			Put(F2_DIVI,x2->r,x2->r,4);
			Put(F1_ADD,x->r,x->baseAddr,x2->r);
		
			closeItem(x2,TRUE);
			closeItem(y,TRUE);
			closeItem(z,TRUE);
			x->mode = reg;
			return TRUE | END_PARSE_PRINT;
		}
		return END_PARSE_PRINT ;
	}else{
		return END_PARSE_PRINT;
	}
}

/*
 * Constant()
 *
 * Description: Can be a number or a defined constant
 *
 * Production rule: Constant = Numeral | ConstantName
 * Input: A codegen item and if we need to go back if an id was found that isn't a constant
 * Output: If the function parsed sucessfully
 */
int Constant(cg_item *x, int *goback)
{
	PRINT_PARSE
	var_node* temp;
	if (Numeral(x)) {
		x->type = t_int;
		x->c = val;
		type = c_int;
		x->mode = cons;
		return TRUE | END_PARSE_PRINT; 
	}
	else {
		if (ConstantName(x,0)) {
			temp = lookup_var(proc,const_name);
			if ((temp == NULL) || ((temp->type != c_bool) && (temp->type != c_int))) {
				if (*goback == FALSE) {
					ERR_HANDLER(14);
				}
				print("%s not a constant goback = %d\n", const_name, *goback);
				sprintf(def_name,"%s",const_name);
				return END_PARSE_PRINT;
			}
			else {
				x->mode = cons;
				x->c = temp->value;
				x->a = temp->disp;
				if (temp->type == c_int) {
					type = c_int;
					x->type = t_int;
				}
				else {
					type = c_bool;
					x->type = t_bool;
				}
				val = temp->value;
				print("Constant %s defined as %d\n",const_name,val);
				return TRUE | END_PARSE_PRINT;
			}
		}
	}
	*goback = FALSE;
	return END_PARSE_PRINT;
}

/*
 * Numeral()
 *
 * Description: A number is expected
 *
 * Production rule: Numeral = Digit {Digit}
 * Input: A codegen item
 * Output: If the function parsed sucessfully
 */
int Numeral(cg_item *x) {
	PRINT_PARSE
	val = token->val;
	if (expect(sym_number,0)) {
		x->isInt = TRUE;
		x->mode = cons;
		return TRUE | END_PARSE_PRINT;
	}
	return END_PARSE_PRINT ;
}

/*
 * Name() 
 *
 * Description: A name is expected
 *
 * production rule: Name = Letter {Letter | Digit}
 * Input: A codegen item and the priority we're expecting on the name
 * Output: If the function parsed sucessfully
 */
int Name(cg_item *x,int priority) {
	PRINT_PARSE
	if (expect(sym_id,priority)) {
		if (x != NULL) {
			x->isInt = FALSE;
			x->mode = var;
		}
		return TRUE | END_PARSE_PRINT;
	}
	else {
		return	END_PARSE_PRINT;
	}
}



