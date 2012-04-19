#include "include/asm.h"
#include "include/error.h"
#include "include/io.h"
#include "include/dma.h"
#include "include/trace.h"
#include "include/mem.h"
#include "include/i386.h"
#include "include/floppy.h"
#include "include/stddef.h"
#include "include/loader.h"
#include "include/process.h"



#define MAX_SIZE_OF_PROCESS 100*SECTOR_SIZE
/* position to start reading on floppy*/
#define FLOP_START 400

void loader_initialize()
{
	unsigned int i = 0;
	unsigned int result = 0;
	unsigned int offset = 0;
	unsigned int size_of_process = 0;
	unsigned int num_of_operations = 0;
	unsigned int num_of_pages = 0;
	unsigned int size_to_copy = 0;
	unsigned int temp_base = 0;
	int z = 1;
	unsigned int valid_process = TRUE;
	
	unsigned int flags = eflags();
	enable_interrupts();
	


	page_directory * page_d;
	page_table *page_t;
	pc_block *pcb;
	
	p_header *ph;
	/*remember to free buffer*/
	void *buffer = mem_alloc(SECTOR_SIZE);
	
	while (valid_process == TRUE)
	{
		/* the drive number is always 0 in our case and if equal to 0 then no error occured!!!!*/
		if((result = floppy_read(0,FLOP_START + offset,1,buffer,SECTOR_SIZE)) == OK)
		{	
			ph = (struct p_header*)buffer;
			temp_base = ph->base;

			if ((ph->main[0] == 0xe8) && (ph->head[1] == 0x43) && (ph->head[2] == 0x53) && (ph->head[3] == 0x33))
			{
				printk("Valid Process Found ! Process [%d]\n",z);
				
				if (ph->end < ph->base)
				{
					printk("ERROR : Process end comes before the base of the process!!");
				}
				/*maybe unsigned int*/
				size_of_process = ph->end - ph->base;
				
				if(size_of_process > MAX_SIZE_OF_PROCESS)
				{
					printk("ERROR : Size of process is too large : size [%d]\n",size_of_process);
				}
			
				/*size of the buffer is the size of a sector. making sure the number
				of operations is one more in case of remaining data*/
				num_of_operations = size_of_process/SECTOR_SIZE;
				if (num_of_operations * SECTOR_SIZE < size_of_process)
				{
					num_of_operations++;
				}
			
				num_of_pages = size_of_process/PAGE_SIZE;
				if (num_of_pages * FOUR_K < size_of_process)
				{
					num_of_pages++;
				}
				//allocate a page_directory for the process
				page_d = (struct page_directory *)mem_allocate_page();
				mem_copy_kernel_page_directory(page_d);
				/* pretty sure i only need to allocate one page table as the max size/PAGE_SIZE 
				should only need one page */ 
				page_t = (struct page_table *)mem_allocate_page();	
				
				page_d->table[temp_base >> 22] = (unsigned int)page_t | USER_PAGE;
				
				for(i = 0; i < PTES; i++)
				{
					page_t->entry[i] = 0x0;
				}
				// compensation for the 2 null pages and for the process stack
				for(i = 0; i < num_of_pages + 0x6; i++)
				{
					/*Null page below and above stack must not be marked as user pages */
					if ((i != num_of_pages + 0x5) && (i != num_of_pages ))
					{
						page_t->entry[i] = ((unsigned int)mem_allocate_page() | USER_PAGE);
					}
				}
				
				i386_set_page_directory(page_d);
				
				size_to_copy = (size_of_process) / SECTOR_SIZE;
				if (size_to_copy*SECTOR_SIZE != (size_of_process))
					size_to_copy++;
				if (size_to_copy > 0) // make sure this cast is correct (void *)
					floppy_read(0,FLOP_START + offset, size_to_copy,(void *)temp_base,SECTOR_SIZE*size_to_copy);
				
				mem_switch_to_kernel_directory();

				pcb = (struct pc_block*)mem_alloc(sizeof(struct pc_block));
				pcb->pd = page_d;
				pcb->tr = mem_alloc(sizeof(unsigned short));
				pcb->tss = (struct task_state_segment*)mem_alloc(sizeof(task_state_segment));
				
				i386_initialize_tss(pcb->tss, pcb->tr);
				pcb->state = (struct cpu_state*)mem_alloc(sizeof(struct cpu_state));
				
				//new - some sort of process id
				//pcb->process_num = z;
			
				i386_set_cpu_state(pcb->state, temp_base + ((num_of_pages + 5)*PAGE_SIZE) , temp_base);
				
				install_ready_queue(pcb);
				offset = offset + 100;
				z++;
			}
			else
			{
				printk("Process not found:\n");
				valid_process = FALSE;
			}
		}
		else
		{
			mem_free(buffer);
			printk("Error reading from floppy\n");
			return; //result;
		}	
	}
	mem_free(buffer);
	//unsigned int mem_left = mem_available();
	//printk("mem available is [%d]\n",mem_left);
	process_load_complete();
	restore_flags(flags);
}
