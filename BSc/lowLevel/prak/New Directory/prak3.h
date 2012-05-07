#ifndef PRAK3_H
#define PRAK3_H

typedef struct node {
  int key;
  struct node *next;
} node;

extern void insert(node **first, node *n);

#endif
