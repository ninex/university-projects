/*
 *   DESCRIPTION: This module provides access to various x86 machine
 *                instructions and registers through macro's. Most of the macro's
 *                act as inline functions, always returning their results in EAX,
 *                AX or AL
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 18.12.2004
 *     CHANGELOG:
 *                18.12.2004 - Added copy_4 and fill_2 macro's
 *                14.12.2004 - Added disable_interrupts and restore_flags macro
 *                13.12.2004 - Added enable_paging macro
 *                21.11.2004 - Added div_64 to perform 64-bit division
 *                15.09.2004 - Added bit manipulation macro's for BT, BTS and
 *                             BTR instructions
 *                27.05.2004 - Macro's added to retrieve registers used by
 *                             exception handler
 *                25.05.2004 - Original implementation
 */
#ifndef ASM_H
#define ASM_H

#ifndef __GNUC__
#define __asm__ asm
#endif


/* MY ASM ROUTINES*/
#define load_TR(tr) \
  __asm__ volatile("ltr %%ax"\
           ::"a" (tr))

#define go_kernel(stack_top)\
  __asm__ volatile(\
                    "movl %%eax,%%esp\n\t"\
                    ::"a" (stack_top))
                    
#define delay(n)\
__asm__ volatile ("movl $1,%%eax\n\t"\
                  "L1_%=:\n\t"\
                  "cmp %%eax,%%ecx\n\t"\
                  "je L2_%=\n\t"\
                  "nop\n\t"\
                  "inc %%eax\n\t"\
                  "jmp L1_%=\n\t"\
                  "L2_%=:\n\t"\
                  :: "c" (n*10000)\
)


/*
 * Returns the bit position of the least significant bit set to 1 in 'value' or -1 if
 * 'value' is 0
 */
#define bsf(value) ({ \
  int _result; \
  __asm__ volatile ("bsf %%ebx, %%eax\n\t \
                     jnz 1f \n\t \
                     movl $-1, %%eax \n\t \
                     1:" \
                    :"=a" (_result):"b" (value) \
                   ); \
  _result; \
})

/* ----- Returns 1 if bit 'n' in 'value' is set, otherwise 0 ------------------------------------ */
#define bt(value, n) ({ \
  int _result; \
  __asm__ volatile ("xorl %%eax, %%eax\n\t \
                     bt   %%edx, %%ebx\n\t \
                     setc %%al\n\t" \
                    :"=a" (_result):"b" (value), "d" (n) \
                   ); \
  _result; \
})

/* ----- Sets bit 'n' of 'value' to 1 ----------------------------------------------------------- */
#define bts(value, n) ({ \
  __asm__ volatile ("bts %%edx, %%eax\n\t" \
                    :"=a" (value):"a" (value), "d" (n)\
                   );\
})

/* ----- Clears bit 'n' of 'value' -------------------------------------------------------------- */
#define btr(value, n) ({ \
  __asm__ volatile ("btr %%edx, %%eax\n\t"\
                    :"=a" (value):"a" (value), "d" (n)\
                   );\
})

/* ----- Rotates 'value' left by 'n' bit positions ---------------------------------------------- */
#define rol(value, n) ({ \
  unsigned int _x; \
  __asm__ volatile ("roll %%cl, %%eax":"=a" (_x):"a" (value), "c" (n)); \
  _x; \
})

/* ----- Rotates 'value' right by 'n' bit positions --------------------------------------------- */
#define ror(value, n) ({ \
  unsigned int _x; \
  __asm__ volatile ("rorl %%cl, %%eax" \
                    :"=a" (_x) \
                    :"a" (value), "c" (n)); \
  _x; \
})

/* ------ Returns the contents of the EFLAGS register ------------------------------------------- */
#define eflags() ({ \
  unsigned int _eflags; \
  __asm__ volatile ("pushfl \n \
                     pop %%eax" \
                    :"=a" (_eflags)); \
  _eflags; \
})

/* ----- Returns the contents of control register CR0 ------------------------------------------- */
#define cr0() ({ \
  unsigned int _cr0; \
  __asm__ volatile ("movl %%cr0, %%eax" \
                    :"=a" (_cr0)); \
  _cr0; \
})

/* ----- Returns the contents of control register CR2 ------------------------------------------- */
#define cr2() ({ \
  unsigned int _cr2; \
  __asm__ volatile ("movl %%cr2, %%eax" \
                    :"=a" (_cr2)); \
  _cr2; \
})

/* ----- Returns the contents of control register CR3 ------------------------------------------- */
#define cr3() ({ \
  unsigned int _cr3; \
  __asm__ volatile ("movl %%cr3, %%eax" \
                    :"=a" (_cr3)); \
  _cr3; \
})

/* ----- Returns the contents of the CS register ------------------------------------------------ */
#define cs() ({ \
  unsigned int _cs; \
  __asm__ volatile ("xorl %%eax, %%eax\n movw %%cs, %%ax" \
                    :"=a" (_cs)); \
  _cs; \
})

/* ----- Returns the contents of the DS register ------------------------------------------------ */
#define ds() ({ \
  unsigned int _ds; \
  __asm__ volatile ("xorl %%eax, %%eax\n movw %%ds, %%ax" \
                    :"=a" (_ds)); \
  _ds; \
})

/* ----- Returns the contents of the ES register ------------------------------------------------ */
#define es() ({ \
  unsigned int _es; \
  __asm__ volatile ("xorl %%eax, %%eax\n movw %%es, %%ax" \
                    :"=a" (_es)); \
  _es; \
})

/* ----- Returns the contents of the FS register ------------------------------------------------ */
#define fs() ({ \
  unsigned int _fs; \
  __asm__ volatile ("xorl %%eax, %%eax\n movw %%fs, %%ax" \
                    :"=a" (_fs)); \
  _fs; \
})

/* ----- Returns the contents of the GS register ------------------------------------------------ */
#define gs() ({ \
  unsigned int _gs; \
  __asm__ volatile ("xorl %%eax, %%eax\n movw %%gs, %%ax" \
                    :"=a" (_gs)); \
  _gs; \
})

/* ----- Returns the contents of the SS register ------------------------------------------------ */
#define ss() ({ \
  unsigned int _ss; \
  __asm__ volatile ("xorl %%eax, %%eax\n movw %%ss, %%ax" \
                    :"=a" (_ss)); \
  _ss; \
})

/* ----- Returns the contents of the ESP register ----------------------------------------------- */
#define esp() ({ \
  unsigned int _esp; \
  __asm__ volatile ("movl %%esp, %%eax" \
                    :"=a" (_esp)); \
  _esp; \
})

/* ----- Returns the contents of the EBP register ----------------------------------------------- */
#define ebp() ({ \
  unsigned int _ebp; \
  __asm__ volatile ("movl %%ebp, %%eax" \
                    :"=a" (_ebp)); \
  _ebp; \
})

/*
 * Macro to perform 64-bit division (include/asm-i386/div64.h - Linux kernel)
 */
#define div_64(n, base) ({ \
  unsigned long __upper; \
  unsigned long __low; \
  unsigned long __high; \
  unsigned long __mod; \
  __asm__("":"=a" (__low), "=d" (__high):"A" (n)); \
  __upper = __high; \
  if (__high) { \
    __upper = __high % (base); \
    __high = __high / (base); \
  } \
  __asm__("divl %2":"=a" (__low), "=d" (__mod):"rm" (base), "0" (__low), "1" (__upper)); \
  __asm__("":"=A" (n):"a" (__low),"d" (__high)); \
  __mod; \
})

/*
 * Macro to enable the paging mechanism of the CPU. The original value of CR0 is first
 * loaded into EAX (its default value differs between processor members i386, i486, etc.)
 * before setting the paging enable (PE) bit. A JMP instruction is also executed to
 * serialize the CPU.
 */
#define enable_paging(pd_base) ({ \
  __asm__ volatile ("  movl %%eax, %%cr3 \n\t \
                       movl %%cr0, %%eax \n\t \
                       orl  $0x80000000, %%eax \n\t \
                       movl %%eax, %%cr0 \n\t \
                       jmp  1f \n\t \
                     1: \n\t" \
                    :: "a" (pd_base) \
                   ); \
})

/*
 * Macro to disable all hardware interrupts and return the contents of the EFLAGS
 * register before setting IF=0
 */
#define disable_interrupts() ({ \
  unsigned int _eflags; \
  __asm__ volatile ("pushfl \n\t \
                     cli \n\t \
                     popl %%eax" \
                    : "=a" (_eflags) \
                   ); \
  _eflags; \
})

/*
 * Macro to enable all hardware interrupts and return the contents of the EFLAGS
 * register before setting IF=1
 */
#define enable_interrupts() ({ \
  unsigned int _eflags; \
  __asm__ volatile ("pushfl \n\t \
                     sti \n\t \
                     popl %%eax" \
                    : "=a" (_eflags) \
                   ); \
  _eflags; \
})

/*
 * Restore the EFLAGS register. 'eflags' should be set to the value returned
 * by 'disable_interrupts' or 'enable_interrupts'
 */
#define restore_flags(eflags) ({ \
  __asm__ volatile ("push %%eax\n\t popfl" \
                    ::"a" ((long)(eflags))\
                   ); \
})

/*
 * Copies the 16-bit 'value', 'n' times into memory, starting at the specified address
 */
#define fill_2(address, value, n) ({ \
  __asm__ volatile ("cld \n\t \
                     rep\n\t stosw" \
                     ::"D" ((long)(address)), "a" ((long)(value)), "c" ((long)(n)) \
                   ); \
})

/*
 * Copies the 32-bit 'value', 'n' times into memory, starting at the specified address
 */
#define fill_4(address, value, n) ({ \
  __asm__ volatile ("cld \n\t \
                     rep\n\t stosl" \
                     ::"D" ((long)(address)), "a" ((long)(value)), "c" ((long)(n)) \
                   );\
})

/*
 * Copies 'n' 32-bit words of data from the address specified in 'src' to the address
 * specified in 'des'
 */
#define copy_4(src, des, n) ({ \
__asm__ volatile ("cld\n\t rep\n\t movsl" \
                  ::"D" ((long)(des)), "S" ((long)(src)), "c" ((long)(n)) \
                 ); \
})

/*
 * Copies 'n' bytes of data from the address specified in 'src' to the address specified
 * in 'des'
 */
#define copy(src, des, n) ({ \
void * _dest = des; \
void * _source = src; \
__asm__ volatile ("cld\n\t rep\n\t movsb" \
                   ::"D" ((long)(_dest)), "S" ((long)(_source)), "c" ((long)(n)) \
                 ); \
})

#endif
