#include "include/asm.h"
#include "include/stddef.h"
#include "include/error.h"
#include "include/trace.h"
#include "include/mem.h"
#include "include/keyboard.h"
#include "include/floppy.h"

#define N 10
#define SECTOR 400

void floppy_test() {
  char *buf;
  int i;
  int j;
  int res;

  buf = mem_alloc(10*SECTOR_SIZE);

  if (buf == NULL) {
    printk("[floppy_test] could not allocate memory for disk buffer - exiting\n");
    return;
  }

  printk("[floppy_test] - press 'h' for help\n");

  while(1) {
    if (keyboard_available()) {
      switch (keyboard_read()) {
        case '1':
          printk("[floppy_test] floppy_read(%d, %d, %d, %#.8x, %d)\n", 0, SECTOR, N, buf, N*SECTOR_SIZE);
          fill_4(buf, 0x0, (N*SECTOR_SIZE)/4);
          res = floppy_read(0, SECTOR, N, buf, N*SECTOR_SIZE);
          printk("[floppy_test] floppy_read returned %d\n", res);
          if (res == OK) {
            printk("[floppy_test] displaying data ...\n");
            for (i = 0; i < 10; i++) {
              for (j = 0; j < 16; j++) {
                printk("%#.2hhx ", buf[(i*SECTOR_SIZE)+j]);
                if (((i+1) % 16) == 0)
                  printk("\n");
              }
            }
          }
          break;

        case '2':
          printk("[floppy_test] floppy_write(%d, %d, %d, %#.8x, %d)\n", 0, SECTOR, N, buf, N*SECTOR_SIZE);
          fill_4(buf, 0x41424344, (N*SECTOR_SIZE)/4);
          res = floppy_write(0, SECTOR, N, buf, N*SECTOR_SIZE);
          printk("[floppy_test] floppy_write returned %d\n", res);
          break;

        case 'c':
          clear_display();
          break;

        case 'h':
          printk("[floppy_test] Options:\n");
          printk("              '1' - Read %d sectors from drive 0 starting at %d\n", N, SECTOR);
          printk("              '2' - Write %d sectors to drive 0 starting at %d\n", N, SECTOR);
          printk("              'c' - Clear display\n");
          printk("              'h' - Help\n");
          break;

        default:
          break;
      }
    }
  }
}
