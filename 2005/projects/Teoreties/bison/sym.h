#ifndef SYM_H
#define SYM_H
typedef struct node{
	char* name;
	double val;
	struct node* next;
}node;

node* sym_look(char*name);


#endif
