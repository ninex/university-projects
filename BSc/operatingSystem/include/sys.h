#ifndef SYS_H
#define SYS_H

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
#define SYS_COMMS_CLOSE       0x69

/* ----- Miscellaneous system call function numbers --------------------------------------------- */
#define SYS_EXIT              0x100

void sys_initialize();

#endif
