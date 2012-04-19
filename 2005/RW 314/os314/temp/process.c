#include "include/asm.h"
#include "include/error.h"
#include "include/io.h"
#include "include/dma.h"
#include "include/trace.h"
#include "include/mem.h"
#include "include/i386.h"
#include "include/stddef.h"
#include "include/loader.h"
#include "include/process.h"
#include "include/tty.h"

// static queue initializations
static process_queue *ready = NULL;
static process_queue *running = NULL;
static process_queue *ready_last = NULL;

static unsigned int loading_status = INCOMPLETE;
static segment_descriptor* gdt;

void install_ready_queue(pc_block *pcb)
{
	if (ready == NULL)
	{
		ready = (process_queue *)mem_alloc(sizeof(struct process_queue));
    	ready->pcb = pcb;
    	ready->next = NULL;    
    	ready_last = ready;
	}
	else
	{
		process_queue * new = (process_queue *)mem_alloc(sizeof(struct process_queue));
    	new->pcb = pcb;
    	new->next = NULL;
		ready_last->next = new;
    	ready_last = new;
	}
	pcb->process_state = READY;
  	//pcb->index = (*pcb->tr - SELECTOR_DPL0)/8;  
	pcb->index = *pcb->tr >> 3;
}

void process_load_complete()
{
	loading_status = COMPLETE;
	gdt = (segment_descriptor *)i386_gdt_offset();
}

int get_status()
{
	return loading_status;
}

void clear_busy_bit(pc_block* pcb)
{
  btr(gdt[pcb->index].access,1);
}

process_queue * get_current_pcb()
{
  if (running == NULL)
  {
    return NULL;
  }else
  {
    return running;
  }
}

process_queue* schedule()
{
  if (ready == NULL)
  {
    return NULL;
  }else
  {
    return ready;
  }
}

void set_current_pcb(process_queue* pq)
{
  pq->pcb->process_state = (void *)RUNNING;
  if (running == NULL)
  {
    running = ready;
    ready = ready->next;
    return;
  }
  ready_last->next = running;
  running->next = NULL;
  ready_last = running;
  running = ready;
  ready = ready->next;
}

void save_stack_info(cpu_state* new_state,cpu_state* stack_info)
{
	copy_4(stack_info,new_state,sizeof(struct cpu_state)/4);
}

void load_context_info(cpu_state* new_state,cpu_state * stack_info)
{
	copy_4(new_state,stack_info,sizeof(struct cpu_state)/4);
}


void aniahilate_process()
{
	tty_set_kernel_focus();
	//process_queue * current_running = get_current_pcb();
	process_queue * temp_current = running;
	
	running = NULL;
	//free_queue_node(temp_current);
}

void free_queue_node(process_queue* current)
{
	page_table * temp_page_t = NULL;
	int i, j;
	//if ((current != NULL) && ())
	mem_switch_to_kernel_directory();
	
	current->next = NULL;
	
	i386_release_GDT_entry(current->pcb->index);
	current->pcb->index = NULL;
	
	current->pcb->process_state = NULL;
	
	mem_free(current->pcb->tr);
	mem_free(current->pcb->tss);
	mem_free(current->pcb->state);
	 // need to free the pages in the tables of the entries of the directory page
	 while (current->pcb->pd->table[i] != NULL)
	 {
	 	temp_page_t = (page_table *)current->pcb->pd->table[i];
		printk("temp_page_t[%d] is [0x%x]\n",i,temp_page_t);
		
		/*while (j < PTES)//(temp_page_t->entry[j] != NULL)
		{
			printk("temp_page_t->entry[%d] is [0x%x] \n",j,temp_page_t->entry[j]);
			temp_page_t->entry[j] = NULL;
			j++;
		}*/
		i++;
	 }
	//mem_free(current->pcb->pd);
	//mem_free(current->pcb);
	//mem_free(current);
	/*unsigned int new_mem_left = mem_available();
	printk("mem available is [%d]\n",new_mem_left);*/
	
}

void re_schedule(cpu_state * state)
{
	process_queue *current,*queue;
	current = get_current_pcb();//returns running proceces in its queue node, or null.
	queue = schedule();//returns ready proceces in its queue node, or null.

	// waiting for all processes to be loaded 
	if (queue != current)
	{
		if (queue != NULL)
		{

			//If the ready process is not a null process
			if (current != NULL)
			{
				/* If an acceptable running process was found then the stack info needs to be saved 
			 	 * so when switching back to the same process its cpu state will have been saved.
			 	 * This allowas the process to continue from wher it left off
			     */
				save_stack_info(current->pcb->state,state);
				/*if (queue->pcb->process_state == RUNNING)
				{
					load_context_info(queue->pcb->state,state);
				}*/
			}
			set_current_pcb(queue);

			i386_set_page_directory(queue->pcb->pd);

			/*clear busy bit in tss*/    
			clear_busy_bit(queue->pcb);

			/*load the task register*/
			load_task_register(queue->pcb->tr);

			load_context_info(queue->pcb->state,state);

		}
	}
	else
	{
		/*if ((current != NULL) && (current->pcb->process_state == RUNNING))
		{
			return_from_interrupt(state);
		}
		else
		{
			//printk("in the else\n");
			return_from_interrupt(state);
      	}*/
		
	}
}
