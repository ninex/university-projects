#include <string.h>
#include <stdio.h>
#include "prak1.h"

int main(int argc, char *argv[]) {
  char x[100];
  
  strcpy(x, "Hello World!");
  upcase(x);
  printf("%s\n", x);
  
  strcpy(x, "HELLO WORLD!");
  upcase(x);
  printf("%s\n", x);
  
  strcpy(x, "1234567890");
  upcase(x);
  printf("%s\n", x);
  
  return 0;
}
