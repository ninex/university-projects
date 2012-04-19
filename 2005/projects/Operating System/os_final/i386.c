/*
 *        MODULE: i386.c
 *   DESCRIPTION: This module is an abstraction of the underlying IA-32 protected mode
 *                architecture and provides the necessary functions and data structures to
 *                interface with the various descriptor tables.
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 17.11.2004
 *     CHANGELOG:
 *                17.11.2004 - Corrected an error in the GDT that generated
 *                             #GP everytime an interrupt occurred.
 *                20.12.2003 - Ported from original Oberon implementation (flouw@cs.sun.ac.za)
 *    REFERENCES: - Tom Shanley, Protected Mode Software Architecture, Addison-Wesley
 *                - Tom Shanely, The Unabridged Pentium-4, Addison-Wesley
 *                - Crawford and Gelsinger, Programming the 80386, SYBEX
 *                - IA-32 Intel Architecture Software Developer's Manual, Volume 3: System
 *                  Programming, order number: 243192, Intel Corporation
 */
#include "include/stddef.h"
#include "include/io.h"
#include "include/asm.h"
#include "include/error.h"
#include "include/boottable.h"
#include "include/trace.h"
#include "include/i386.h"

#ifdef DEBUG_I386
  #define PRINT_386(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_386(x, ...)
#endif

/* ----- Interrupt descriptor table (IDT) ------------------------------------------------------- */
static gate_descriptor idt[MAX_IDT_ENTRIES];

/* ----- Global descriptor table (GDT) ---------------------------------------------------------- */
static segment_descriptor gdt[MAX_GDT_ENTRIES];

/* ----- Index of the first open entry inside the GDT ------------------------------------------- */
static int gdt_first;

void load_gdtr(int size, int base) {
  __asm__ volatile ("sall $16, 8(%ebp) \n\t \
                     lgdt 10(%ebp)");
}

void load_idtr(int size, int base) {
  __asm__ volatile ("sall $16, 8(%ebp) \n\t \
                     lidt 10(%ebp)");
}

void load_segment_selectors(int selector) {
  __asm__ volatile ("movw %%ax, %%ds \n\t \
                     movw %%ax, %%es \n\t" \
                    ::"a" ((long)(selector)));
}


void i386_set_page_directory(page_directory *pd) {
  __asm__ volatile ("movl %%cr3, %%eax \n\t \
                     cmp  %%ebx, %%eax \n\t \
                     je   1f \n\t \
                     movl %%ebx, %%cr3 \n\t \
                     1: \n\t" \
                    ::"b" ((long)(pd)) : "%eax" \
                   );
}

unsigned int i386_align(unsigned int value, unsigned int boundary) {
  return (value+boundary-1) & (~(boundary-1));
}

void i386_display_idt_entry(int vector) {
  printk("ofs[0:15]=%x, sel=%x, attr=%x, ofs[16:31]=%x\n",
         idt[vector].offset_0_to_15, idt[vector].selector,
         idt[vector].attributes, idt[vector].offset_16_to_31);
}

void i386_initialize_idt_entry(int vector, int address, unsigned char p_level) {
  if ((vector < 0) || (vector >= MAX_IDT_ENTRIES))
    PRINT_386("[i386_initialize_idt_entry]: vector (%d) out of bounds\n", vector);
  else {
    idt[vector].offset_0_to_15 = (short)(address % 0x10000);
    idt[vector].selector = KERNEL_CODE_SEGMENT_SELECTOR;
    if (p_level == 1)
      idt[vector].attributes = (short)(SEG_PRESENT|SEG_DPL3|SYS_SEG_INTERRUPT_GATE_386);
    else
      idt[vector].attributes = (short)(SEG_PRESENT|SEG_DPL0|SYS_SEG_INTERRUPT_GATE_386);
    idt[vector].offset_16_to_31 = (short)(address/0x10000);
  }
}

int i386_allocate_GDT_entry(int *entry) {
  if (gdt_first == -1)
    return -ERR_I386_GDT_FULL;
  else {
    *entry = gdt_first;
    gdt_first = gdt[*entry].limit_0_to_15;
    return OK;
  }
}

void i386_release_GDT_entry(int entry) {
  gdt[entry].limit_0_to_15 = gdt_first;
  gdt_first = entry;
}

int i386_initialize_tss(task_state_segment *tss, unsigned short *tr) {
  int i;

  if (tss == NULL) {
    PRINT_386("[i386_initialize_tss] NULL tss\n");
    return -ERR_I386_INVALID_TSS;
  }
  if (i386_allocate_GDT_entry(&i) != OK) {
    PRINT_386("[i386_initialize_tss] could not allocate a GDT entry\n");
    *tr = -1;
    return -ERR_I386_GDT_FULL;
  }

  /* ----- Initialize the TSS ------------------------------------------------------------------- */
  fill_4(tss, 0, sizeof(task_state_segment)/4);
  tss->ss0 = KERNEL_STACK_SEGMENT_SELECTOR;
  tss->esp0 = bt_entry(BT_KERNEL_STACK_TOP);
  tss->io_bitmap_offset = -1;
  /* ----- Calculate the selector value that should be used by LTR ------------------------------ */
  *tr = (i*8) | SELECTOR_DPL0;
  /* ----- Initialize the descriptor in the GDT ------------------------------------------------- */
  gdt[i].limit_0_to_15 = (unsigned short)((sizeof(task_state_segment)-1) % 0x10000);
  gdt[i].base_0_to_15 = (unsigned short)((unsigned int)(tss) % 0x10000);
  gdt[i].base_16_to_23 = (unsigned char)(((unsigned int)(tss)/0x10000) % 0x100);
  gdt[i].base_24_to_31 = (unsigned char)((unsigned int)(tss)/0x1000000);
  //gdt[i].access = (unsigned char)(SEG_PRESENT | SEG_DPL0 | SYS_SEG_AVAILABLE_TSS_386);
  gdt[i].access = 0x89;
  gdt[i].granularity = (((sizeof(task_state_segment)-1)/0x10000) % 0x10) | SEG_GRANULARITY_BYTES;
  PRINT_386("[i386_initialize_tss] %#.16llx\n", gdt[i]);
  return OK;
}

void i386_set_cpu_state(cpu_state *state, unsigned int esp, unsigned int eip) {
  if (state != NULL) {
    fill_4(state, 0, sizeof(cpu_state)/4);
    state->cs = PROCESS_CODE_SEGMENT_SELECTOR;
    state->user_ss = PROCESS_STACK_SEGMENT_SELECTOR;
    state->user_esp = esp;
    state->eip = eip;
    state->eflags = 0x204;
  }
}

unsigned int i386_gdt_offset() {
  return (unsigned int)(&gdt);
}

void i386_initialize_descriptor_tables() {
  int i;
  int gdt_address;

  gdt_address = (int)(&gdt[0]);

  /* ----- Entry 0: NULL segment descriptor ----------------------------------------------------- */
  writel(0, gdt_address);
  writel(0, gdt_address+4);

  /*
   * Entry 1: Kernel code segment - Base 0, Limit FFFFF (4G), read/write,
   * not accessed, non-conforming, present, privilege level 0,
   * 4K granularity, 32-bit code
   * 0   0    C   F    9   A    0   0    0   0    0   0    F   F    F   F
   * 00000000 11001111 10011010 00000000 00000000 00000000 11111111 11111111
   */
  writel(0x0000ffff, gdt_address+8);
  writel(0x00cf9a00, gdt_address+12);

  /*
   * Entry 2: Kernel data segment - Base 0, Limit FFFFF (4G), read/write,
   * not accessed, expand-up, present, privilege level 0, 4K granularity,
   * 32-bit data
   * 0   0    C   F    9   2    0   0    0   0    0   0    F   F    F   F
   * 00000000 11001111 10010010 00000000 00000000 00000000 11111111 11111111
   */
  writel(0x0000ffff, gdt_address+16);
  writel(0x00cf9200, gdt_address+20);

  /*
   * Entry 3: Process code segment - Base 0, Limit FFFFF (4G), read/write,
   * not accessed, not-conforming, present, privilege level 3, 4K granularity,
   * 32-bit code
   * 0   0    C   F    F   A    0   0    0   0    0   0    F   F    F   F
   * 00000000 11001111 11111010 00000000 00000000 00000000 11111111 11111111
   */
   writel(0x0000ffff, gdt_address+24);
   writel(0x00cffa00, gdt_address+28);

  /*
   * Entry 4: Process data segment: Base 0, Limit FFFFF(4G), read/write,
   * not accessed, data segment, present, privilege level 3, 4K granularity,
   * 32-bit data
   * 0   0    C   F    F   2    0   0    0   0    0   0    F   F    F   F
   * 00000000 11001111 11110010 00000000 00000000 00000000 11111111 11111111
   */
  writel(0x0000ffff, gdt_address+32);
  writel(0x00cff200, gdt_address+36);

  /*
   * The number of entries in the GDT can grow and shrink, typically when new
   * tasks are created or old tasks are removed (every task is assigned a TSS
   * descriptor). Allocating and deallocating entries require that some
   * structure is maintained to facilitate these operations. This is done by
   * using the 'limit_0_to_15' field of every empty GDT entry as a 'next' field
   * pointing to the following open GDT entry (effectively creating a linked
   * list). The linked list is terminated by storing a value of -1 in the last
   * entry's 'limit_0_to_15' field.
   */
  for (i = 5; i < (MAX_GDT_ENTRIES-1); i++) {
    gdt[i].access = 0;
    gdt[i].limit_0_to_15 = (short)(i+1);
  }
  gdt_first = 5;
  gdt[MAX_GDT_ENTRIES-1].limit_0_to_15 = -1;

  /*
   * Load the IDT and GDT registers with the descriptor table addresses and re-initialize
   * the segment selectors. This is required since up to this point the empty IDT and GDT
   * created by the bootstrap loader were in use.
   */
  load_idtr(((int)((MAX_IDT_ENTRIES*sizeof(gate_descriptor))-1)), ((int)(&(idt[0]))));
  load_gdtr(((int)((MAX_GDT_ENTRIES*sizeof(segment_descriptor))-1)), ((int)(&(gdt[0]))));
  load_segment_selectors(DATA_SEGMENT_SELECTOR);
}
