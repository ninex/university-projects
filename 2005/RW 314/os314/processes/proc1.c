#include "include/crt.h"

int main() {
  int handle;
  int i;
  
  handle = crt_open();

  i = 0;
  while (1) {
    crt_gotoxy(handle, 0, 0);
    crt_printf(handle, "i=%d", i);
    i++;
  }
  
  return 0;
}
