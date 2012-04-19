#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdio.h>

typedef struct var_node{
  char * name; 
  char type;// constant or boolean
  int value; 
  struct array_node * array_root;
  struct var_node * next;    
}var_node;

typedef struct array_node{
  int value;
  struct array_node * next;  
}node;


typedef struct procedure_node{
  char * name;
  int level;
  struct procedure_node * next;
  struct procedure_node * prev;
  struct array_node * array_node;
  struct var_node * var_list;
}procedure_node;


void init_table();
struct procedure_node * lookup_proc(char * proc_name);
struct var_node * lookup_var(char * proc_name, char * var_name);
void insert_proc(char * name);
struct procedure_node * findPlace();
void insert_var(char * proc_name, char * var_name, char type,int value);
void remove_proc();
void printSymTable();
void printProcVars(char * proc_name);
//void print_tree();

#endif

