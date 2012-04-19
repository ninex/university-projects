#ifndef ASM_H
#define ASM_H

#ifndef __GNUC__
#define __asm__ asm
#endif

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
