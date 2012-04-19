/*
 *        MODULE: keyboard.c
 *   DESCRIPTION: This module provides a basic implementation for PC/AT keyboards.
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 27.01.2005
 *     CHANGELOG:
 *                27.01.2005 - Minor optimizations and cleaning to allow 'tty.c' to reuse some
 *                             features
 *                20.12.2003 - Ported from original Oberon implementation (flouw@cs.sun.ac.za)
 *    REFERENCES: - Frank van Gilluwe, The Undocumented PC, Addison-Wesley
 */
#include "include/asm.h"
#include "include/io.h"
#include "include/error.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/keyboard.h"

/* ----- Size of the scan code translation table ------------------------------------------------ */
#define MAX_KEYS 128

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

static keyboard_buffer buffer;
static unsigned int kbd_flags;
static unsigned char kbd_leds;

char keyboard_read() {
  char ch;
  if (buffer.count != 0) {
    ch = buffer.data[buffer.tail];
    buffer.tail = (buffer.tail+1) % KBD_BUFFER_SIZE;
    buffer.count --;
    return ch;
  } else {
    return 0xff;
  }
}

unsigned char keyboard_available() {
  return (buffer.count > 0);
}

void keyboard_wait() {
  while ((inb(KBD_STATUS_REGISTER) & KBD_BUFFER_FULL));
}

void keyboard_set_LEDs(unsigned char leds) {
  keyboard_wait();
  outb(CMD_WRITE_LED, PPI_PORT_A);
  keyboard_wait();
  outb(leds & 0x07, PPI_PORT_A);
}

unsigned char keyboard_handler(cpu_state *state, int n) {
  unsigned char scan_code;
  unsigned char port_b;
  char new_key;

  /* ----- Read scan code from port A ----------------------------------------------------------- */
  scan_code = inb(PPI_PORT_A);
  /* ----- Acknowledge keypress by setting bit 7 of port B before restoring port B's value ------ */
  port_b = inb(PPI_PORT_B);
  outb(port_b | 0x80, PPI_PORT_B);
  outb(port_b, PPI_PORT_B);

  if ((scan_code & 0x80) == 0) {
    /* ----- Key is being pressed --------------------------------------------------------------- */
    switch (scan_code) {
      case SC_LEFT_SHIFT:
        kbd_flags = kbd_flags | FL_LEFT_SHIFT;
        break;
      case SC_RIGHT_SHIFT:
        kbd_flags = kbd_flags | FL_RIGHT_SHIFT;
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
          keyboard_set_LEDs(kbd_leds);
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
          keyboard_set_LEDs(kbd_leds);
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
          keyboard_set_LEDs(kbd_leds);
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

void keyboard_initialize() {
  int i;

  /* ----- Initialize the buffer ---------------------------------------------------------------- */
  buffer.head = 0;
  buffer.tail = 0;
  buffer.count = 0;
  for (i=0; i < KBD_BUFFER_SIZE; i++)
    buffer.data[i] = 0xff;
  /* ----- Clear all the LEDs ------------------------------------------------------------------- */
  kbd_leds = 0;
  keyboard_set_LEDs(kbd_leds);
  kbd_flags = 0;
  /* ----- Install the interrupt handler -------------------------------------------------------- */
  if (i8259_install_interrupt_handler(IRQ1, keyboard_handler, 0) == OK)
    printk("[keyboard_initialize] Interrupt handler successfully installed on IRQ-1\n");
  else
    printk("[keyboard_initialize] Installation of interrupt handler failed!\n");
}
