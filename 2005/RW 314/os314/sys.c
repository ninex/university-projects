#include "include/error.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/tty.h"
#include "include/process.h"
#include "include/comms.h"
#include "include/sys.h"

unsigned char sys_handler(cpu_state *state, int n) {
  unsigned char reschedule;

  /* ----- Per default we assume that we won't reschedule --------------------------------------- */
  reschedule = 0;
  switch (state->eax) {
    /* ----- 'tty.c' ----------------------------------------------------------------------------*/
    case SYS_TTY_OPEN:
      state->eax = tty_open();
      break;
    case SYS_TTY_CLOSE:
      tty_close(state->ebx);
      break;
    case SYS_TTY_PRINT:
      tty_print(state->ebx, (char *)(state->ecx));
      break;
    case SYS_TTY_SET_ATTRIBUTE:
      tty_set_attribute(state->ebx, (unsigned char)(state->ecx), (unsigned char)(state->edx));
      break;
    case SYS_TTY_GET_ATTRIBUTE:
      state->eax = (int)(tty_get_attribute(state->ebx));
      break;
    case SYS_TTY_CLEAR_DISPLAY:
      tty_clear_display(state->ebx);
      break;
    case SYS_TTY_GOTOXY:
      tty_gotoxy(state->ebx, state->ecx, state->edx);
      break;
    case SYS_TTY_KEY_AVAILABLE:
      state->eax = (int)(tty_key_available(state->ebx));
      break;
    case SYS_TTY_READ_KEY:
      state->eax = (int)(tty_read_key(state->ebx));
      break;
    /* ----- 'comms.c' -------------------------------------------------------------------------- */
    case SYS_COMMS_LOOKUP_PORT:
      state->eax = comms_lookup_port(state->ebx);
      break;
    case SYS_COMMS_CREATE_PORT:
      state->eax = comms_create_port(state->ebx);
      break;
    case SYS_COMMS_SEND:
      state->eax = comms_send(state->ebx,(void*)state->ecx,state->edx,(void*)state->esi,state->edi);
      break;
    case SYS_COMMS_RECEIVE:
      state->eax = comms_receive(state->ebx,(void*)state->ecx,state->edx);
      break;
    case SYS_COMMS_REPLY:
      state->eax = comms_reply(state->ebx,(void*)state->ecx,state->edx);
      break;
    case SYS_COMMS_CLOSE:
      state->eax = comms_close_port(state->ebx);
      break;

    case SYS_EXIT:        
        kill_process();        
        reschedule = 1;
      break;

    default:
      printk("[sys_handler]: Invalid system call\n");
      break;
  }

  return reschedule;
}

void sys_initialize() {
  /* ----- Note: The system call handler must set its DPL field to 3 ---------------------------- */
  if (i8259_install_interrupt_handler(0xff, sys_handler, 1) == OK)
    printk("[sys_initialize] System call handler successfully installed\n");
  else
    printk("[sys_initialize] Installation of interrupt handler failed!\n");
}
