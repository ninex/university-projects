/****************************************************************
 * 	error.h							*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/
#ifndef ERROR_H
#define ERROR_H

/*
 * ERR_HANDLER()
 *
 * Description : Error handling function - used to explain errors but also to keep all printed
 * errors in one place.
 *
 * Input : An integer representing the error encountered
 * Output : NONE
 *
*/

void ERR_HANDLER(int error);

/*
 * set_count()
 *
 * Description : Initialises the line and column numbers.
 *
 * Input : NONE
 * Output : NONE
 *
*/

void set_count();

/*
 * inc_line()
 *
 * Description : Increments the line number.
 *
 * Input : NONE
 * Output : NONE
 *
*/

void inc_line();

/*
 * inc_col()
 *
 * Description : Increments the column number.
 *
 * Input : NONE
 * Output : NONE
 *
*/

void inc_col();

/*
 * set_error()
 *
 * Description : 
 *
 * Input :
 * Output :
 *
*/

char* set_error(enum symtype sym_type);

#endif
