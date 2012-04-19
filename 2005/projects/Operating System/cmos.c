/*
 *        MODULE: cmos.c
 *   DESCRIPTION: This module provides access to the CMOS registers
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 15.12.2004
 *     CHANGELOG:
 *                15.12.2004 - Original implementation
 */

#include "include/asm.h"
#include "include/io.h"
#include "include/cmos.h"

unsigned char cmos_read_register(unsigned char cmos_register) {
  unsigned int eflags;
  unsigned char val;

  val = 0;
  if (cmos_register <= CMOS_DDT) {
    /* ----- Interrupts must be disabled when reading CMOS registers ---------------------------- */
    eflags = disable_interrupts();
    outb(cmos_register, CMOS_ADDRESS_REGISTER);
    val = inb(CMOS_DATA_REGISTER);
    restore_flags(eflags);
  }
  return val;
}
