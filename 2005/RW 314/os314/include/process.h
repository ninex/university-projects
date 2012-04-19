#ifndef PROCESS_H
#define PROCESS_H

#define WAIT 0
#define READY 1
#define RUNNING 2
#define INSTALL 3
#define LOADING 4
#define DONE_LOADING 5
#define NO_PROCESSES 6
          
#define load_TR(tr) \
  __asm__ volatile("ltr (%%eax)"\
           ::"a" (tr))
/*                     
#define reduce_stack() \
  __asm__ volatile(\
                   "addl  $28, %%esp\n\t "\
                   "popl %%eax \n\t"\
                   "movl %%eax,%%esp \n\t"\
           ::)
*/
#define go_kernel(stack_top)\
  __asm__ volatile(\
                    "movl %%eax,%%esp\n\t"\
                    ::"a" (stack_top))
typedef struct process_control_block{
  page_directory* pd;
  unsigned short p_state,*tr;
  cpu_state* state;
  task_state_segment* tss;
  int index,handle;
  unsigned int base;
}process_control_block;

typedef struct process_list{
  process_control_block* pcb;
  struct process_list* next;  
}process_list;

void need_screensaver();

void processes_start();

unsigned short get_status();

void install_process(process_control_block* pcb);

void kill_process();

void save_state(cpu_state* state,cpu_state* stack);

void load_state(process_control_block* pcb,cpu_state* stack);

process_list* get_current();

process_list* schedule();

void set_current();

void clear_busy(process_control_block* pcb);

#endif
