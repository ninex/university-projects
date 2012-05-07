#include <stdio.h>
#include "prak4.h"

void bin_to_string(int n, char *s) {
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

int main(int argc, char *argv[]) {
  char s1[33];
  char s2[33];
  
  bin_to_string(0xff773311, s1);
  bin_to_string(reverse(0xff773311, 32), s2);
  
  printf("%s reversed is %s\n", s1, s2);
  
  return 0;
}
