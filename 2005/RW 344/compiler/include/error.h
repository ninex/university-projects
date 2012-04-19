#ifndef ERROR_H
#define ERROR_H

void ERR_HANDLER(int error);

void set_count();

void inc_line();

void inc_col();

char* set_error(enum symtype sym_type);

#endif
