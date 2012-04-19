/********************************************************
*	Scanner.c					*
*							*
*	Date : 						*
*							*
*       Part of Software design: Compiler project (2005)*
*	Developed by : Eddie, Abrie, Mia & Andrew	*
*							*
********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include/types.h"
#include "include/scanner.h"
#include "include/error.h"

static char sym;

static FILE* fptr;
static token_str*  token;
char* KeyWordTable[KEYWORDS];

/********************************************************
* This functions main aim is solely to retrieve the next*
* character in the file, store it into sym and wait to	*
* retrieve the next symbol. terminates if file ptr is 	*
* found to be null. will also pick up the end of file	*
********************************************************/
void readChar(){
	if (fptr!= NULL){
		fscanf(fptr,"%c",&sym);
		inc_col();
		if (sym == '\n') {
			inc_line();
		}
		if (feof(fptr)!= 0){
			sym = '\0';
		}
	}else
	{
		ERR_HANDLER(1);
	}
}

/********************************************************
* This function recieves a word (token->name) and 	*
* checks if that word belongs to a table of Keywords by *
* searching through an array using string compare. If a *
* keyword is found then sym_type is set as sym_keyword  *
* (of the specific keyword. ie. sym_begin) but if not 	*
* then it is left as sym_id, an identifier.		*
********************************************************/
void isKeyWord(char name[MAX_LEN]){
	int i;
	token->sym_type = sym_id;
	
	for(i=0; i < KEYWORDS; i++){
		if (strcmp(name,KeyWordTable[i]) == 0){
			token->sym_type = i;
		}
	}
}

/********************************************************
*This funcion is called initially when a comment is 	*
*found. We enter this function already have scanned 	*
* "(*" and then continues to handle comments completely *
*including nested comments, recursively			*
********************************************************/
void readComment(){

	while (!(feof(fptr))) {
		readChar();
		while ((sym != '*') && (sym != '(') && (!(feof(fptr)))) {
			readChar();
		}
		if (sym == '*'){
			readChar();
			if (sym == ')')
				return ;
		}else if (sym == '('){
			readChar();
			if (sym == '*'){
				readComment();
			}
		}
	}
	ERR_HANDLER(3);
}

/********************************************************
* This function is called when any letter is found. The	*
* word is scanned comlpetely, char for char. The whole  *
* word is then tested against isKeyWord.	        *
********************************************************/
int readWord(){
	int index = 0;
	
	while (isalnum(sym) && (index < 32)){
		token->name[index] = sym;
		index++;
		readChar();
	}
	if (isalnum(sym) && (index == 32)){
		ERR_HANDLER(2);
		return -1;
	}else{
		token->name[index] = 0;
	}
	isKeyWord(token->name);
	return 0;
}

/********************************************************
*This function simply processes a number, as well as 	*
*checking if the number is in range and if found not to	*
*then the number is scanned over until the first non 	*
*number is found and then disregarded. There is conflict*
*here again where if an error is found, the scanner is 	*
*terminated and so makes some code redundent. 		*
********************************************************/
void readNumber(){
	 token->val = 0;
	 token->sym_type = sym_number;
	 while (isdigit(sym)){
	 	if (token->val <= ((MAX_INT - atoi(&sym) )/10)) {
			token->val = token->val * 10 + atoi(&sym);
			readChar(); 
		}
		else
		{
			ERR_HANDLER(4);
			token->val = 0;
			readChar(); 
			while ((sym >= '0') && (sym <= '9') ){
				readChar(); 
			}
		}
	 }
}

/********************************************************
*This function may be considered as the main function of*
*the scanner as all the tokens are processed here and 	*
*readChar is used to get the next char in the file to 	*
*then once again process a token. All cases should be   *
*covered by now.					*
********************************************************/
token_str* getToken(){
	
	while (((sym >= 9) && (sym <= 13)) || (sym == ' ')){
		readChar();
	}	
	if (feof(fptr) != 0){		
		token->sym_type = sym_eof;
		return token;
	}
	if (isalpha(sym)){
		readWord();
		return token;
	}	
	if (isdigit(sym)){
 		readNumber();
 		return token;
 	}
	switch(sym) {
	case '.':    
			token->sym_type = sym_stop;
			readChar();
			break;
	case ',':
			token->sym_type = sym_comma;
			readChar();
			break;
	case ';':
		       	token->sym_type = sym_semicolon;
			readChar();
			break;
	case '=':
			token->sym_type = sym_equal;
			readChar();
			break;
	case '&':
			token->sym_type = sym_and;
			readChar();
			break;
	case '|':
			token->sym_type = sym_or;
			readChar();
			break;
	case '<':
			token->sym_type = sym_less;
			readChar();
			break;
	case '>':
			token->sym_type = sym_greater;
			readChar();
			break;
	case '*':
			token->sym_type = sym_mult;
			readChar();
			break;
	case '+':
			token->sym_type = sym_plus;
			readChar();
			break;
	case '/':
			token->sym_type = sym_div;
			readChar();
			break;
	case '\\':
			token->sym_type = sym_mod;
			readChar();
			break;
	case '~':
			token->sym_type = sym_not;
			readChar();
			break;
	case '(':
			readChar();
			if (sym == '*'){
				readComment();
				readChar();
				token = getToken();
			}else{
				token->sym_type = sym_leftbracket;
			}
			break;
	case ')':
			token->sym_type = sym_rightbracket;
			readChar();
			break;
	case ']':
			token->sym_type = sym_rightblock;
			readChar();
			break;
	case '-':
			readChar();
			if (sym == '>')	{
				token->sym_type = sym_arrow;
				readChar();
			}else{
				token->sym_type = sym_minus;
			}
			break;
	case ':':
			readChar();
			if (sym == '='){
				token->sym_type = sym_assign;
				readChar();
			}
			break;
	case '[':
			readChar();
			if (sym == ']'){
				token->sym_type = sym_guard;
				readChar();			
			}else{
				token->sym_type = sym_leftblock;
			}	
			break;
	default : 
			ERR_HANDLER(0);
			break;
	}
	return token;
}
/********************************************************
* Opens the source file and dynamically creates space	*
* for for the token_struct. returns the file ptr of the	*
* open file. Will most probably be used to initialize	*
* the symbol table					*
********************************************************/
FILE* init(char* f) {
	token = NULL;
	fptr = fopen(f,"r");
	if (fptr == NULL){
		ERR_HANDLER(1);
		return NULL;
	}
	else{
		token = malloc(sizeof(struct token_str));
 		readChar();
		KeyWordTable[0] = "array";
		KeyWordTable[1] = "begin";
		KeyWordTable[2] = "boolean";
		KeyWordTable[3] = "call";
		KeyWordTable[4] = "const";
		KeyWordTable[5] = "do";
		KeyWordTable[6] = "end";
		KeyWordTable[7] = "fi";
		KeyWordTable[8] = "if";
		KeyWordTable[9] = "integer";
		KeyWordTable[10] = "od";
		KeyWordTable[11] = "proc";
		KeyWordTable[12] = "read";
		KeyWordTable[13] = "skip";
		KeyWordTable[14] = "write";
		KeyWordTable[15] = "value";
		
		return fptr;
	}
}

/********************************************************
* Frees all memory allocates with the malloc statement  *
********************************************************/
void close() {
	free(token);
	if (fptr != NULL)
	{
		fclose(fptr);
	}
}
