#ifndef SCANNER_H
#define SCANNER_H

#define MAX_LEN 32
#define MAX_INT 2147893648
#define TRUE 1
#define FALSE 0

#define ASCII_A 65
#define ASCII_Z 90
#define ASCII_a 97
#define ASCII_z 122

enum symtype  {
	sym_eof = -1,
	sym_array,
	sym_begin,
	sym_bool,
	sym_call,
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
	sym_intdiv,
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
  Reads a character from the file into sym
  	INPUT : none
	OUTPUT : none
*/
void readChar();

/*
  Skips all characters enclosed in comments.
  	INPUT : none
  	OUTPUT : Integer that indicates an error
*/
void readComment();

/*
  Reads a word from the source file and decides whether it is a variable or a keyword
  	INPUT : none
	OUTPUT : Integer that indicates an error
*/
int readWord();

/*
  Read a number
  	INPUT :
	OUTPUT :
*/
int readNumber();

/*
  get the next token from the file
  	INPUT :
	OUTPUT :
*/
token_str* getToken();


FILE* init(char* f);

#endif

