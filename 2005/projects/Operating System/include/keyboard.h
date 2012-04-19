#ifndef KEYBOARD_H
#define KEYBOARD_H

/* ----- Size of the internal keyboard buffer used to capture keystrokes ------------------------ */
#define KBD_BUFFER_SIZE 1024

/* ----- Specific keyboard scan codes ----------------------------------------------------------- */
#define SC_RELEASE     0x80
#define SC_LEFT_SHIFT  0x2a
#define SC_RIGHT_SHIFT 0x36
#define SC_CAPS_LOCK   0x3a
#define SC_NUM_LOCK    0x45
#define SC_SCROLL_LOCK 0x46
#define SC_ALT         0x38
#define SC_TAB         0x0f

/* ----- 8255 programmable peripheral interface (ppi) I/O ports --------------------------------- */
#define PPI_PORT_A 0x60
#define PPI_PORT_B 0x61
#define PPI_PORT_C 0x62

/* ----- 8042 Command/Status Register I/O port -------------------------------------------------- */
#define KBD_STATUS_REGISTER 0x64

/* ----- Keyboard commands (PORT 0x60) ---------------------------------------------------------- */
#define CMD_WRITE_LED 0xed

#define KBD_BUFFER_FULL 0x02

/* ----- Flags to identify special keys --------------------------------------------------------- */
#define FL_LEFT_SHIFT          0x0001
#define FL_RIGHT_SHIFT         0x0002
#define FL_LEFT_ALT            0x0004
#define FL_RIGHT_ALT           0x0008
#define FL_LEFT_CTRL           0x0010
#define FL_RIGHT_CTRL          0x0020
#define FL_CAPS_LOCK           0x0040
#define FL_NUM_LOCK            0x0080
#define FL_SCROLL_LOCK         0x0100
#define FL_CAPS_LOCK_SETTING   0x0200
#define FL_NUM_LOCK_SETTING    0x0400
#define FL_SCROLL_LOCK_SETTING 0x0800
#define FL_ALT                 0x1000

/* ----- Bit representation for LEDs (CMD_WRITE_LED) -------------------------------------------- */
#define LED_SCROLL_LOCK 0x01
#define LED_NUM_LOCK    0x02
#define LED_CAPS_LOCK   0x04

/* ----- Internal keyboard buffer used to capture keystrokes ------------------------------------ */
typedef struct keyboard_buffer {
  int head;
  int tail;
  int count;
  char data[KBD_BUFFER_SIZE];
} keyboard_buffer;

/*
 * DESCRIPTION: Wait for the internal keyboard buffer to become empty
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void keyboard_wait();

/*
 * DESCRIPTION: Set/Clear LED bits for Caps Lock, Num Lock and Scroll Lock
 *       INPUT: leds - Bit mask to enable/disable LEDs
 *      OUTPUT: NONE
 */
void keyboard_set_LEDs(unsigned char leds);

/*
 * DESCRIPTION: Read a key from the keyboard buffer
 *       INPUT: NONE
 *      OUTPUT: Returns the next available ASCII character from the
 *              keyboard buffer. Returns 0xff if the buffer is empty
 */
char keyboard_read();

/*
 * DESCRIPTION: Query keyboard buffer to determine if there are any keystrokes available
 *       INPUT: NONE
 *      OUTPUT: Returns 1 if a keystroke is available, otherwise 0
 */
unsigned char keyboard_available();

/*
 * DESCRIPTION: Initialize the keyboard driver
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void keyboard_initialize();

#endif
