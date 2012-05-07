#ifndef SYS_CALLS_H
#define SYS_CALLS_H

/* ----- System call function numbers for tty --------------------------------------------------- */
#define SYS_TTY_OPEN          0x00
#define SYS_TTY_CLOSE         0x01
#define SYS_TTY_PRINT         0x02
#define SYS_TTY_SET_ATTRIBUTE 0x03
#define SYS_TTY_GET_ATTRIBUTE 0x04
#define SYS_TTY_CLEAR_DISPLAY 0x05
#define SYS_TTY_GOTOXY        0x06
#define SYS_TTY_KEY_AVAILABLE 0x40
#define SYS_TTY_READ_KEY      0x41

/* ----- System call function numbers for comms ------------------------------------------------- */
#define SYS_COMMS_LOOKUP_PORT 0x64
#define SYS_COMMS_CREATE_PORT 0x65
#define SYS_COMMS_SEND        0x66
#define SYS_COMMS_RECEIVE     0x67
#define SYS_COMMS_REPLY       0x68
#define SYS_COMMS_CLOSE_PORT  0x69

/* ----- Miscellaneous system call function numbers --------------------------------------------- */
#define SYS_EXIT              0x100

/* ----- Macro for system calls with no parameters, no return value ----------------------------- */
#define sys_0(sys_call_no) ({ \
  __asm__ volatile ("int $255\n\t" \
                    ::"a" (sys_call_no) \
                   ); \
})

/* ----- Macro for system calls with 1 parameter, no return value ------------------------------- */
#define sys_1(sys_call_no, par) ({ \
  __asm__ volatile ("int $255\n\t" \
                    ::"a" (sys_call_no), "b" (par) \
                   ); \
})

/* ----- Macro for system calls with 2 parameters, no return value ------------------------------ */
#define sys_2(sys_call_no, par1, par2) ({ \
  __asm__ volatile ("int $255\n\t" \
                    ::"a" (sys_call_no), "b" (par1), "c" (par2) \
                   ); \
})

/* ----- Macro for system calls with 3 parameters, no return value ------------------------------ */
#define sys_3(sys_call_no, par1, par2, par3) ({ \
  __asm__ volatile ("int $255\n\t" \
                    ::"a" (sys_call_no), "b" (par1), "c" (par2), "d" (par3) \
                   ); \
})

/* ----- Macro for system calls with no parameters and return value ----------------------------- */
#define _sys_0(sys_call_no) ({ \
  int _result; \
  __asm__ volatile ("int $255\n\t"\
                    :"=a" (_result):"a" (sys_call_no) \
                   ); \
  _result; \
})

/* ----- Macro for system calls with 1 parameter and return value ------------------------------- */
#define _sys_1(sys_call_no, par) ({ \
  int _result; \
  __asm__ volatile ("int $255\n\t"\
                    :"=a" (_result):"a" (sys_call_no), "b" (par) \
                   ); \
  _result; \
})

/* ----- Macro for system calls with 2 parameters and return value ------------------------------- */
#define _sys_2(sys_call_no, par1, par2) ({ \
  int _result; \
  __asm__ volatile ("int $255\n\t"\
                    :"=a" (_result):"a" (sys_call_no), "b" (par1), "c" (par2) \
                   ); \
  _result; \
})

/* ----- Macro for system calls with 3 parameters and return value ------------------------------- */
#define _sys_3(sys_call_no, par1, par2, par3) ({ \
  int _result; \
  __asm__ volatile ("int $255\n\t"\
                    :"=a" (_result):"a" (sys_call_no), "b" (par1), "c" (par2), "d" (par3) \
                   ); \
  _result; \
})

/* ----- Macro for system calls with 5 parameters and return value ------------------------------- */
#define _sys_5(sys_call_no, par1, par2, par3, par4, par5) ({ \
  int _result; \
  __asm__ volatile ("int $255\n\t"\
                    :"=a" (_result):"a" (sys_call_no), "b" (par1), "c" (par2), "d" (par3), \
                     "S" (par4), "D" (par5) \
                   ); \
  _result; \
})

#endif
