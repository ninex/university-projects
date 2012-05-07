/*
 *        MODULE: i8259.c
 *   DESCRIPTION: This module provides basic functions to install and manage interrupt handlers. It
 *                is also responsible for initializing the Intel 8259/8259A Programmable Interrupt
 *                Controller (PIC).
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 25.01.2005
 *     CHANGELOG:
 *                25.01.2005 - Added i8259_installed()
 *                15.09.2004 - New functions added:
 *                             i8259_enable_irq()
 *                             i8259_disable_irq()
 *                20.12.2003 - Ported from original Oberon implementation (flouw@cs.sun.ac.za)
 *    REFERENCES: - 8259A Programmable Interrupt Controller (8295A/8259A-2), Intel Corporation,
 *                  order number 231468-003, December 1988
 *                - Frank van Gilluwe, Undocumented PC, Addison-Wesley
 */
#include "include/asm.h"
#include "include/io.h"
#include "include/error.h"
#include "include/stddef.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/process.h"
#include "include/loader.h"

/* ----- Definitions for primary interrupt controller (IRQ0..IRQ7) ------------------------------ */
#define I8259_PIC0_PORTA 0x20
#define I8259_PIC0_PORTB 0x21

/* ----- Definitions for secondary interrupt controller (IRQ8..IRQ15) --------------------------- */
#define I8259_PIC1_PORTA 0xa0
#define I8259_PIC1_PORTB 0xa1

/* ----- Definitions for ICW1 (primary and secondary controllers) ------------------------------- */
#define ICW1_IC4  0x01 /* ICW4 must be read */
#define ICW1_SNGL 0x02 /* Single - Only one controler present in system */
#define ICW1_ADI  0x04 /* CALL address interval: selects 4 if 1, or 8 if 0 */
#define ICW1_LTIM 0x08 /* Select level triggered interrupt mode */
#define ICW1_BIT4 0x10 /* */

/* ----- Definitions for ICW3 (primary controller) ---------------------------------------------- */
#define ICW3_MASTER_S0 0x01
#define ICW3_MASTER_S1 0x02
#define ICW3_MASTER_S2 0x04
#define ICW3_MASTER_S3 0x08
#define ICW3_MASTER_S4 0x10
#define ICW3_MASTER_S5 0x20
#define ICW3_MASTER_S6 0x40
#define ICW3_MASTER_S7 0x80

/* ----- Definitions for ICW3 (secondary controller) -------------------------------------------- */
#define ICW3_SLAVE_ID0 0x01
#define ICW3_SLAVE_ID1 0x02
#define ICW3_SLAVE_ID2 0x04

/* ----- Definitions for ICW4 (primary and secondary controllers) ------------------------------- */
#define ICW4_uPM  0x01 /* Microprocessor mode (selects 8086 systems) */
#define ICW4_AEOI 0x02 /* Automatic end of interrupt mode */
#define ICW4_MS   0x04 /* Select PIC as master when using buffered mode */
#define ICW4_BUF  0x08 /* Buffered mode */
#define ICW4_SFNM 0x10 /* Special fully nested mode */

/*
 * Offsets for hardware interrupts (IRQ0..IRQ15) in the IDT. The 8259 will add these offsets to
 * the IRQ line number to inform the processor where the descriptor in the IDT for the specific
 * handler can be located. IDT entries 0..31 are reserved for processor exceptions.
 */
#define IDT_OFFSET_IRQ0 0x20
#define IDT_OFFSET_IRQ8 0x28

/*
 * NB: The primary controller specifies the IRQ line used for chaining as the corresponding
 *     bit position while the slave specifies the actual line number in binary
 */
#define CHAIN_MASTER_IRQ ICW3_MASTER_S2
#define CHAIN_SLAVE_IRQ  ICW3_SLAVE_ID1

#define EOI 0x20

#ifdef DEBUG_I8259
  #define PRINT_8259(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_8259(x, ...)
#endif

/*
 * Macro used to return directly after an interrupt occurs. The cpu_state
 * structure is also removed from the stack. The outer-level SS and ESP fields
 * will automatically be removed when the processor detects that the RPL/DPL field
 * of CS on the stack differs from its current value (CPL).
 */
#define return_from_interrupt(state) ({ \
  __asm__ volatile ("movl  %%eax, %%esp\n\t \
                     popal \n\t \
                     addl  $4, %%esp\n\t \
                     iretl" \
                    ::"a"((long)(state))); \
})

/* ----- Table containing 'glue code' associated with interrupt handlers ------------------------ */
static char jump_table[MAX_IDT_ENTRIES][16];

/* ----- Table containing function pointers of interrupt handlers ------------------------------- */
static interrupt_handler interrupt_table[MAX_IDT_ENTRIES];

/* ----- */
static int level;

/*
 * Bitmap tracking installed interrupt handlers. The bitmap index is computed
 *  as 'interrupt_number/32' and the bit position as 'interrupt_number%32'
 *  A '1' inside the bitmap indicates that a handler has been installed
 *  while a 0 indicates that there is no interrupt handler. Refer
 *  to 'i8259_install_interrupt_handler' and 'i8259_remove_interrupt_handler'
 *  for more details regarding its use.
 */
static int idt_bitmap[MAX_IDT_ENTRIES/32];


/*
This function is used to determine whether or not an interrupt handler is currently installed within the IDT at the specified vector. The function must return 1 if a handler is already present or 0 otherwise. You should use the vector number to access the bit position within the idt_bitmap structure to determine whether or not a handler is present. 
*/
unsigned char i8259_installed(int vector)
{
// using function bt(value,n) in asm.h
	return bt(idt_bitmap[vector/32],vector%32);
}


int i8259_install_interrupt_handler(int vector, interrupt_handler handler, int p_level) 
{
/*This function will be used to install an interrupt handler for the specified vector. The function must return OK if the installation was successful or otherwise an error code (see error.h). The interrupt mask register should be updated if the vector number refers to an entry associated with IRQ0..IRQ15. The function must return -ERR_I8259_INVALID_VECTOR if the vector is out of bounds or -ERR_I8259_INSTALLED if a handler is already present.*/

  /* ----- Check if the IDT vector is valid ----------------------------------------------------- */
  if ((vector < 0) || (vector >= MAX_IDT_ENTRIES))
  {
  	return -ERR_I8259_INVALID_VECTOR;
  }
  
  /* ----- Check if the IDT currently contains a handler for the specified vector --------------- */
  
  if (i8259_installed(vector) == 0)
  {
  	/* ----- Create a new gate descriptor and place it into the IDT ------------------------------- */
  	i386_initialize_idt_entry(vector, (int)(&jump_table[vector][0]),p_level);
  
  	/* ----- Add the handler to our interrupt table ----------------------------------------------- */
  	interrupt_table[vector] = handler;
  
  	/* ----- Update the IMR if the handler is associated with IRQ0..IRQ15 ------------------------- */
  	unsigned char initial_IMR_reading;
  
  	if ((vector - IRQ0 >= 0) && (vector - IRQ0 < 8 ))	// pic - 0
  	{
  	    initial_IMR_reading = inb(I8259_PIC0_PORTB);
	 	btr(initial_IMR_reading,vector - IRQ0);
		outb(initial_IMR_reading,I8259_PIC0_PORTB);
  	}
  
  	if ((vector - IRQ0 >= 8) && (vector - IRQ0 <= 15 )) // pic 1
  	{
  		initial_IMR_reading = inb(I8259_PIC1_PORTB);
	 	btr(initial_IMR_reading,vector - IRQ8);
	 	outb(initial_IMR_reading,I8259_PIC1_PORTB);
  	}
  	/* ----- Mark the entry inside the bitmap ----------------------------------------------------- */
  	bts(idt_bitmap[vector/32],vector%32);
  
	return OK;
  }	
  else
  {
  	return -ERR_I8259_INSTALLED;
  }	
}

int i8259_remove_interrupt_handler(int vector) 
{
  /* ----- Check if the IDT vector is valid ----------------------------------------------------- */
  if ((vector < 0) || (vector >= MAX_IDT_ENTRIES))
  {
  	return -ERR_I8259_INVALID_VECTOR;
  }
  /* ----- Check if the IDT currently contains a handler for the specified vector --------------- */
  if (i8259_installed(vector) == 1)
  {
  	/* ----- Update the entry inside the IDT ------------------------------------------------------ */
  	i386_initialize_idt_entry(vector, (int)(&jump_table[vector][0]),0);
  	/* ----- Remove the handler from our interrupt table ------------------------------------------ */
  	interrupt_table[vector] = NULL;
  
  	/* ----- Update the IMR if the handler is associated with IRQ0..IRQ15 ------------------------- */
	
	unsigned char initial_IMR_reading;
	
  	if ((vector - IRQ0 >= 0) && (vector - IRQ0 < 8 ))	// pic - 0
  	{
  		initial_IMR_reading = inb(I8259_PIC0_PORTB);
	  	btr(initial_IMR_reading,vector - IRQ0);
      	outb(initial_IMR_reading,I8259_PIC0_PORTB);
  	}
  
  	if ((vector - IRQ0 >= 8) && (vector - IRQ0 <= 15 )) // pic 1
  	{
  	  initial_IMR_reading = inb(I8259_PIC1_PORTB);
	  btr(initial_IMR_reading,vector - IRQ8);
 	  outb(initial_IMR_reading,I8259_PIC1_PORTB);
  	}
  
  	/* ----- Clear the entry inside the bitmap ---------------------------------------------------- */
  	btr(idt_bitmap[vector/32],vector%32);
  	return OK;
  }
  else
  {
  	return -ERR_I8259_REMOVED;
  }
}

void i8259_start() 
{
  level = -1;
  __asm__ volatile ("sti \n\t \
                     hlt \n\t");
}

/*
 * All interrupts (exceptions, traps, faults, IRQ and software interrupts)
 *  are redirected to this routine that in turn calls the appropriate
 *  interrupt handler through a function pointer stored inside the
 *  'interrupt_table' array. It also takes care of acknowledging interrupts
 *  for hardware devices by sending an End of Interrupt (EOI) signal to the
 *  appropriate controller. The diagram below illustrates the layout of
 *  the stack upon entry.
 *
 *  +------------------+
 *  | Outer level SS   | Automatically pushed by the CPU during control
 *  | Outer level ESP  |  transfer between different privilege levels
 *  +------------------+
 *  | EFLAGS           | Automatically pushed by the CPU when control
 *  | CS               |  is transferred through the IDT
 *  | EIP              |
 *  +------------------+
 *  | Error code / EBP | The 'Error code' field is pushed on the stack by
 *  |                  |  traps and faults (eg. Page Fault Exception).
 *  |                  |  The glue code executed after an interrupt will
 *  |                  |  copy the EBP register on the stack to ensure that
 *  |                  |  it always contains the same amount of items and
 *  |                  |  only serve as a placeholder in case there is no
 *  |                  |  error code.
 *  +------------------+
 *  | EAX              | The general registers are saved by the glue code
 *  | ECX              |  and forms part of the CPU state (refer to the
 *  | EDX              |  'cpu_state' struct in 'i386.h'). Note that the ESP
 *  | EBX              |  field refers to the kernel stack top. The stack top
 *  | ESP (kernel)     |  for user-level processes can be obtained from the
 *  | EBP              |  'Outer level ESP' field ('user_esp' according to
 *  | ESI              |  the struct in 'i386.h')
 *  | EDI              |
 *  +------------------+
 *  | n                | The parameters of the 'field_interrupt' function
 *  | state            |  are placed on the stack by the glue code before
 *  | EIP              |  it is called. The EIP register is pushed by
 *  |                  |  by the 'call' instruction
 *  +------------------+
 *  | EBP              | The frame pointer is pushed as part of the
 *  |                  |  prologue code when 'field_interrupt' begins
 *  |                  |  executing
 *  +------------------+
 */
void field_interrupt(cpu_state *state, int n) 
{
  int reschedule;

  /*
   * Check if a handler routine has been installed and call it. The NULL check is probably
   * redundant, but performed as an extra safety measure
   */
  if ((bt(idt_bitmap[n/32], n % 32) == 1) && (interrupt_table[n] != NULL))
    reschedule = interrupt_table[n](state, n);
  else {
    printk("[field_interrupt]: Default handler for vector %d\n", n);
    reschedule = 0;
  }
  level++;
  /* ----- Take the appropriate action if the interrupt was an exception ------------------------ */
  if (n < IRQ0) {
    /* ----- Halt everything if the exception originated inside the kernel ---------------------- */
    if ((state->cs & 0x03) != 0x03) {
       printk("[field_interrupt]: Exception occurred inside kernel\n");
       while (1);
    } else 
	{
      /* ----- Kill process if the exception originated at the user-level ----------------------- */
	  printk("8259 : should kill a process here\n");
	  /*process_queue * dying_process = get_current_pcb();
	  aniahilate_process(dying_process,state);*/
    }
    while (1);
  } else if (n <= IRQ15) 
  {
    /*
     * Issue a non-specific End Of Interrupt (EOI) signal to the PIC. If the interrupt
     * originated from the secondary PIC (IRQ8..IRQ15), both PICS must be acknowledged
     */
    if (n >= IRQ8)
      outb(EOI, I8259_PIC1_PORTA);
    outb(EOI, I8259_PIC0_PORTA);
  }
  level--;
  /*
   * If the kernel was interrupted (during initialization) or the handler does
   *  not require us to reschedule we return directly to the instruction that was interrupted
   */
   
   /*get status will return 0 if all process have been loaded*/
   //if correct interupt is generated, in the case when a new process is requested, call reschedule
   if (((level != -1) || (reschedule == 0)) && (get_status() == 0))
   {
      re_schedule(state);
   }
   return_from_interrupt(state);
}

/*
 * All the descriptors inside the IDT point to the following static block of machine instructions
 * (glue code) in AT&T format:
 *   pushl  %ebp (except for exceptions 8, 10..14, 17 and 18)
 *   pushal
 *   movl   %esp, %ebx
 *   pushl  interrupt_number
 *   pushl  %ebx
 *   call   field_interrupt
 */
void initialize_jump_table() {
  int i, j;
  void (*f)(cpu_state *, int);

  f = field_interrupt;

  for (i=0; i < MAX_IDT_ENTRIES; i++) {
    j=0;
    /*
     * Skip the 'pushl %ebp' instruction if an error code is supplied
     * (exceptions 8, 10..14, 17 and 18), otherwise EBP is pushed on
     * the stack to ensure that it is always aligned. No specific reason
     * for choosing EBP, any other general purpose register would also do
     */
    if (((i < IRQ0) && (bt(0x67d00, i) == 0)) || (i >= IRQ0)) {
      /* pushl %epb */
      jump_table[i][j] = 0x55;
      j++;
    }
    /* pushal */
    jump_table[i][j] = 0x60;
    j++;
    /* movl %esp, %ebx */
    jump_table[i][j] = 0x8b;
    jump_table[i][j+1] = 0xdc;
    j = j+2;
    /* pushl interrupt_number */
    jump_table[i][j] = 0x68;
    j++;
    *(int *)(&jump_table[i][j]) = i;
    j = j+4;
    /* pushl %ebx */
    jump_table[i][j] = 0x53;
    j++;
    /* call field_interrupt */
    jump_table[i][j] = 0xe8;
    j++;
    *(int *)(&jump_table[i][j]) = (int)(f)-(int)(&jump_table[i][j+4]);
    /* ----- Update the entry inside the IDT ---------------------------------------------------- */
    i386_initialize_idt_entry(i, (int)(&jump_table[i][0]), 0);
  }
}

void initialize_8259() 
{
  /*
   * ICW1: Set bit 4 and IC4
   * The 8259 automatically enters its initialization mode if bit 4 of ICW1 is
   * set. IC4 is set to inform the controller that ICW4 should be expected during initialization
   */   
   outb(ICW1_BIT4 | ICW1_IC4,I8259_PIC0_PORTA);
   outb(ICW1_BIT4 | ICW1_IC4,I8259_PIC1_PORTA);

  /* ----- ICW2: Offset of ISRs inside the IDT (IRQ0 starts at 32 (0x20)) ----------------------- */  
   outb(IDT_OFFSET_IRQ0,I8259_PIC0_PORTB);
   outb(IDT_OFFSET_IRQ8,I8259_PIC1_PORTB);
  
  /*
   * ICW3: The primary controller uses IRQ-2 for chaining to the slave controller and ICW3
   * must select the appropriate line for the master controller. The slave selects IRQ-2 when
   * sending ICW3 to the controller
   */   
   outb(CHAIN_MASTER_IRQ,I8259_PIC0_PORTB);
   outb(CHAIN_SLAVE_IRQ,I8259_PIC1_PORTB);

  /* ----- ICW4: Select not fully nested, non-buffered, normal EOI, 8086/8088 mode -------------- */  
   outb(ICW4_uPM,I8259_PIC0_PORTB);
   outb(ICW4_uPM,I8259_PIC1_PORTB);

  /*
   * Once the ICWs have been written, the 8259A is ready to accept operation command words (OCWs).
   * We're still initializing and no interrupt handlers have been installed, so all the IRQs
   * except IRQ2 are disabled
   */
   
   outb(0xfb,I8259_PIC0_PORTB);
   outb(0xff,I8259_PIC1_PORTB);
   
}

void i8259_initialize() 
{
  int i;

  /* ----- Initialize the 8259A PIC ------------------------------------------------------------- */
  initialize_8259();
  /* ----- Mark all the entries inside the bitmap as empty -------------------------------------- */
  for (i = 0; i < (MAX_IDT_ENTRIES/32); i++)
    idt_bitmap[i] = 0;
  /* ----- Initialize the jump table ------------------------------------------------------------ */
  initialize_jump_table();
  /* ----- All interrupt handlers during initialization will return directly -------------------- */
  level = 0;
}

