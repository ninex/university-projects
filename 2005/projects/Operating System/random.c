
#include "include/i8253.h"

int a, m, q, r, z;

int uniform(int max) {
  int x, gamma;
  gamma = (a*(z % q))-(r*(z/q));
  if (gamma > 0)
    z = gamma;
  else
    z = gamma+m;
  x = z%m;
  while (x >= max)
    x = x/10;
  return x;
}	


void seed()
{
	a = 16087;
	m = 2147483647;
	q = m / a;
	r = m % a;
	z = timer_get_ticks(); /* seed */
}

