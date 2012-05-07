#ifndef TUT3_H
#define TUT3_H

typedef struct item {
  int  number;
  char name[32];
} item;

typedef struct node {
  char name[32];
  struct node *left;
  struct node *right;
} node;


/*
 *       INPUT: item *list - pointer to an array of items
 *              int n - total number of elements in the array
 *      OUTPUT: None
 * DESCRIPTION: Performs a binary sort on the items pointed to by list
 */
extern void binary_sort(item *list, int n);
/*
 *       INPUT: node **root - reference parameter to the root node
 *                            of a binary tree
 *              char *name - name field of the node that must be deleted
 *      OUTPUT: None
 * DESCRIPTION: Locates a node whose 'name' field matches the name
 *              specified as a parameter. If the node exits, it is
 *              deleted and the tree is reorganized (if necessary).
 *              The memory occupied by the node is also released
 *              using the 'free' function
 */
extern void delete_node(node **root, char *name);
/*
 *       INPUT: int n - any legal signed integer value
 *              char *s - pointer to a character string. The
 *                        string must be able to hold 33 characters
 *      OUTPUT: None
 * DESCRIPTION: Converts the integer value to NULL terminated character
 *              string containing the binary representation of the
 *              the value specified by 'n'
 */
extern void bin_to_string(int n, char *s);

#endif
