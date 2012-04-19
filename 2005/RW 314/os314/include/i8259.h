#ifndef I8259_H
#define I8259_H

#include "i386.h"

/*
 * IRQ0=32 and points to the entry in the IDT associated with IRQ0. Entries 0..31
 * are reserved for processor exceptions. Device drivers should use these
 * definitions when installing or removing hardware interrupt handlers
 */
#define IRQ0  32
#define IRQ1  (IRQ0+1)
#define IRQ2  (IRQ0+2)
#define IRQ3  (IRQ0+3)
#define IRQ4  (IRQ0+4)
#define IRQ5  (IRQ0+5)
#define IRQ6  (IRQ0+6)
#define IRQ7  (IRQ0+7)
#define IRQ8  (IRQ0+8)
#define IRQ9  (IRQ0+9)
#define IRQ10 (IRQ0+10)
#define IRQ11 (IRQ0+11)
#define IRQ12 (IRQ0+12)
#define IRQ13 (IRQ0+13)
#define IRQ14 (IRQ0+14)
#define IRQ15 (IRQ0+15)

/* ----- Function pointer definition for interrupt handlers ------------------------------------- */
typedef unsigned char (*interrupt_handler) (cpu_state*, int);

/*
 * DESCRIPTION: Initializes the 8259 and IDT. All IRQ lines are masked out
 *              except IRQ-2 because it is used for chaining the primary
 *              and secondary interrupt controllers.
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void i8259_initialize();

/*
 * DESCRIPTION: Executes the STI machine instruction, allowing the CPU to recognize external
 *              interrupts before executing the HLT instruction that places the CPU
 *              in a hold state until an interrupt signal is received.
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void i8259_start();

/*
 * DESCRIPTION: Determine if an interrupt handler has been installed at
 *              a specific vector
 *       INPUT: vector - vector number to be examined
 *      OUTPUT: Returns 1 if a handler is present, otherwise 0
 */
unsigned char i8259_installed(int vector);

/*
 * DESCRIPTION: Installs an interrupt handler routine and updates a number of
 *              internal data structures, including the IDT.
 *       INPUT: vector - vector number associated with the handler.
 *              handler - function pointer to handler routine that must
 *                        be called when the associated interrupt
 *                        is triggered.
 *              p_level - Privilege level associated with the gate descriptor
 *                        inside the IDT. This parameter is specifically
 *                        intended for system calls that access the IDT
 *                        through the 'int xx' instruction.
 *      OUTPUT: Returns OK if successful, otherwise a negative result
 *              indicating an error condition (refer to 'error.h').
 */
int i8259_install_interrupt_handler(int vector, interrupt_handler handler, int p_level);

/*
 * DESCRIPTION: Removes an interrupt handler previously installed with
 *              'i8259_install_interrupt_handler'.
 *       INPUT: vector - vector number associated with the handler.
 *      OUTPUT: Returns OK if successful, otherwise a negative result
 *              indicating an error condition (refer to 'error.h')
 */
int i8259_remove_interrupt_handler(int vector);

#endif
