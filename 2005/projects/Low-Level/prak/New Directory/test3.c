#include <stdio.h>
#include <stdlib.h>
#include "prak3.h"

node *create_node(int key) {
  node *x;
  x = malloc(sizeof(node));
  x->key = key;
  x->next = NULL;
  return x;
}

void enumerate(node *first) {
  while (first != NULL) {
    printf("key = %d\n", first->key);
    first = first->next;
  }
}

int main(int argc, char *argv[]) {
  node *list;
  
  list = NULL;
  
  insert(&list, create_node(10));
  insert(&list, create_node(12)); 
  insert(&list, create_node(4));
  insert(&list, create_node(2));
  insert(&list, create_node(17));
  insert(&list, create_node(13)); 
  enumerate(list);
  
  return 0;
}
