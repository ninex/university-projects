#include <stdio.h>
#include "prak2.h"

int main(int argc, char *argv[]) {
  int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int min;
  int max;
  
  min_max(a, 10, &min, &max);
  printf("min = %d, max = %d\n", min, max);
  
  return 0;
}
