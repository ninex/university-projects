#ifndef SYM_H
#define SYM_H

typedef struct sym_node{
	char * name;
	double value;
	struct sym_node *next ;
}sym_node;

sym_node * symlook(char *name);
sym_node *insert_node(char *name);
void free_node(char *name);
#endif
