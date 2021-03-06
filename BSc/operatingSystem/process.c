#include "include/asm.h"
#include "include/i386.h"
#include "include/boottable.h"
#include "include/exceptions.h"
#include "include/trace.h"
#include "include/stddef.h"
#include "include/mem.h"
#include "include/tty.h"
#include "include/i8253.h"
#include "include/random.h"
#include "include/process.h"

#ifdef DEBUG_PROCESS
  #define PRINT_PROC(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_PROC(x, ...)
#endif
                        
static process_list *rq = NULL, *rq_last = NULL, *runq = NULL;
static unsigned int status = LOADING;
static segment_descriptor* gdt;
             
void processes_start()
{  
  //get the gdt from i386
  gdt  = (segment_descriptor*)i386_gdt_offset();
    
  //we're done loading
  status = DONE_LOADING;  
}

unsigned short get_status()
{
  //return load status
  return status;
}


void install_process(process_control_block* pcb)
{
  //is this the first process we install
  if (rq == NULL)
  {
    //allocate memory for node, it's the start of the ready queue
    rq = (struct process_list*)mem_alloc(sizeof(struct process_list));
    //set it's pcb
    rq->pcb = pcb;
    //doesn't have a next
    rq->next = NULL;    
    rq->bq_next = NULL;
    //it's also the last one
    rq_last = rq;
  }else
  {
    //alocate mem for new node
    process_list* temp = (struct process_list*)mem_alloc(sizeof(struct process_list));
    //set it's pcb
    temp->pcb = pcb;
    //doesn't have a next
    temp->next = NULL;
    temp->bq_next = NULL;
    //the last node in ready queue points to it
    rq_last->next = temp;
    //it's now the last node
    rq_last = temp;
  }  
  //it's ready to state executing
  pcb->p_state = READY;
  //compute it's index in the gdt
  pcb->index = pcb->tr >> 3;
}


void save_state(cpu_state* state,cpu_state* stack)
{
  //save the context info from the stack
  copy_4(stack,state,sizeof(struct cpu_state)/4);
}

void load_state(process_control_block* pcb,cpu_state* stack)
{
  //load context info onto stack
  if (pcb->p_state == RUNNING)
  {
    //we need all the registers
    copy_4(pcb->state,stack,sizeof(struct cpu_state)/4);
  }else
  {
    //first time the context info is needed
    //we only need ss,user_esp,eip,cs and eflags
    copy_4((unsigned int)pcb->state + 36,stack,20/4);
  }
}


process_list* remove_running(cpu_state* state)
{
  
  if (runq != NULL)
  {
    //save the context info
    save_state(runq->pcb->state,state);
    
    //set runq to a temp var
    process_list* temp = runq;
  
    //no more running process
    runq = NULL;
  
    //return the process that was running
    return temp;
  }
  return NULL;
}

void add_to_rq(process_list* insert)
{  
  //is there a ready queue?
  if (rq == NULL)
  {      
    //add to front of ready queue
    rq = insert;
  }else
  {
    //add to back of ready queue
    rq_last->next = insert;
  }
  //it is the last node in the ready queue
  rq_last = insert;
  rq_last->next = NULL;
}

process_list* get_current()
{  
  //is there any running processes
  if ((runq == NULL) || (status != DONE_LOADING))
  {
    //nope
    return NULL;
  }else
  {
    //return the running process
    return runq;
  }
}

process_list* schedule()
{
  //any processes to schedule to
  if ((rq == NULL) || (status != DONE_LOADING))
  {
    //nope
    return NULL;
  }else
  {    
    //return it
    return rq;
  }
}

void set_current()
{  
  //is there a running process
  if (runq == NULL)
  {
    //no. make first in ready queue running
    runq = rq;    
    if (rq == rq_last)
    {
      //printk("HELLO MOTO2\n");
      rq_last = NULL;      
    }
    
    //move ready queue on one
    rq = rq->next;
    
    runq->next = NULL;
  }else
  {  
    //yes. add running to back of ready queue
    if (rq_last != NULL)
    {
      rq_last->next = runq;
    }
    runq->next = NULL;
    rq_last = runq;
    //make running first in ready queue
    runq = rq;  
    if (rq == rq_last)
    {      
      rq_last = NULL;      
      
    }
      //move ready queue on one
      rq = rq->next;
  }
  //it's running
  runq->pcb->p_state = RUNNING;
}

void clear_busy(process_control_block* pcb)
{
  //clear the busy bit in gdt
  btr(gdt[pcb->index].access,1);
}

void kill_process()
{
  PRINT_PROC("Let the killing start\n");

  //current = the running processes (that needs to be killed)
  process_list* curr = runq;
  
  //is there actually a process that needs to be killed
  if ((curr != NULL) && (curr->pcb->p_state == RUNNING))
  {    
    //switch to kernel tty
    tty_set_kernel_focus();
    //remove current running tty
    tty_close(runq->pcb->handle);
    
    //we need to free in supervisor mode
    mem_switch_to_kernel_directory();
    
    //release the process from the gdt
    i386_release_GDT_entry(curr->pcb->index);
    
    //index in page_table
    unsigned int j,i = curr->pcb->base >> 22;
    
    page_table* pt;
    
    //traverse the page_dir
    while ((curr->pcb->pd->table[i] != 0x0) && (i <= curr->pcb->end_index))
    {
      //get the page table(s) of the process
      pt = (struct page_table *) ( curr->pcb->pd->table[i] & PAGE_MASK );
    
      for(j=0;j<PTES;j++)
      {
        if (pt->entry[j] != 0x0)
        {
          //remove the page
          mem_free_page((void *)((unsigned int)pt->entry[j]  & PAGE_MASK));
        }
      }
      //free the page table
      mem_free_page(pt);
      
      i++;
    }
    
    //free all used structs
    mem_free_page(curr->pcb->pd);
    mem_free(curr->pcb->state);
    mem_free(curr->pcb->tss);
    mem_free(curr->pcb);
    //free the run/ready queue node
    mem_free(curr);
    
    
    //there isn't a running process anymore
    runq = NULL;
       
    PRINT_PROC("KILLED PROCESS DEAD IN SY MOER\n");
  }else
  {    
    PRINT_PROC("Nothing to kill\n");
  }
  //we don't want extra checks in i8259.c if there's no processes
  if ((runq == NULL) && (rq == NULL))
  {    
    status = NO_PROCESSES;    
  }
}
