#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tut3.h"

#define MAX_STUDENTS_A 1
#define MAX_STUDENTS_B 2
#define MAX_STUDENTS_C 14
#define MAX_KOSIE 100
#define MAX_BIN_TO_STRING_VALUES 10

void create_student(item *p, char *name, int number) {
  if (p != NULL) {
    strcpy(p->name, name);
    p->number = number;
  }
}

void display_students(item list[], int n) {
  int i;
  i = 0;
  while (i < n) {
    printf("%d, %s\n", list[i].number, list[i].name); 
    i = i+1;
  }
}  

void binary_sort_c(item *list, int n) {
  int i;
  int j;
  int bottom;
  int top;
  int middle;
  item temp;
  
  if (list != NULL) {  
    i = 1;
    while (i < n) {
      temp = list[i];
      bottom = 0;
      top = i-1;
      while (bottom <= top) {
        middle = (bottom+top)/2;
        if (temp.number < list[middle].number)
          top = middle-1;
        else
          bottom = middle+1;
      }
      j = i-1;
      while (j >= bottom) {
        list[j+1] = list[j];
        j = j-1;
      }
      list[bottom] = temp;
      i = i+1;
    }
  }
}


void enumerate(const node *root) {
  if (root != NULL) {
    enumerate(root->left);
    printf("%s\n", root->name);
    enumerate(root->right);
  }
}

void insert_node(node **root, node *n) {
  if (*root == NULL)
    *root = n;
  else {
    if (strcmp(n->name, (*root)->name) > 0)
      insert_node(&(*root)->right, n);
    else
      if (strcmp(n->name, (*root)->name) < 0)
        insert_node(&(*root)->left, n);
  }
}

int equal(node *r1, node *r2) {
  if ((r1 == NULL) && (r2 == NULL))
    return 1;
  else
    return ((r1 != NULL) && (r2 != NULL) && (strcmp(r1->name, r2->name) == 0) &&
            equal(r1->right, r2->right) && equal(r1->left, r2->left));
}

node* create_node(char *name) {
  node* n;
  n = malloc(sizeof(node));
  strcpy(n->name, name);
  n->left = NULL;
  n->right = NULL;
  return n;
}

void remove_max_c(node **root, node **max) {
  if ((*root)->right != NULL)
    remove_max_c(&(*root)->right, max);
  else {
    *max = *root;
    *root = (*max)->left;
  }
}

void delete_node_c(node **root, char *name) {
  node *n;
  node *temp;
  
  if (*root != NULL) {
    if (strcmp(name, (*root)->name) < 0)
      delete_node_c(&(*root)->left, name);
    else {
      if (strcmp(name, (*root)->name) > 0)
        delete_node_c(&(*root)->right, name);
      else {
        temp = *root;
        if ((*root)->left == NULL) {
	  *root = (*root)->right;
	}
	else {
	  if ((*root)->right == NULL) {
	    *root = (*root)->left;
	  }
	  else {
	    remove_max_c(&(*root)->left, &n);
	    n->left = (*root)->left;
	    n->right = (*root)->right;
	    *root = n;
	  }
	}
	free(temp);
      }
    }
  }
}

void bin_to_string_c(int n, char *s) {
  unsigned x;
  int i;
  
  x = 0x80000000;
  i = 0;
  while (i < 32) {
    if ((n & x) != 0)
      s[i] = '1';
    else
      s[i] = '0';
    i = i+1;
    x = x >> 1;
  }
  s[i] = '\0';
}

void evaluate_binary_sort(char *msg, item *l1, item *l2, int n) {
  printf("evaluating binary sort for %s\n", msg);
  binary_sort(l1, n);
  binary_sort_c(l2, n);
  if (memcmp(l1, l2, n*sizeof(item)) == 0)
    printf("correct\n");
  else {
    printf("incorrect, the list contains:\n");
    display_students(l1, n);
    printf("and should contain:\n");
    display_students(l2, n);
  }
}

void evaluate_delete_node(node *root_1, node *root_2, char *name) {
  delete_node_c(&root_1, name);
  delete_node(&root_2, name);
  
  if (equal(root_1, root_2) == 1)
    printf("delete_node correct\n");
  else {
    printf("delete_node incorrect! - binary tree");
    if (root_2 == NULL)
      printf(" is NULL\n");
    else {
      printf(" contains\n");
      enumerate(root_2);
    }
    if (root_1 == NULL)
      printf("and should be NULL\n");
    else {
      printf("and should contain\n");
      enumerate(root_1);
    }
  }
}

void evaluate_bin_to_string(int n) {
  char s1[33];
  char s2[33];
  
  bin_to_string_c(n, s1);
  bin_to_string(n, s2);
  
  printf("bin_to_string(%d) returned '%s'", n, s2);
  if (strcmp(s1, s2) == 0)
    printf(" - correct\n");
  else
    printf(" - incorrect, should return '%s'\n", s1);
}

int main(int argc, char *argv[]) {
  item students_a[MAX_STUDENTS_A];
  item students_a1[MAX_STUDENTS_A];
  item students_b[MAX_STUDENTS_B];
  item students_b1[MAX_STUDENTS_B];
  item students_c[MAX_STUDENTS_C];
  item students_c1[MAX_STUDENTS_C];
  
  int x[MAX_BIN_TO_STRING_VALUES];

  int i;
  
  node* root_a;
  node* root_a1;
  node* root_b;
  node* root_b1;
  node* root_c;
  node* root_c1;
  node* root_d;
  node* root_d1;
  node* root_e;
  node* root_e1;
  node* root_f;
  node* root_f1;
  node* root_g;
  node* root_g1;
  node* root_h;
  node* root_h1;
  
  /* Binary sort test cases */  
  create_student(&students_a[0], "Kosie", 1387689);
  memcpy(students_a1, students_a, sizeof(item)*MAX_STUDENTS_A);
  
  create_student(&students_b[0], "Sarel", 1389735);
  create_student(&students_b[1], "Caroline", 1382024);
  memcpy(students_b1, students_b, sizeof(item)*MAX_STUDENTS_B);
  
  create_student(&students_c[0], "Kosie", 1387689);
  create_student(&students_c[1], "Sarel", 1389735);
  create_student(&students_c[2], "Gertjan", 1392755);
  create_student(&students_c[3], "Pietie", 1357987);
  create_student(&students_c[4], "Siphwe", 1384426); 
  create_student(&students_c[5], "Rickus", 1387551);
  create_student(&students_c[6], "Samantha", 1389699);
  create_student(&students_c[7], "Linda", 1383689);
  create_student(&students_c[8], "Abraham", 1353339);
  create_student(&students_c[9], "Heinrich", 1382376); 
  create_student(&students_c[10], "Caroline", 1382024);
  create_student(&students_c[11], "Willem", 1387009);
  create_student(&students_c[12], "Bianca", 1381987);
  create_student(&students_c[13], "Stefan", 1368754);
  memcpy(students_c1, students_c, sizeof(item)*MAX_STUDENTS_C);

  printf("***** results for 'binary_sort' *****\n");
  evaluate_binary_sort("students_a", students_a, students_a1, MAX_STUDENTS_A);
  evaluate_binary_sort("students_b", students_b, students_b1, MAX_STUDENTS_B);
  evaluate_binary_sort("students_c", students_c, students_c1, MAX_STUDENTS_C);
  
  /* node deletion from binary tree test cases */
  root_a = NULL;
  root_a1 = NULL;
  root_b = NULL;
  root_b1 = NULL;
  root_c = NULL;
  root_c1 = NULL;
  root_d = NULL;
  root_d1 = NULL;
  root_e = NULL;
  root_e1 = NULL;
  root_f = NULL;
  root_f1 = NULL;
  root_g = NULL;
  root_g1 = NULL;
  root_h = NULL;
  root_h1 = NULL;
  
  printf("***** results for 'delete_node' *****\n");
  insert_node(&root_a, create_node("Jannie"));
  insert_node(&root_a1, create_node("Jannie"));
  evaluate_delete_node(root_a, root_a1, "Jannie");
  
  insert_node(&root_b, create_node("Jannie"));
  insert_node(&root_b, create_node("Kosie"));
  insert_node(&root_b1, create_node("Jannie"));
  insert_node(&root_b1, create_node("Kosie"));
  evaluate_delete_node(root_b, root_b1, "Jannie");
  
  insert_node(&root_c, create_node("Jannie"));
  insert_node(&root_c, create_node("Kosie"));
  insert_node(&root_c1, create_node("Jannie"));
  insert_node(&root_c1, create_node("Kosie"));
  evaluate_delete_node(root_c, root_c1, "Kosie");
  
  insert_node(&root_d, create_node("Jannie"));
  insert_node(&root_d, create_node("Caroline"));
  insert_node(&root_d1, create_node("Jannie"));
  insert_node(&root_d1, create_node("Caroline"));
  evaluate_delete_node(root_d, root_d1, "Jannie");
  
  insert_node(&root_e, create_node("Jannie"));
  insert_node(&root_e, create_node("Caroline"));
  insert_node(&root_e1, create_node("Jannie"));
  insert_node(&root_e1, create_node("Caroline"));
  evaluate_delete_node(root_e, root_e1, "Caroline");
  
  insert_node(&root_f, create_node("Jannie"));
  insert_node(&root_f, create_node("Caroline"));
  insert_node(&root_f, create_node("Kosie"));
  insert_node(&root_f1, create_node("Jannie"));
  insert_node(&root_f1, create_node("Caroline"));
  insert_node(&root_f1, create_node("Kosie"));
  evaluate_delete_node(root_f, root_f1, "Jannie");
  
  insert_node(&root_g, create_node("Jannie"));
  insert_node(&root_g, create_node("Caroline"));
  insert_node(&root_g, create_node("Kosie"));
  insert_node(&root_g, create_node("Danie"));
  insert_node(&root_g, create_node("Bianca"));
  insert_node(&root_g1, create_node("Jannie"));
  insert_node(&root_g1, create_node("Caroline"));
  insert_node(&root_g1, create_node("Kosie"));
  insert_node(&root_g1, create_node("Danie"));
  insert_node(&root_g1, create_node("Bianca"));
  evaluate_delete_node(root_g, root_g1, "Caroline");

  /* binary to string conversion test cases */
  x[0] = 0;
  x[1] = 1;
  x[2] = 16;
  x[3] = -1;
  x[4] = 123;
  x[5] = 2147483647;
  x[6] = -2147483648;
  x[7] = 1000;
  x[8] = 3;
  x[9] = 2;
  printf("***** results for 'bin_to_string' ***** \n");
  for (i=0; i < MAX_BIN_TO_STRING_VALUES; i++)
    evaluate_bin_to_string(x[i]);
  return 0;
}
