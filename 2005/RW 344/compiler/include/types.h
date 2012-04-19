#ifndef TYPES_H
#define TYPES_H

#include <math.h>
#define MAX_LEN 32
#define MAX_INT (pow(2,31)-1)
//2147893647
#define TRUE 1
#define FALSE 0
#define KEYWORDS 16
#define MAX_LEN 32

enum modes
{
	nomode = 0,
	var,
	cons,
	reg,
	cond	//<--- added condition type
};

enum types
{
	no_type=0,
	t_int,
	t_bool,
	a_int,
	a_bool,
	c_int,
	c_bool
};

typedef struct cg_item{
	enum modes mode;   //var, const, cond or reg
	enum types type;    //tipe van entiteit
	long a, r, c, b;   //memory or register or constant
	int isInt,isArray,varIndex;
	int level;
	unsigned long baseAddr;
} cg_item;

typedef struct varlist
{
	cg_item *x;
	//int val;
	struct varlist* next;
}varlist;

typedef struct var_node{
  char name[MAX_LEN+1]; 
  enum types type;// constant or boolean
  int value,disp,level,isValue; 
  cg_item * x;
  struct procedure_node *parent;
  struct var_node * next ,*prev;    
}var_node;

typedef struct array_node{
  int value;
  struct array_node * next;  
}node;

typedef struct call_node{
  unsigned long pc;
  struct var_node *params;
  char proc_name[MAX_LEN+1];
  struct call_node * next;  
}call_node;

typedef struct procedure_node{
  unsigned long pc;
  struct var_node * params;
  char name[32];
  int level,disp,paramDisp;  
  struct procedure_node * next;
  struct procedure_node * prev;
  struct array_node * array_node;
  struct var_node * var_list;  
  struct call_node * call_list, *call;
}procedure_node;

#endif
