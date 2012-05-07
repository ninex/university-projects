#ifndef ASM_H
#define ASM_H

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
#endif
