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

                              
static process_list *rq = NULL, *rq_last = NULL, *runq = NULL;
static unsigned int status = LOADING;
static segment_descriptor* gdt;


void print_rq()
{
  process_list* fir = rq;
  while(fir!=NULL)
  {
    printk("node = 0x%x\n",fir);
    fir = fir->next;
  }
}        
             
void processes_start()
{  
  //get the gdt from i386
  gdt  = (segment_descriptor*)i386_gdt_offset();
  printk("Start running processes!!!\nStartup Communications\n Press ALT-LSHIFT for next terminal\n");  
  
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
    //the last node in ready queue points to it
    rq_last->next = temp;
    //it's now the last node
    rq_last = temp;
  }  
  //it's ready to state executing
  pcb->p_state = READY;
  //compute it's index in the gdt
  pcb->index = pcb->tr >> 3;
  printk("tr = %d\n",pcb->index);
  //delay(5000);
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
  //make sure we're not rescheduled
  status = LOADING;
  
  //save the context info
  save_state(runq->pcb->state,state);
    
  //set runq to a temp var
  process_list* temp = runq;
  
  //no more running process
  runq = NULL;
  
  //may reschedule now
  status = DONE_LOADING;
  
  //return the process that was running
  return temp;
}

void* add_to_rq(process_list* insert)
{
  //make sure we're not rescheduled
  status = LOADING;
    
  //is there a reasdy queue?
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
      
  //may reschedule now
  status = DONE_LOADING;
  
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
    /*if (rq->pcb->p_state == BLOCKED)
    {
      block_process(rq);
      rq = rq->next;
    }*/
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
    //rq_last = NULL;
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
      printk("HELLO MOTO\n");
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
  printk("Let the killing start\n");
  

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
    
    //********************************************************************
    //bts(gdt[curr->pcb->index].access,1);
    //printk("i = %d\n",curr->pcb->index);
    
    //release the process from the gdt
    i386_release_GDT_entry(curr->pcb->index);
    
    //print_blocks();
    //index in page_table
    unsigned int i= ((curr->pcb->base << 10) >> 22);
    printk("i = %d",i);
    //delay(5000);
    //get the page table of the process
    page_table* pt = (struct page_table *) ( curr->pcb->pd->table[curr->pcb->base >> 22] & PAGE_MASK );
    
    //if (i < PTES)
    
    //free process image
    while ( ((void *)((unsigned int)pt->entry[i]  & PAGE_MASK))!= NULL)
    {
      mem_free_page((void *)((unsigned int)pt->entry[i]  & PAGE_MASK));
      i++;
    }
    //free the stack
    mem_free_page((void *)((unsigned int)pt->entry[i+1]  & PAGE_MASK));
    mem_free_page((void *)((unsigned int)pt->entry[i+2]  & PAGE_MASK));
    mem_free_page((void *)((unsigned int)pt->entry[i+3]  & PAGE_MASK));
    mem_free_page((void *)((unsigned int)pt->entry[i+4]  & PAGE_MASK));
    mem_free_page(pt);
    //free all needed variables
    mem_free_page(curr->pcb->pd);
    mem_free(curr->pcb->state);
    mem_free(curr->pcb->tss);
    mem_free(curr->pcb);
    //free the run/ready queue node
    mem_free(curr);
    
    
    //there isn't a running process anymore
    runq = NULL;
       
    printk("KILLED PROCESS DEAD IN SY MOER\n");
  }else
  {
    //screensaver();
    printk("Nothing to kill\n");
  }
  //we don't want extra checks in i8259.c if there's no processes
  if ((runq == NULL) && (rq == NULL))
  {
    status = NO_PROCESSES;
    //need_scr();  
  }
}
