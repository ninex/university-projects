#ifndef PROCESS_H
#define PROCESS_H

#define BLOCKED 0
#define READY 1
#define RUNNING 2
#define INSTALL 3
#define LOADING 4
#define DONE_LOADING 5
#define NO_PROCESSES 6
          
//struct for the pcb                    
typedef struct process_control_block{
  page_directory* pd;
  unsigned short p_state,tr;
  cpu_state* state;
  task_state_segment* tss;
  int index,handle,end_index;
  unsigned int base;
}process_control_block;

//struct for a process list used by ready queue
typedef struct process_list{
  process_control_block* pcb;
  struct process_list* next;  
  struct process_list* bq_next;  
}process_list;

//start processes
void processes_start();

//get the status of processes
unsigned short get_status();

//install a process and put it in the ready queue
void install_process(process_control_block* pcb);

//kill a process
void kill_process();

//save a process context info
void save_state(cpu_state* state,cpu_state* stack);

//load a process context info
void load_state(process_control_block* pcb,cpu_state* stack);

//remove a running process
process_list* remove_running(cpu_state* state);

//add a blocked process to the ready queue
void add_to_rq(process_list* last);

//get the current running process
process_list* get_current();

//get the process to schedule next
process_list* schedule();

//set the running process to the first process in the ready queue
void set_current();

//clear the busy bit
void clear_busy(process_control_block* pcb);

#endif
