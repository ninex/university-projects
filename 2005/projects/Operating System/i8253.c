/*
 *        MODULE: i8253.c
 *   DESCRIPTION: This module provides a simple driver to initialize and use the Intel 8253/8254
 *                programmable timer chip.
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 02.11.2004
 *     CHANGELOG:
 *                20.12.2003 - Ported from original Oberon implementation (flouw@cs.sun.ac.za)
 */
#include "include/io.h"
#include "include/stdlib.h"
#include "include/error.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/process.h"
#include "include/i8253.h"

/* ----- Intel 8253/8254 Timer registers -------------------------------------------------------- */
#define I8253_LATCH0       0x40
#define I8253_LATCH1       0x41
#define I8253_LATCH2       0x42
#define I8253_CMD_REGISTER 0x43

/* ----- Command register bit fields ------------------------------------------------------------ */
#define I8253_CMD_SC1   0x80  /* Channel selection bit 1 */
#define I8253_CMD_SC0   0x40  /* Channel selection bit 0 */
#define I8253_CMD_SL1   0x20  /* Store/Load selection bit 1 */
#define I8253_CMD_SL0   0x10  /* Store/Load selection bit 0 */
#define I8253_CMD_MODE2 0x08  /* Mode selection bit 2 */
#define I8253_CMD_MODE1 0x04  /* Mode selection bit 1 */
#define I8253_CMD_MODE0 0x02  /* Mode selection bit 0 */
#define I8253_CMD_BCD   0x01  /* Select BCD countdown */

#ifdef DEBUG_I8253
  #define PRINT_8253(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_8253(x, ...)
#endif

/* ----- Number of clock interrupts ------------------------------------------------------------- */
static int ticks;

int timer_get_ticks() {
  return ticks;
}

int timer_set(int us) {
  char n[4];

  us = ((RATE/10)*us)/100000;
  /* ----- Read the counter register of timer 0 ------------------------------------------------- */
  outb(0x00, I8253_CMD_REGISTER);
  n[0] = inb(I8253_LATCH0);
  n[1] = inb(I8253_LATCH0);
  n[2] = 0;
  n[3] = 0;

  return *(int *)(&n[0])-us;
}

unsigned char timer_expired(int handle) {
  char n[4];
  int *temp;

  /*----- Read the counter register of timer 0 -------------------------------------------------- */
  outb(0x00, I8253_CMD_REGISTER);
  n[0] = inb(I8253_LATCH0);
  n[1] = inb(I8253_LATCH0);
  n[2] = 0;
  n[3] = 0;
  temp = (int *)(&n[0]);
  if (abs(*temp-handle) < (N/2))
    return (unsigned char)(*temp < handle);
  else
    return (unsigned char)(*temp < (N+handle));
}

unsigned char timer_handler(cpu_state *state, int n) {
#ifdef DEBUG_I8253
  gotoxy(0, 0);
  printk("ticks = %d\n", ticks);
#endif
  //increment ticks
  ticks++;
  /* ----- Always reschedule on a timer interrupt ----------------------------------------------- */
 
  return 1;
}

void initialize_8253() {
  /* ----- Select channel 0 (Read/Write LSB/MSB, mode 2, BCD = 0) ------------------------------- */
  outb(I8253_CMD_MODE1 | I8253_CMD_SL0 | I8253_CMD_SL1 , I8253_CMD_REGISTER);
  
  /* ----- Write lower 8 bits of counter to Latch 0 --------------------------------------------- */
  outb(N , I8253_LATCH0);
  
  /* ----- Write upper 8 bits of counter to Latch 0 --------------------------------------------- */
  outb(N >> 8 , I8253_LATCH0);
  
  ticks = 0;
}

void timer_initialize() {
  initialize_8253();
  if (i8259_install_interrupt_handler(IRQ0, timer_handler, 0) == OK)
    printk("[timer_initialize] Interrupt handler successfully installed on IRQ-0\n");
  else
    printk("[timer_initialize] Installation of interrupt handler failed!\n");
}
