#ifndef I386_H
#define I386_H

/* ----- Number of segment descriptors for GDT and IDT ------------------------------------------ */
#define MAX_IDT_ENTRIES 256
#define MAX_GDT_ENTRIES 256

#define SELECTOR_DPL0 0
#define SELECTOR_DPL1 1
#define SELECTOR_DPL2 2
#define SELECTOR_DPL3 3

#define CODE_SEG            0x08
#define CODE_SEG_ACCESSED   0x01
#define CODE_SEG_DATA_READ  0x02
#define CODE_SEG_CONFORMING 0x04

#define DATA_SEG             0x00
#define DATA_SEG_ACCESSED    0x01
#define DATA_SEG_WRITE       0x02
#define DATA_SEG_EXPAND_DOWN 0x04

#define SEG_PRESENT 0x8000
#define SEG_DPL0    0x0000
#define SEG_DPL1    0x2000
#define SEG_DPL2    0x4000
#define SEG_DPL3    0x6000

#define SEG_GRANULARITY_BYTES 0x0000
#define SEG_GRANULARITY_PAGES 0x8000

/* ----- System segment descriptor types -------------------------------------------------------- */
#define SYS_SEG_AVAILABLE_TSS_286  0x0100
#define SYS_SEG_LDT                0x0200
#define SYS_SEG_BUSY_TSS_286       0x0300
#define SYS_SEG_CALL_GATE_286      0x0400
#define SYS_SEG_TASK_GATE          0x0500
#define SYS_SEG_INTERRUPT_GATE_286 0x0600
#define SYS_SEG_TRAP_GATE_286      0x0700
#define SYS_SEG_AVAILABLE_TSS_386  0x0900
#define SYS_SEG_BUSY_TSS_386       0x0b00
#define SYS_SEG_CALL_GATE_386      0x0c00
#define SYS_SEG_INTERRUPT_GATE_386 0x0e00
#define SYS_SEG_TRAP_GATE_386      0x0f00

/* ----- Segment selectors ---------------------------------------------------------------------- */
#define NULL_SELECTOR                  SELECTOR_DPL0
#define KERNEL_CODE_SEGMENT_SELECTOR   1*8+SELECTOR_DPL0
#define KERNEL_STACK_SEGMENT_SELECTOR  2*8+SELECTOR_DPL0
#define PROCESS_CODE_SEGMENT_SELECTOR  3*8+SELECTOR_DPL3
#define PROCESS_STACK_SEGMENT_SELECTOR 4*8+SELECTOR_DPL3
#define DATA_SEGMENT_SELECTOR          4*8+SELECTOR_DPL0

/* ----- Size in bytes for a single page -------------------------------------------------------- */
#define PAGE_SIZE 4096

/* ----- Kernel stack spans 4 pages, excluding the two NULL pages ------------------------------- */
#define KERNEL_STACK_SIZE (4*PAGE_SIZE)

/* ----- Number of entries in a single page table or page directory ----------------------------- */
#define PTES (PAGE_SIZE/4)

/* ----- Page table attributes ------------------------------------------------------------------ */
#define PT_MASK        0xfffff000
#define PT_PRESENT     0x001
#define PT_READ_WRITE  0x002
#define PT_USER        0x004

/* ----- Page directory attributes -------------------------------------------------------------- */
#define PD_MASK        0xfffff000
#define PD_PRESENT     0x001
#define PD_READ_WRITE  0x002
#define PD_USER        0x004

/* ----- Predefined page table/directory attributes --------------------------------------------- */
#define PAGE_MASK        0xfffff000
#define PAGE_NOT_PRESENT 0x000
#define PAGE_PRESENT     0x001
#define PAGE_RW          0x002
#define PAGE_USER        0x004
#define KERNEL_PAGE      (PAGE_PRESENT | PAGE_RW)
#define USER_PAGE        (PAGE_PRESENT | PAGE_RW | PAGE_USER)

/* ----- 4K page directory ---------------------------------------------------------------------- */
typedef struct page_directory {
  unsigned int table[PTES];
} page_directory;

/* ----- 4K page table -------------------------------------------------------------------------- */
typedef struct page_table {
  unsigned int entry[PTES];
} page_table;

/* ----- General segment descriptor for GDT entries --------------------------------------------- */
typedef struct segment_descriptor {
  unsigned short limit_0_to_15;
  unsigned short base_0_to_15;
  unsigned char base_16_to_23;
  unsigned char access;
  unsigned char granularity;
  unsigned char base_24_to_31;
} segment_descriptor;

/* ----- Gate descriptor for IDT entries -------------------------------------------------------- */
typedef struct gate_descriptor {
  short offset_0_to_15;
  short selector;
  short attributes;
  short offset_16_to_31;
} gate_descriptor;

/* ----- Minimum Task State Segment (TSS) structure --------------------------------------------- */
typedef struct task_state_segment {
  short link, pad0;
  int esp0;
  short ss0, pad1;
  int esp1;
  short ss1, pad2;
  int esp2;
  short ss2, pad3;
  page_directory *cr3;
  int eip;
  int eflags;
  int eax;
  int ecx;
  int edx;
  int ebx;
  int esp;
  int ebp;
  int esi;
  int edi;
  short es, pad4;
  short cs, pad5;
  short ss, pad6;
  short ds, pad7;
  short fs, pad8;
  short gs, pad9;
  short ldt, pad10;
  short attributes;
  short io_bitmap_offset;
} task_state_segment;

/* ----- Context information for the kernel/user-level process ---------------------------------- */
typedef struct cpu_state {
  int edi;
  int esi;
  int ebp;
  int kernel_esp;
  int ebx;
  int edx;
  int ecx;
  int eax;
  int error_code;      /* Only valid for exceptions with error codes */
  int eip;
  int cs;
  unsigned int eflags;
  int user_esp;        /* Pushed automatically during a stack switch */
  int user_ss;         /* Pushed automatically during a stack switch */
} cpu_state;


void i386_release_GDT_entry(int entry);
/*
 * DESCRIPTION: Aligns a value on a boundary
 *       INPUT: value - The value that must be aligned.
 *              boundary - The boundary to which 'value' must be
 *                         aligned.
 *      OUTPUT: Returns the aligned value or 0 if 'value' = 0;
 */
unsigned int i386_align(unsigned int value, unsigned int boundary);

/*
 * DESCRIPTION: Initializes a gate descriptor for the specified interrupt vector inside the IDT.
 *              This routine should not be called directly by device drivers or system call
 *              libraries to install a handler - 'i8259_install_interrupt_handler'
 *              should be used instead
 *       INPUT: vector - Vector number in the IDT
 *              address - 32-bit offset of the interrupt handler within the
 *                        linear address space (kernel code segment)
 *              p_level - When set to 1, the DPL field of the descriptor
 *                        is set to 3, otherwise it is set to 0. The field
 *                        is typically set to 1 when installing interrupt
 *                        handlers for system calls to allow user-level
 *                        processes access to the kernel. Device drivers inside the
 *                        kernel should set this parameter to 0
 *      OUTPUT: NONE
 */
void i386_initialize_idt_entry(int vector, int address, unsigned char p_level);

/*
 * DESCRIPTION: Initializes the GDT and IDT. This procedure overrides the
 *              initial GDT that was created by the bootstrap loader. A pseudo
 *              linked list is also created to facilitate the insertion and
 *              removal of segment descriptors within the GDT. The GDTR and
 *              IDTR registers are also reloaded, along with other segment selectors
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void i386_initialize_descriptor_tables();

/*
 * DESCRIPTION: Loads a new value into the page directory base address register (CR3).
 *              CR3 is not loaded if its existing value corresponds with the new
 *              value.
 *       INPUT: pd - A pointer containing the linear address of the new page directory
 *      OUTPUT: NONE
 */
void i386_set_page_directory(page_directory *pd);

/*
 * DESCRIPTION: Initializes the TSS and creates a TSS descriptor inside the GDT
 *       INPUT: tss - A non-NULL pointer to a task_state_segment.
 *              tr - A pointer to a 16-bit value
 *      OUTPUT: Returns OK if successfull. tr contains the segment selector
 *              that should be used to select the TSS descriptor from the
 *              GDT when executing the LTR instruction
 */
int i386_initialize_tss(task_state_segment *tss, unsigned short *tr);

/*
 * DESCRIPTION: Returns the linear base address of the GDT (&gdt[0])
 *       INPUT: NONE
 *      OUTPUT: Returns the linear base address of the GDT
 */
unsigned int i386_gdt_offset();

/*
 * DESCRIPTION: Initializes the context information for a specific user level
 *              process. All fields are set to 0, except for cs, eip, user_ss
 *              and user_esp
 *       INPUT: state - pointer to the context information
 *              esp - virtual address for the top of stack (user_esp)
 *              eip - entry point for the process (eip)
 *      OUTPUT: NONE
 */
void i386_set_cpu_state(cpu_state *state, unsigned int esp, unsigned int eip);

/*
 * DESCRIPTION: Displays the contents of a the specified gate descriptor from the IDT
 *       INPUT: vector - Vector number in the IDT that should be displayed
 *      OUTPUT: NONE
 */
void i386_display_idt_entry(int vector);

#endif
