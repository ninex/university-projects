/*
 *        MODULE: trace.c
 *   DESCRIPTION: This module provides low-level output primitives
 *                that can be used to display debug information. The module
 *                supports both Monochrome Display Adapters (MDA) and
 *                colour adapters (CGA/EGA/VGA/MCGA). It is assumed
 *                that the bootstrap loader will correctly configure
 *                the display to 80x25 (mode 3 according to the BIOS, or
 *                7 in the case of a MDA).
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 25.01.2005
 *     CHANGELOG:
 *                25.01.2005 - __printk() has been removed. printk() now prints
 *                             to an internal buffer (similar to sprintf()).
 *                             Once the output string has been processed it
 *                             is written to the display using __write_string(). The modifications
 *                             were necessary to allow for a more efficient implementation in
 *                             'tty.c'
 *                18.12.2004 - printk now scrolls when reaching the end of the
 *                             display
 *                22.11.2004 - Added field width specifier ('.')
 *                21.11.2004 - Added clear_display and gotoxy.
 *                             trace_set_attributes replaced by set_attribute.
 *                             Added support for length modifiers in printk
 *                20.11.2004 - New implementation of __printk that's partially
 *                             based on 'vsnprintf' of Linux kernel. Only a
 *                             subset of the modifiers/specifiers are
 *                             supported.
 *                20.12.2003 - Ported from original Oberon implementation (flouw@cs.sun.ac.za)
 *                             Note: The original port provided the same primitives as the
 *                             Oberon implementation, ie. individual functions to
 *                             display characters, strings and values. The new implementation
 *                             uses a single output function called 'printk' that works
 *                             similar to 'printf'
 */
#include <stdarg.h>
#include "include/boottable.h"
#include "include/io.h"
#include "include/asm.h"
#include "include/trace.h"

/* ----- Motorolla 6845 CRT registers ----------------------------------------------------------- */
#define REG_CURSOR_LOCATION_HIGH  0x0e
#define REG_CURSOR_LOCATION_LOW   0x0f

/* ----- Flags used to track conversion specifiers and length modifiers ------------------------- */
#define FL_UPPER   0x01
#define FL_SIGN    0x02
#define FL_INT     0x04
#define FL_SPECIAL 0x08
#define FL_WIDTH   0x10

/* ----- Size of the internal buffer used to process format specifiers -------------------------- */
#define PRINTK_BUFFER_SIZE 1024

/* ----- Current offset relative to the display base -------------------------------------------- */
static int display_offset;
static int display_base;

/* ----- Display adapter registers -------------------------------------------------------------- */
static int reg_address;
static int reg_data;

/* ----- Current foreground/background attribute ------------------------------------------------ */
static unsigned char display_attribute;

/* ----- Used by %x conversion specifier -------------------------------------------------------- */
static char small_digits[] = "0123456789abcdef";
/* ----- Used by %X conversion specifier -------------------------------------------------------- */
static char large_digits[] = "0123456789ABCDEF";

/* ----- Intermediate buffer used by printk ----------------------------------------------------- */
static char buffer[PRINTK_BUFFER_SIZE];

void __write_string(char *s) {
  while (*s != '\0') {
    if (*s == '\n') {
      while ((display_offset % LINE_WIDTH) != 0) {
        writew((unsigned short)((display_attribute << 8)|0x20), display_base+display_offset);
        display_offset = display_offset+2;
      }
    } else {
      writew((unsigned short)((display_attribute << 8)|*s), display_base+display_offset);
      display_offset = display_offset+2;
    }
    if (display_offset >= DISPLAY_SIZE) {
      copy_4(display_base+LINE_WIDTH, display_base, ((DISPLAY_SIZE-LINE_WIDTH) >> 2));
      fill_2((display_base+(LINE_WIDTH*(MAX_ROWS-1))), (display_attribute << 8), (LINE_WIDTH >> 1));
      display_offset = (((MAX_ROWS-1)*MAX_COLUMNS)*2) % DISPLAY_SIZE;
    }
    s++;
  }
  /* ----- Update the physical cursor position -------------------------------------------------- */
  outb(REG_CURSOR_LOCATION_LOW, reg_address);
  outb(display_offset >> 1, reg_data);
  outb(REG_CURSOR_LOCATION_HIGH, reg_address);
  outb(display_offset >> 9, reg_data);
}

int int2string(long long x, char *s, int base, int flags, int width) {
  int i, j;
  char tmp[32];
  char sign;

  i = 0;
  sign = 0;
  if ((x < 0) && ((flags & FL_SIGN) != 0)) {
    x = -x;
    sign = '-';
  }

  if ((flags & FL_UPPER) != 0) {
    do {
      tmp[i] = large_digits[div_64(x, base)];
      i++;
      width--;
    } while (x != 0);
  } else {
    do {
      tmp[i] = small_digits[div_64(x, base)];
      i++;
      width--;
    } while (x != 0);
  }
  i--;
  j = 0;
  if ((flags & FL_SPECIAL) != 0) {
    if (base == 8) {
      s[j] = '0';
      j++;
    } else if (base == 16) {
      s[j] = '0';
      j++;
      if ((flags & FL_UPPER) != 0)
        s[j] = 'X';
      else
        s[j] = 'x';
      j++;
    }
  }

  if (((flags & FL_SIGN) != 0) && (sign != 0)) {
    s[j] = sign;
    j++;
  }

  if ((flags & FL_WIDTH) != 0) {
    while (width > 0) {
      s[j] = '0';
      j++;
      width--;
    }
  }

  do {
    s[j] = tmp[i];
    j++;
    i--;
  } while (i >= 0);
  s[j] = 0;

  return j;
}

void printk(char *format, ...) {
  va_list ap;
  int base;
  int flags;
  int width;
  int i;
  char *c;
  char *s;
  char ch;
  char length;
  unsigned long long value;

  va_start(ap, format);
  c = format;
  i = 0;
  while (*c != 0) {
    base = 10;
    flags = 0;
    value = 0;
    length = 0;
    width = 0;

    if (*c == '%') {
      c++;
      /* ----- Special characters --------------------------------------------------------------- */
      if (*c == '#') {
        flags = flags | FL_SPECIAL;
        c++;
      }
      /* ----- Field width ---------------------------------------------------------------------- */
      if (*c == '.') {
        c++;
        flags = flags | FL_WIDTH;
        while ((*c >= '0') && (*c <= '9')) {
          width = (width*10)+(*c-'0');
          c++;
        }
      }
      /* ----- Length modifiers ----------------------------------------------------------------- */
      if ((*c == 'h') || (*c == 'l') || (*c == 'L')) {
        length = *c;
        c++;
        if ((*c == 'l') && (length == 'l')) {
          length = 'L';
          c++;
        } else if ((*c == 'h') && (length == 'h')) {
          length = 'H';
          c++;
        }
      }
      /* ----- Conversion specifiers ------------------------------------------------------------ */
      switch (*c) {
        case 'c':
          ch = (unsigned char)(va_arg(ap, int));
          buffer[i] = ch;
          i++;
          break;
        case 'd':
        case 'i':
          flags = flags | FL_SIGN | FL_INT;
          break;
        case 'o':
          flags = flags | FL_INT;
          base = 8;
          break;
        case 's':
          s = (char *)(va_arg(ap, int));
          while (*s != 0) {
            buffer[i] = *s;
            i++;
            s++;
          }
          break;
        case 'u':
          flags = flags | FL_INT;
          break;
        case 'X':
          flags = flags | FL_UPPER | FL_INT;
          base = 16;
          break;
        case 'x':
          flags = flags | FL_INT;
          base = 16;
          break;
        default:
          buffer[i] = *c;
          i++;
          break;
      }
      /* ----- Cast the next integer argument to an appropriate type ---------------------------- */
      if ((flags & FL_INT) != 0) {
        if (length == 'L')
          value = va_arg(ap, long long);
        else if (length == 'l') {
          value = va_arg(ap, unsigned long);
          if ((flags & FL_SIGN) != 0)
            value = (signed long) value;
        } else if (length == 'h') {
          value = (unsigned short) va_arg(ap, int);
          if ((flags & FL_SIGN) != 0)
            value = (signed short) value;
        } else if (length == 'H') {
          value = (unsigned char) va_arg(ap, int);
          if ((flags & FL_SIGN) != 0)
            value = (signed char) value;
        } else {
          value = (unsigned int) va_arg(ap, int);
          if ((flags & FL_SIGN) != 0)
            value = (signed int) value;
        }
        /* ----- Convert the number into a string ----------------------------------------------- */
        i = i+int2string(value, &buffer[i], base, flags, width);
      }
    } else {
      buffer[i] = *c;
      i++;
    }
    c++;
  }

  buffer[i] = '\0';
  va_end(ap);
  write_string(buffer);
}

void __clear_display() {
  unsigned int attribute;
  unsigned int count;
  attribute = 0x00200020 | ((display_attribute << 24) | (display_attribute << 8));
  count = DISPLAY_SIZE / 4;
  __asm__ volatile ("cld \n\t \
                     rep stosl " \
                    ::"D" (display_base), "a" (attribute), "c" (count)\
                   ); \
  display_offset = 0;
  /* ----- Update the physical cursor position -------------------------------------------------- */
  outb(REG_CURSOR_LOCATION_LOW, reg_address);
  outb(display_offset >> 1, reg_data);
  outb(REG_CURSOR_LOCATION_HIGH, reg_address);
  outb(display_offset >> 9, reg_data);
}

void __set_attribute(unsigned char foreground, unsigned char background) {
  display_attribute = foreground | background;
}

unsigned char __get_attribute() {
  return display_attribute;
}

int trace_get_offset() {
  return display_offset;
}

void __gotoxy(int x, int y) {
  display_offset = (((y*MAX_COLUMNS)+x) << 1) % DISPLAY_SIZE;
  /* ----- Update the physical cursor position -------------------------------------------------- */
  outb(REG_CURSOR_LOCATION_LOW, reg_address);
  outb(display_offset >> 1, reg_data);
  outb(REG_CURSOR_LOCATION_HIGH, reg_address);
  outb(display_offset >> 9, reg_data);
}

void trace_initialize() {
  int i;
  /*
   * For MDA adapters, the linear display address is 0x000b0000. For CGA/EGA/VGA/MCGA adapters
   * it resides at 0x000b8000
   */
  display_base = bt_entry(BT_DISPLAY_BASE_ADDRESS);
  /*
   * For MDA adapters, the CRT registers are located at I/O base address 0x3b4 (address register)
   * and 0x3b5 (data register) and for colour adapters at I/O base address 0x3d4 (address register)
   * and 0x3d5 (data register). The value of this entry is obtained from the BIOS by the bootstrap
   * loader.
   */
  reg_address = bt_entry(BT_DISPLAY_ADDRESS_REGISTER);
  reg_data = reg_address+1;
  /*
   * Function pointers for basic display primitives. The function pointers will be assigned new
   * values once the I/O sub-system takes control and allows one to continue using 'printk'
   * throughout the kernel.
   */
  write_string = __write_string;
  set_attribute = __set_attribute;
  get_attribute = __get_attribute;
  gotoxy = __gotoxy;
  clear_display = __clear_display;
  /* ----- Initialize the display --------------------------------------------------------------- */
  set_attribute(ATTR_BG_BLUE, ATTR_FG_YELLOW);
  clear_display();

/*  printk("hello world!\n");
  printk("0 in hex = %x\n", 0);
  printk("15 in hex = %x\n", 15);
  printk("15 in hex = %X\n", 15);
  printk("%%\n");
  printk("unsigned long long = %llx\n", 0x8000000000000000);
  printk("signed long long = %lld\n", 0x8000000000000000);
  printk("0x12345678 as a short = %hx\n", 0x12345678);
  printk("%hhx %hhx %hhx %hhx\n", 0x40, 0xff, 0x60, 0x70);
  printk("%#hhx %#hhx %#hhx %#hhx\n", 0x40, 0xff, 0x60, 0x70);
  printk("%#d\n", 5);
  printk("%#.8x\n", 5);
  printk("%.3d\n", 123);
  printk("%.3d\n", 12);
  printk("%.3d\n", 1234);

  for (i = 0; i < 10; i++) {
    printk("boot table entry at [%#x] = %#x\n", 0xffc-(i*4), readl(0xffc-(i*4)));
  }

  i = 0;
  while(1) {
    printk("%d\n", i);
    i++;
  } */
}
