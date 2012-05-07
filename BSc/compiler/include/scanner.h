/****************************************************************
 * 	scanner.h						*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/
#ifndef SCANNER_H
#define SCANNER_H

enum symtype  {
	sym_eof = -1,
	sym_array,
	sym_begin,
	sym_bool,
	//sym_call,
	sym_const,
	sym_do,
	sym_end,
	sym_fi,
	sym_if,
	sym_int,
	sym_od,
	sym_proc,
	sym_read,
	sym_skip,
	sym_value,
	sym_write,
	
	sym_id,
	sym_number,
	sym_comma,
	sym_semicolon,
	sym_colon,
	sym_equal,
	sym_leftblock,
	sym_rightblock,
	sym_arrow,
	sym_and,
	sym_or,
	sym_less,
	sym_assign,
	sym_greater,
	sym_plus,
	sym_minus,
	sym_guard,
	sym_mult,
	sym_div,
	sym_mod,
	sym_not,
	sym_leftbracket,
	sym_rightbracket,
	sym_stop,
	sym_eol,
	sym_space
};


typedef struct token_str{
	char name[MAX_LEN];
	enum symtype sym_type;
	unsigned int val;
} token_str;

 
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

token_str* getToken();

/*
 * init()
 *
 * Description : Opens the source file and dynamically creates space for the token_struct.
 *
 * Input : The name of the file
 * Output : The file ptr of the open file
 *
*/

FILE* init(char* f);

/*
 * close()
 *
 * Description : Frees all memory allocated with the malloc statement
 *
 * Input : NONE
 * Output : NONE
 *
*/

void close();

#endif

