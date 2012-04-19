#include <stdio.h>
#include "tut2.h"

#define MAX_FACT_TEST 7
#define MAX_ACKERMAN_X 4
#define MAX_ACKERMAN_Y 4

#define MAX_ITEMS_A 1 
#define MAX_ITEMS_B 2
#define MAX_ITEMS_C 3
#define MAX_ITEMS_D 9

#define LIST_A_NAME "list_a"
#define LIST_B_NAME "list_b"
#define LIST_C_NAME "list_c"
#define LIST_D_NAME "list_d"

int fact_c(int n) {
  if (n == 0) 
    return 1;
  
  else {
    return fact_c(n-1)*n;
  }
}

int power_c(int x, int y) {
  if (y == 0) {
    return 1;
  }
  else {
    return power_c(x, y-1)*x;
  }
}

int ackerman_c(int x, int y) {
  if (x == 0) {
    return y+1;
  }
  else if (y == 0) {
    return ackerman_c(x-1, 1);
  }
  else {
    return ackerman_c(x-1, ackerman_c(x, y-1));
  }
}

int binary_search_c(int n, int list[], int low, int high) {
  int middle;

  if (low > high)
    return -1;
    
  middle = (low+high)/2;
    
  if (n == list[middle])
    return middle;
  if (n < list[middle])
    high = middle-1;
  else
    low = middle+1;
    
  return binary_search_c(n, list, low, high);
}

void swap_c(int *x, int *y) {
  int temp;
  
  temp = *x;
  *x = *y;
  *y = temp;
}

void evaluate_fact(int n) {
  printf("fact(%d) returned %d - ", n, fact(n));
  if (fact(n) == fact_c(n))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", fact_c(n));
}


void evaluate_power(int x, int y) {
  printf("power(%d, %d) returned %d - ", x, y, power(x,y));
  if (power(x, y) == power_c(x, y))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", power_c(x, y));
}

void evaluate_ackerman(int x, int y) {
  printf("ackerman(%d, %d) returned %d - ", x, y, ackerman(x, y));
  if (ackerman(x, y) == ackerman_c(x, y))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", ackerman_c(x, y));
}

void evaluate_swap(int x, int y) {
  int sx;
  int sy;
  int c_sx;
  int c_sy;
  
  sx = x;
  sy = y;
  c_sx = x;
  c_sy = y;
  swap(&sx, &sy);
  swap_c(&c_sx, &c_sy);
  printf("swap(%d, %d) returned x=%d and y=%d - ", x, y, sx, sy);
  if ((sx == c_sx) && (sy == c_sy))
    printf("correct\n");
  else
    printf("incorrect, should return x=%d and y=%d\n", c_sx, c_sy);
}

void evaluate_binary_search(const char *s, int list[], int high, int n) {
  int pos;
  
  pos = binary_search(n, list, 0, high);
  printf("binary_search(%d, %s, 0, %d) returned %d - ", n, s, high, pos);
  if (pos == binary_search_c(n, list, 0, high))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", binary_search_c(n, list, 0, high));
}

int main(int argc, char *argv[]) {
  int i, j;
  
  int list_a[MAX_ITEMS_A] = {4};
  int list_b[MAX_ITEMS_B] = {10, 11};
  int list_c[MAX_ITEMS_C] = {3, 5, 7};
  int list_d[MAX_ITEMS_D] = {5, 23, 78, 102, 103, 104, 256, 16384, 32768};
  
  printf("***** results for 'fact' *****\n");
  for (i = 1; i < MAX_FACT_TEST; i++)
    evaluate_fact(i);
    
  printf("***** results for 'power' *****\n");
  evaluate_power(2, 0);
  evaluate_power(2, 8);
  evaluate_power(2, 16);
  evaluate_power(2, 30);    
  
  printf("***** results for 'ackerman' *****\n");
  for (i = 0; i < MAX_ACKERMAN_X; i++) {
    for (j = 0; j < MAX_ACKERMAN_Y; j++) {
      evaluate_ackerman(i, j);
    }
  }
  
  printf("***** results for 'swap' *****\n");
  evaluate_swap(3, 5);
  evaluate_swap(10, 11);
  
  printf("***** results for 'binary search' *****\n");
  evaluate_binary_search(LIST_A_NAME, list_a, MAX_ITEMS_A, list_a[0]);
  evaluate_binary_search(LIST_A_NAME, list_a, MAX_ITEMS_A, 10);
  evaluate_binary_search(LIST_B_NAME, list_b, MAX_ITEMS_B, list_b[0]);
  evaluate_binary_search(LIST_B_NAME, list_b, MAX_ITEMS_B, list_b[MAX_ITEMS_B-1]);
  evaluate_binary_search(LIST_B_NAME, list_b, MAX_ITEMS_B, 12);
  evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, list_c[0]);
  evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, list_c[1]);
  evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, list_c[MAX_ITEMS_C-1]);
  evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, 9);
  evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, list_d[0]);
  evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, list_d[MAX_ITEMS_D/2]);
  evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, list_d[MAX_ITEMS_D-1]);
  evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, 65536);

  return 0;
}
