/****************************************************************
 * 	scanner.c						*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "include/types.h"
#include "include/scanner.h"
#include "include/error.h"

static char sym;

static FILE* fptr;
static token_str*  token;
char* KeyWordTable[KEYWORDS];

/*
 * readChar()
 *
 * Description :  This functions main aim is solely to retrieve the next character in the file, store it into sym and wait to
 * retrieve the next symbol. Terminates if file ptr is found to be null. Will also pick up the end of file.
 *
 * Input : NONE
 * Output : NONE
 *
*/

void readChar() {

	if (fptr!= NULL) {
		fscanf(fptr,"%c",&sym);
		inc_col();
		if (sym == '\n') {
			inc_line();
		}
		if (feof(fptr)!= 0) {
			sym = '\0';
		}
	}
	else {
		ERR_HANDLER(1);
	}
}


/*
 * binarySearch()
 *
 * Description : Implements the binary search algoritm for retrieving an the index of a keyword in the keyword table.
 *
 * Input : int first = first element in the array
 *	  int last = last element in the array
 *	  char name[] = keyword being searched for
 * Output : an integer with the offset value of the keyword that was looked for
 *
*/

int binarySearch(int first, int last, char name[MAX_LEN]) {
	
	int pivot;
	int found;
	int result;
	int cmpResult;
	
	pivot = 0;
	found = FALSE;
	result = -1;
	
	while ((first <= last) && (found == FALSE)) {
		pivot = (first+last)/2;
		cmpResult = strcmp(KeyWordTable[pivot],name);
		if(cmpResult == 0) {
			found = TRUE;
			result = pivot;
		}
		else if (cmpResult == 1) {
			last = pivot-1;
		}
		else {
			first = pivot + 1;
		}
		
	}
	return result;
}



/*
 * isKeyWord()
 *
 * Description : This function recieves a word (token->name) and checks if that word belongs to a table of Keywords by
 * searching through an array using binary search. If a keyword is found then sym_type is set as sym_keyword (of the 
 * specific keyword. ie. sym_begin) but if not then it is left as sym_id, an identifier.
 *
 * Input : char name[] = keyword looked for
 * Output : NONE
 *
*/

void isKeyWord(char name[MAX_LEN]) {

	int i;
	
	token->sym_type = sym_id;
	i = binarySearch(0,KEYWORDS-1,name);
	if (i != -1) {
		token->sym_type = i;
	}
}

/*
 * readComment()
 *
 * Description : Called when a comment is found. Called after "(*" has already been scanned and continues to
 * handle comments, including nested comments, recursively
 *
 * Input : NONE
 * Output : NONE
 *
*/

void readComment() {

	while (!(feof(fptr))) {
		readChar();
		while ((sym != '*') && (sym != '(') && (!(feof(fptr)))) {
			readChar();
		}
		if (sym == '*') {
			readChar();
			if (sym == ')')
				return ;
		}
		else if (sym == '(') {
			readChar();
			if (sym == '*') {
				readComment();
			}
		}
	}
	ERR_HANDLER(3);
}

/*
 * readWord()
 *
 * Description : This function is called when any letter is found. The word is scanned completely, char for char. The whole
 * word is then tested with isKeyWord.
 *
 * Input : NONE
 * Output : Integer 0 for success and -1 for error
 *
*/

int readWord() {

	int index;
	index = 0;
	
	while (isalnum(sym) && (index < 32)) {
		token->name[index] = sym;
		index++;
		readChar();
	}
	if (isalnum(sym) && (index == 32)) {
		ERR_HANDLER(2);
		return -1;
	}
	else {
		token->name[index] = 0;
	}
	isKeyWord(token->name);
	return 0;
}

/*
 * readNumber()
 *
 * Description : This function simply processes a number, as well as checking if the number is in range. If found not to
 * it is scanned over until the first non number is found and then disregarded.
 * 
 * Input : NONE
 * Output : NONE
 *
*/

void readNumber() {

	 token->val = 0;
	 token->sym_type = sym_number;

	 while (isdigit(sym)) {
	 	if (token->val <= ((MAX_INT - atoi(&sym) )/10)) {
			token->val = token->val * 10 + atoi(&sym);
			readChar(); 
		}
		else {
			ERR_HANDLER(4);
			token->val = 0;
			readChar(); 
			while ((sym >= '0') && (sym <= '9') ) {
				readChar(); 
			}
		}
	 }
}

/*
 * getToken()
 *
 * Description : This function may be considered as the main function of the scanner as all the tokens are processed here and
 * readChar is used to get the next char in the file to then once again process a token.
 *
 * Input : NONE
 * Ouput : Pointer of type 'token'
 *
*/

token_str* getToken() {
	
	while (((sym >= 9) && (sym <= 13)) || (sym == ' ')) {
		readChar();
	}	
	if (feof(fptr) != 0) {
		token->sym_type = sym_eof;
		return token;
	}
	if (isalpha(sym)) {
		readWord();
		return token;
	}	
	if (isdigit(sym)) {
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
			if (sym == '*') {
				readComment();
				readChar();
				token = getToken();
			}
			else {
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
			}
			else {
				token->sym_type = sym_minus;
			}
			break;
	case ':':
			readChar();
			if (sym == '=') {
				token->sym_type = sym_assign;
				readChar();
			}
			break;
	case '[':
			readChar();
			if (sym == ']') {
				token->sym_type = sym_guard;
				readChar();			
			}
			else {
				token->sym_type = sym_leftblock;
			}
			break;
	default : 
			ERR_HANDLER(0);
			break;
	}
	return token;
}

/*
 * init()
 *
 * Description : Opens the source file and dynamically creates space for the token_struct.
 *
 * Input : The name of the file
 * Output : The file ptr of the open file
 *
*/

FILE* init(char* f) {

	token = NULL;
	fptr = fopen(f,"r");
	
	if (fptr == NULL) {
		ERR_HANDLER(1);
		return NULL;
	}
	else {
		token = malloc(sizeof(struct token_str));
 		readChar();
		KeyWordTable[0] = "array";
		KeyWordTable[1] = "begin";
		KeyWordTable[2] = "boolean";
		KeyWordTable[3] = "const";
		KeyWordTable[4] = "do";
		KeyWordTable[5] = "end";
		KeyWordTable[6] = "fi";
		KeyWordTable[7] = "if";
		KeyWordTable[8] = "integer";
		KeyWordTable[9] = "od";
		KeyWordTable[10] = "proc";
		KeyWordTable[11] = "read";
		KeyWordTable[12] = "skip";
		KeyWordTable[13] = "value";
		KeyWordTable[14] = "write";
		return fptr;
	}
}

/*
 * close()
 *
 * Description : Frees all memory allocated with the malloc statement
 *
 * Input : NONE
 * Output : NONE
 *
*/

void close() {

	free(token);
	
	if (fptr != NULL) {
		fclose(fptr);
	}
}
