#include "include/error.h"
#include "include/boottable.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/i8253.h"
#include "include/mem.h"
#include "include/keyboard.h"
#include "include/dma.h"
#include "include/floppy.h"
#include "include/exceptions.h"
#include "include/cmos.h"
#include "include/tty.h"
#include "include/sys.h"
#include "include/process.h"
#include "include/loader.h"
#include "include/random.h"
//#include "include/memdrill.h"

void main() {
  bt_initialize();
  trace_initialize();
  i386_initialize_descriptor_tables();
  i8259_initialize();
  exceptions_initialize();
  timer_initialize();
  mem_initialize();
  dma_initialize();
  keyboard_initialize();  
  floppy_initialize();  
  tty_initialize();  
  sys_initialize();   
  loader_initialize();  
  //printk("Everything running!\n");
  asm("sti"::);  
  //floppy_test();
  //mem_drill();  
  seed();
  //mem_test();
  while (1);
}
