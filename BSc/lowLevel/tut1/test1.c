#include <stdio.h>
#include "tut1.h"

int max_c(int x, int y) {
  if (x > y)
    return x;
  else
    return y;
}

int power_c(int x, int y) {
  int z;
  
  z = 1;
  while (y > 0) {
    if ((y % 2) == 1) {
      y = y-1;
      z = z*x;
    }
    else {
      y = y/2;
      x = x*x;
    }
  }
  return z;
}

int gcd_c(int m, int n) {
  int r;
  do {
    r =  m % n;
    if (r != 0) {
      m = n;
      n = r;
    }  
  } while (r != 0);
  
  return n;
}

void evaluate_max(int x, int y) {
  printf("max(%d, %d) returned %d - ", x, y, max(x,y));
  if (max(x, y) == max_c(x, y))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", max_c(x, y));
}

void evaluate_power(int x, int y) {
  printf("power(%d, %d) returned %d - ", x, y, power(x,y));
  if (power(x, y) == power_c(x, y))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", power_c(x, y));
}

void evaluate_gcd(int x, int y) {
  printf("power(%d, %d) returned %d - ", x, y, gcd(x,y));
  if (gcd(x, y) == gcd_c(x, y))
    printf("correct\n");
  else
    printf("incorrect, should return %d\n", gcd_c(x, y));
}

int main(int argc, char *argv[]) {
  printf("***** results for 'max' *****\n");
  evaluate_max(-1, -2);
  evaluate_max(0, 1);
  evaluate_max(1, 0);
  printf("***** results for 'power' *****\n");
  evaluate_power(2, 0);
  evaluate_power(2, 1);
  evaluate_power(2, 2);
  evaluate_power(2, 3);
  printf("***** results for 'gcd' *****\n");
  evaluate_gcd(2, 4);
  evaluate_gcd(4, 2);
  evaluate_gcd(100, 20);
  evaluate_gcd(32189, 57);
  return 0;
}
