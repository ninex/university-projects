#include "include/asm.h"
#include "include/io.h"
#include "include/stddef.h"
#include "include/boottable.h"
#include "include/error.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/mem.h"
#include "include/keyboard.h"
#include "include/process.h"
#include "include/tty.h"

/* ----- Motorolla 6845 CRT registers ----------------------------------------------------------- */
#define REG_CURSOR_LOCATION_HIGH  0x0e
#define REG_CURSOR_LOCATION_LOW   0x0f

/* ----- Total number of terminals supported ---------------------------------------------------- */
#define MAX_TTY 32

/* ----- TTY 0 is reserved for the kernel (printk) ---------------------------------------------- */
#define KERNEL_TTY 0

/* ----- Number of 32-bit words needed to track allocation/release of individual ttys ----------- */
#define TTY_BITMAP_SIZE ((MAX_TTY+31)/32)

#define MAX_KEYS 128

#ifdef DEBUG_TTY
  #define PRINT_TTY(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_TTY(x, ...)
#endif

typedef struct display {
  unsigned char buffer[DISPLAY_SIZE];
  unsigned char attribute;
  int offset;
} display;

/*
 * Keyboard scan code translation table
 */
static unsigned int table[MAX_KEYS] = {
  0x00000000, 0x00000000, 0x31210000, 0x32400000, 0x33230000, 0x34240000, 0x35250000, 0x365e0000,
  0x37260000, 0x382a0000, 0x39280000, 0x30290000, 0x2d5f0000, 0x3d2b0000, 0x00000000, 0x00000000,
  0x71515100, 0x77575700, 0x65454500, 0x72525200, 0x74545400, 0x79595900, 0x75555500, 0x69494900,
  0x6f4f4f00, 0x70505000, 0x5b7b0000, 0x5d7d0000, 0x0d0d0d0d, 0x00000000, 0x61414100, 0x73535300,
  0x64444400, 0x66464600, 0x67474700, 0x68484800, 0x6a4a4a00, 0x6b4b4b00, 0x6c4c4c00, 0x3b3a0000,
  0x27220000, 0x607e0000, 0x00000000, 0x5c7c0000, 0x7a5a5a00, 0x78585800, 0x63434300, 0x76565600,
  0x62424200, 0x6e4e4e00, 0x6d4d4d00, 0x2c3c0000, 0x2e3e0000, 0x2f3f0000, 0x00000000, 0x00000000,
  0x00000000, 0x20202020, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000037,
  0x00000038, 0x00000039, 0x2d2d2d2d, 0x00000034, 0x00000035, 0x00000036, 0x2b2b2b2b, 0x00000031,
  0x00000032, 0x00000033, 0x00000030, 0x0000002e, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/* ----- List of ttys --------------------------------------------------------------------------- */
static display *tty[MAX_TTY];

/* ----- Linear address for start of display memory --------------------------------------------- */
static unsigned int display_base;

/* ----- Bitmap to track allocation of ttys ----------------------------------------------------- */
static unsigned int tty_bitmap[TTY_BITMAP_SIZE];

/* ----- Active TTY that currently owns the display --------------------------------------------- */
static int active_tty;

/* ----- CRT registers -------------------------------------------------------------------------- */
static int reg_data;
static int reg_address;

static unsigned int kbd_flags;
static unsigned char kbd_leds;
static keyboard_buffer buffer;

void wait() {
  while ((inb(KBD_STATUS_REGISTER) & KBD_BUFFER_FULL));
}

int active()
{
  return active_tty;
}

void set_LED(unsigned char leds) {
  wait();
  outb(CMD_WRITE_LED, PPI_PORT_A);
  wait();
  outb(leds & 0x07, PPI_PORT_A);
}

int tty_open() {
  int i;
  int handle;

  /* ----- Locate an open slot inside the bitmap ------------------------------------------------ */
  handle = -1;
  i = 0;
  while ((handle == -1) && (i < TTY_BITMAP_SIZE)) {
    handle = bsf(tty_bitmap[i]);
    i++;
  }

  if (handle != -1) {
    /* ----- Remove the handle and allocate memory for the buffer ------------------------------- */
    btr(tty_bitmap[handle/32], handle % 32);
    tty[handle] = mem_alloc(sizeof(display));
    tty[handle]->attribute = ATTR_BG_BLUE|ATTR_FG_WHITE;
    tty[handle]->offset = 0;
    tty_clear_display(handle);
  }
  if (get_current() != NULL)
    get_current()->pcb->handle = handle;
  return handle;
}

void tty_close(int handle) {
  /* ----- Check if it is a valid handle -------------------------------------------------------- */
  if ((handle == KERNEL_TTY) || (handle < 0) || (handle > MAX_TTY-1))
    return;
  if (bt(tty_bitmap[handle / 32], handle % 32) == 1)
    return;
  bts(tty_bitmap[handle / 32], handle %32);
}

void tty_print(int handle, char *s) {
  unsigned int base_address;

  if (bt(tty_bitmap[handle/32], handle % 32) == 1)
    return;

  if (active_tty == handle)
    base_address = display_base;
  else
    base_address = (unsigned int)(&(tty[handle]->buffer[0]));

  while (*s != '\0') {
    if (*s == '\n') {
      while ((tty[handle]->offset % LINE_WIDTH) != 0) {
      writew((unsigned short)((tty[handle]->attribute << 8)|0x20), base_address+tty[handle]->offset);
      tty[handle]->offset = tty[handle]->offset+2;
      }
    } else {
      writew((unsigned short)((tty[handle]->attribute << 8)|*s), base_address+tty[handle]->offset);
      tty[handle]->offset = tty[handle]->offset+2;
    }
    if (tty[handle]->offset >= DISPLAY_SIZE) {
      copy_4(base_address+LINE_WIDTH, base_address, ((DISPLAY_SIZE-LINE_WIDTH) >> 2));
      fill_2((base_address+(LINE_WIDTH*(MAX_ROWS-1))), (tty[handle]->attribute << 8), (LINE_WIDTH >> 1));
      tty[handle]->offset = (((MAX_ROWS-1)*MAX_COLUMNS)*2) % DISPLAY_SIZE;
    }
    s++;
  }
  /* ----- Update the physical cursor position if the kernel owns the display ------------------- */
  if (active_tty == handle) {
    outb(REG_CURSOR_LOCATION_LOW, reg_address);
    outb(tty[handle]->offset >> 1, reg_data);
    outb(REG_CURSOR_LOCATION_HIGH, reg_address);
    outb(tty[handle]->offset >> 9, reg_data);
  }
}

void tty_set_attribute(int handle, unsigned char foreground, unsigned char background) {
  if (bt(tty_bitmap[handle/32], handle % 32) != 1)
    tty[handle]->attribute = foreground | background;
}

unsigned char tty_get_attribute(int handle) {
  if (bt(tty_bitmap[handle/32], handle % 32) != 1)
    return tty[handle]->attribute;
  else
    return 0;
}

void tty_clear_display(int handle) {
  unsigned base_address;
  unsigned int attribute;
  unsigned int count;

  if (bt(tty_bitmap[handle/32], handle % 32) == 1)
    return;
  if (active_tty == handle)
    base_address = display_base;
  else
    base_address = (unsigned int)(&(tty[handle]->buffer[0]));

  attribute = 0x00200020 | (tty[handle]->attribute << 24) | (tty[handle]->attribute << 8);
  count = DISPLAY_SIZE / 4;
  __asm__ volatile ("cld \n\t \
                     rep stosl " \
                    ::"D" (base_address), "a" (attribute), "c" (count)\
                   ); \
  tty[handle]->offset = 0;
  /* ----- Update the physical cursor position if the kernel owns the display ------------------- */
  if (active_tty == handle) {
    outb(REG_CURSOR_LOCATION_LOW, reg_address);
    outb(tty[handle]->offset >> 1, reg_data);
    outb(REG_CURSOR_LOCATION_HIGH, reg_address);
    outb(tty[handle]->offset >> 9, reg_data);
  }
}

void tty_gotoxy(int handle, int x, int y) {
  /* ----- Check if the handle is valid --------------------------------------------------------- */
  if (bt(tty_bitmap[handle/32], handle % 32) == 1)
    return;
  tty[handle]->offset = (((y*MAX_COLUMNS)+x) << 1) % DISPLAY_SIZE;
  /* ----- Update the physical cursor position only if the handle owns the display -------------- */
  if (active_tty == handle) {
    outb(REG_CURSOR_LOCATION_LOW, reg_address);
    outb(tty[handle]->offset >> 1, reg_data);
    outb(REG_CURSOR_LOCATION_HIGH, reg_address);
    outb(tty[handle]->offset >> 9, reg_data);
  }
}

unsigned char tty_key_available(int handle) {
  return ((handle == active_tty) && (buffer.count > 0));
}

unsigned char tty_read_key(int handle) {
  unsigned char ch;
  if ((handle == active_tty) && (buffer.count > 0)) {
    ch = buffer.data[buffer.tail];
    buffer.tail = (buffer.tail+1) % KBD_BUFFER_SIZE;
    buffer.count--;
    return ch;
  } else {
    return 0xff;
  }
}

void set_focus(int old_tty, int new_tty, unsigned char switch_tty) {
  /* ----- No need to do anything if the actual TTY did not change ------------------------------ */
  if (new_tty != old_tty) {
    active_tty = new_tty;
    if (switch_tty == 1) {
       copy_4(display_base, &(tty[old_tty]->buffer[0]), DISPLAY_SIZE/4);
       copy_4(&(tty[active_tty]->buffer[0]), display_base, DISPLAY_SIZE/4);
    }
    /* ------ Update the physical cursor location ----------------------------------------------- */
    outb(REG_CURSOR_LOCATION_LOW, reg_address);
    outb(tty[active_tty]->offset >> 1, reg_data);
    outb(REG_CURSOR_LOCATION_HIGH, reg_address);
    outb(tty[active_tty]->offset >> 9, reg_data);
  }
}

void tty_set_kernel_focus() {
  set_focus(active_tty, KERNEL_TTY, 1);
}

unsigned char tty_handler(cpu_state *state, int n) {
  unsigned char scan_code;
  unsigned char port_b;
  int old_tty;
  int new_tty;
  char new_key;

  /* ----- Read scan code from port A ----------------------------------------------------------- */
  scan_code = inb(PPI_PORT_A);
  /* ----- Acknowledge keypress by setting bit 7 of port B and restoring port B's value --------- */
  port_b = inb(PPI_PORT_B);
  outb(port_b | 0x80, PPI_PORT_B);
  outb(port_b, PPI_PORT_B);

  if ((scan_code & 0x80) == 0) {
    /* ----- Key is being pressed --------------------------------------------------------------- */
    switch (scan_code) {
      case SC_LEFT_SHIFT:
        /* ----- Switch to new TTY when ALT-TAB is pressed -------------------------------------- */
        if ((kbd_flags & FL_ALT) != 0) {
          old_tty = active_tty;
          new_tty = (active_tty+1) % MAX_TTY;
          /* ----- Cycle through available TTYs ------------------------------------------------- */
          while ((new_tty != old_tty) && (bt(tty_bitmap[new_tty/32], new_tty % 32) != 0))
            new_tty = (new_tty+1) % MAX_TTY;
          /* ----- Clear the keyboard buffer ---------------------------------------------------- */
          buffer.head = 0;
          buffer.tail = 0;
          buffer.count = 0;
          /* ----- Make the new TTY active ------------------------------------------------------ */
          active_tty = new_tty;
          set_focus(old_tty, new_tty, 1);
        }
        break;
      case SC_ALT:
        kbd_flags = kbd_flags | FL_ALT;
        break;
      /*case SC_LEFT_SHIFT:
        kbd_flags = kbd_flags | FL_LEFT_SHIFT;
        break;*/
      case SC_RIGHT_SHIFT:
        //kbd_flags = kbd_flags | FL_RIGHT_SHIFT;
        if ((kbd_flags & FL_ALT) != 0) {
          old_tty = active_tty;
          tty_set_kernel_focus();
          if (old_tty != KERNEL_TTY)
          {
            tty_close(old_tty);
          }
          //old_tty = active_tty;
          //new_tty = (active_tty+1) % MAX_TTY;
          /* ----- Cycle through available TTYs ------------------------------------------------- */
          //while ((new_tty != old_tty) && (bt(tty_bitmap[new_tty/32], new_tty % 32) != 0))
          //  new_tty = (new_tty+1) % MAX_TTY;
          /* ----- Clear the keyboard buffer ---------------------------------------------------- */
          buffer.head = 0;
          buffer.tail = 0;
          buffer.count = 0;
          /* ----- Make the new TTY active ------------------------------------------------------ */
          //active_tty = new_tty;
          
        }
        break;
      case SC_CAPS_LOCK:
        if ((kbd_flags & FL_CAPS_LOCK_SETTING) == 0) {
          if ((kbd_flags & FL_CAPS_LOCK) == 0) {
            kbd_flags = kbd_flags | FL_CAPS_LOCK;
            kbd_leds = kbd_leds | LED_CAPS_LOCK;
          } else {
            kbd_flags = kbd_flags & (~FL_CAPS_LOCK);
            kbd_leds = kbd_leds & (~LED_CAPS_LOCK);
          }
          kbd_flags = kbd_flags | FL_CAPS_LOCK_SETTING;
          set_LED(kbd_leds);
        }
        break;
      case SC_NUM_LOCK:
        if ((kbd_flags & FL_NUM_LOCK_SETTING) == 0) {
          if ((kbd_flags & FL_NUM_LOCK) == 0) {
            kbd_flags = kbd_flags | FL_NUM_LOCK;
            kbd_leds = kbd_leds | LED_NUM_LOCK;
          } else {
            kbd_flags = kbd_flags & (~FL_NUM_LOCK);
            kbd_leds = kbd_leds & (~LED_NUM_LOCK);
          }
          kbd_flags = kbd_flags | FL_NUM_LOCK_SETTING;
          set_LED(kbd_leds);
        }
        break;
      case SC_SCROLL_LOCK:
        if ((kbd_flags & FL_SCROLL_LOCK_SETTING) == 0) {
          if ((kbd_flags & FL_SCROLL_LOCK) == 0) {
            kbd_flags = kbd_flags | FL_SCROLL_LOCK;
            kbd_leds = kbd_leds | LED_SCROLL_LOCK;
          } else {
            kbd_flags = kbd_flags & (~FL_SCROLL_LOCK);
            kbd_leds = kbd_leds & (~LED_SCROLL_LOCK);
          }
          kbd_flags = kbd_flags | FL_SCROLL_LOCK_SETTING;
          set_LED(kbd_leds);
        }
        break;
      default:
        if (buffer.count < KBD_BUFFER_SIZE) {
          /* ----- Translate the scan code and insert ASCII character into the buffer ----------- */
          if ((kbd_flags & (FL_LEFT_SHIFT | FL_RIGHT_SHIFT)) != 0)
            buffer.data[buffer.head] = (char)(table[scan_code] >> 16);
          else {
            /* ----- Select normal ASCII character ---------------------------------------------- */
            buffer.data[buffer.head] = (char)(table[scan_code] >> 24);
            /* ----- Check if Caps Lock is active and modify ASCII value accordingly ------------ */
            new_key = (char)(table[scan_code] >> 8);
            if ((new_key != 0) && ((kbd_flags & FL_CAPS_LOCK) != 0))
              buffer.data[buffer.head] = new_key;
            /* ----- Check if Num Lock is active and modify ASCII value accordingly ------------- */
            new_key = (char)(table[scan_code]);
            if ((new_key != 0) && ((kbd_flags & FL_NUM_LOCK) != 0))
              buffer.data[buffer.head] = new_key;
          }
          /* ----- Update the buffer ------------------------------------------------------------ */
          buffer.head = (buffer.head+1) % KBD_BUFFER_SIZE;
          buffer.count++;
        }
        /* ----- Drop the keystroke if the buffer is full --------------------------------------- */
        break;
    }
  } else {
    /* ----- Key is being released -------------------------------------------------------------- */
    switch (scan_code & (~SC_RELEASE)) {
      case SC_ALT:
        kbd_flags = kbd_flags & (~FL_ALT);
        break;
      case SC_LEFT_SHIFT:
        kbd_flags = kbd_flags & (~FL_LEFT_SHIFT);
        break;
      case SC_RIGHT_SHIFT:
        kbd_flags = kbd_flags & (~FL_RIGHT_SHIFT);
        break;
      case SC_NUM_LOCK:
        kbd_flags = kbd_flags & (~FL_NUM_LOCK_SETTING);
        break;
      case SC_CAPS_LOCK:
        kbd_flags = kbd_flags & (~FL_CAPS_LOCK_SETTING);
        break;
      case SC_SCROLL_LOCK:
        kbd_flags = kbd_flags & (~FL_SCROLL_LOCK_SETTING);
        break;
      default:
        break;
    }
  }

  return 0;
}

/* ----- Replacement for '__write_string' in 'trace.c'------------------------------------------- */
void write_string0(char *s) {
  tty_print(KERNEL_TTY, s);
}

/* ----- Replacement for '__clear_display' in 'trace.c' ----------------------------------------- */
void clear_display0() {
  tty_clear_display(KERNEL_TTY);
}

/* ----- Replacement for '__set_attribute' in 'trace.c' ----------------------------------------- */
void set_attribute0(unsigned char foreground, unsigned char background) {
  tty_set_attribute(KERNEL_TTY, foreground, background);
}

/* ----- Replacement for '__get_attribute' in 'trace.c' ----------------------------------------- */
unsigned char get_attribute0() {
  return tty_get_attribute(KERNEL_TTY);
}

/* ----- Replacement for '__gotoxy' in 'trace.c' ------------------------------------------------ */
void gotoxy0(int x, int y) {
  tty_gotoxy(KERNEL_TTY, x, y);
}

void tty_initialize() {
  int i;

  /* ----- Get the linear address of the display memory from the boot table --------------------- */
  display_base = bt_entry(BT_DISPLAY_BASE_ADDRESS);
  /*
   * For MDA adapters, the CRT registers are located at I/O base address 0x3b4 (address register)
   * and 0x3b5 (data register) and for colour adapters at I/O base address 0x3d4 (address register)
   * and 0x3d5 (data register). The value of this entry is obtained from the BIOS by the bootstrap
   * loader.
   */
  reg_address = bt_entry(BT_DISPLAY_ADDRESS_REGISTER);
  reg_data = reg_address+1;
  /* ----- Only initialize tty0 (kernel output) ------------------------------------------------- */
  tty[KERNEL_TTY] = mem_alloc(sizeof(display));
  if (tty[KERNEL_TTY] == NULL) {
    printk("[tty_initialize] Could not allocate memory for kernel tty!\n");
    return;
  }
  /* ----- Copy 'trace' information to tty[TTY_KERNEL] ------------------------------------------ */
  tty[KERNEL_TTY]->attribute = get_attribute();
  tty[KERNEL_TTY]->offset = trace_get_offset();
  /* ----- Kernel owns the display -------------------------------------------------------------- */
  active_tty = KERNEL_TTY;
  /* ----- Initialize the tty bitmap (only TTY_KERNEL is allocated) ----------------------------- */
  for (i = 0; i < TTY_BITMAP_SIZE; i++)
    tty_bitmap[i] = 0xffffffff;
  btr(tty_bitmap[KERNEL_TTY / 32], KERNEL_TTY % 32);
  /* ----- Update the function pointers defined in 'trace.c' ------------------------------------ */
  write_string = write_string0;
  set_attribute = set_attribute0;
  get_attribute = get_attribute0;
  gotoxy = gotoxy0;
  clear_display = clear_display0;
  /* ----- Determine if an interrupt handler for the keyboard is already present and remove it -- */
  if (i8259_installed(IRQ1) == 1) {
    if (i8259_remove_interrupt_handler(IRQ1) == OK)
      printk("[tty_initialize] Interrupt handler successfully removed from IRQ-1\n");
    else
      printk("[tty_initialize] Removal of interrupt handler failed!\n");
  }
  /* ----- Install the new interrupt handler ---------------------------------------------------- */
  if (i8259_install_interrupt_handler(IRQ1, tty_handler, 0) == OK)
    printk("[tty_initialize] Interrupt handler successfully installed on IRQ-1\n");
  else
    printk("[tty_initialize] Installation of interrupt handler failed!\n");
}
