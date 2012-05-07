#include "include/asm.h"
#include "include/mem.h"
#include "include/floppy.h"
#include "include/trace.h"
#include "include/process.h"
#include "include/error.h"
#include "include/io.h"
#include "include/loader.h"

#ifdef DEBUG_LOADER
  #define PRINT_LOAD(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_LOAD(x, ...)
#endif

#define PROCESS_DISK_START 400


void loader_initialize()
{
  PRINT_LOAD("Start Loading Processes...\n");
  unsigned int offset = 0,result,loaded = LOADING,size,operations,pages,base,end;
  int i;
  page_directory* pd;
  page_table* pt;
  process_control_block* pcb; 
  /*enable interrupts to make sure floppy and clock work properly*/
  unsigned int eflags = enable_interrupts();
  
  /*read all the processes from disk*/  
  void* buf = mem_alloc(SECTOR_SIZE);
  
  //while there's processes that needed to be loaded
  while (loaded == LOADING)
  {    
    //read the first sector from the disk
    if ( (result = floppy_read(0, PROCESS_DISK_START + offset, 1, buf, SECTOR_SIZE)) == 0)
    {
      //check the process header      
      if ((readb(buf) == 0xe8) && (readb(buf+5) == 0x43) && (readb(buf+6) == 0x53) && (readb(buf+7) == 0x33))
      {
        //valid process header found
        
        //get the linkbase
        base = readl(buf+8);
        end = readl(buf+12);
                
        //get size of process
        size = end - base;
        
        //calculate number of disk operations
        operations = size/SECTOR_SIZE;
        if ((size % SECTOR_SIZE) > 0)
        {
          operations++;
        }
        
        //calculate amount of 4k pages
        pages = size/0x1000;
        if ((size % 0x1000) > 0)
        {
          pages++;
        }
        
        //allocate page for process page_directory
        pd = (struct page_directory*)mem_allocate_page();
        
        //copy kernel page directory
        mem_copy_kernel_page_directory(pd);
        
        unsigned int index = (base << 10) >> 22;
        
        unsigned int limit = index + pages + 0x5;
        
        
        //create page table(s) for process
        pt = (struct page_table*)mem_allocate_page();
        
        for (i=0;i< PTES;i++)
        {
          pt->entry[i] = 0x0;
        }
        //work out if the linkbase exceeds 2 page tables
        int limit2;
        if (limit >= PTES)
        {
          limit2 = PTES;
        }else
        {
          limit2 = limit;
        }
        //create the first page table
        for (i = index; i < limit2;i++)
        {
          if ((i != index + pages + 0x5) && (i != index + pages))
          {
            pt->entry[i] = (unsigned int)mem_allocate_page() | USER_PAGE;
          }
        }
          
        //add page table into page directory
        pd->table[base >> 22] = (unsigned int)pt | USER_PAGE;
        i = base >> 22;  
        //do we need a second page table
        if ((limit >= PTES) && (i < PTES))
        {
          pt = (struct page_table*)mem_allocate_page();
        
          //clear it
          for (i=0;i< PTES;i++)
          {
            pt->entry[i] = 0x0;
          }
          //wotk out the top
          limit = limit - index - 1;
          
          //fill the page table from the top
          for (i = limit; i >= 0;i--)
          {            
            if ((i != limit - 0x5) && (i != limit))
            {
              pt->entry[i] = (unsigned int)mem_allocate_page() | USER_PAGE;
            }
          }
          //add to the page directory
          pd->table[(base >> 22) + 1] = (unsigned int)pt | USER_PAGE;
          i = (base >> 22) + 1;
        }
        
        
        
        //switch to page directory
        i386_set_page_directory(pd);
        
        //copy the disk image
        
        if (operations > 1)
        {
          floppy_read(0,PROCESS_DISK_START + offset + 1,(operations-1),(void *)((unsigned int)base + SECTOR_SIZE),(operations-1) * SECTOR_SIZE);          
        }
        copy_4(buf,base,SECTOR_SIZE/4);
        
        //make sure we work in superuser mode
        mem_switch_to_kernel_directory();
        
        //install the process
        //allocate all neccessary structs
        pcb = (struct process_control_block*)mem_alloc(sizeof(struct process_control_block));
        pcb->tss = (struct task_state_segment*)mem_alloc(sizeof(struct task_state_segment));
        pcb->state = (struct cpu_state*)mem_alloc(sizeof(struct cpu_state));
        
        //set page directory
        pcb->pd = pd;
        //set base address
        pcb->base = base;
        //where the page tables stop in the page_dir
        pcb->end_index = i;
        
        //intialize tts into gdt and get a task register
        i386_initialize_tss(pcb->tss,&(pcb->tr));
        
        //initialize context info        
        i386_set_cpu_state(pcb->state,base + (pages+0x5)*PAGE_SIZE,base);

        //install into ready queue
        install_process(pcb);
        
        PRINT_LOAD("Process %d loaded\n",offset/100 + 1);
        //move offset on by 100 to next possible process
        offset = offset + 100;        
      }else
      {        
        //all processes done loading
        loaded = DONE_LOADING;
      }
    }else
    {
      //disk error      
      PRINT_LOAD("Disk error %d\n",result);
    } 
  }  
  //free memory used by buffer
  mem_free(buf);  
  
  if (offset == 0)
  {
    PRINT_LOAD("No processes on disk\n");    
  }else
  {
    PRINT_LOAD("Done loading processes\n");
    //set the processes to start
    processes_start();    
  }
  //restore eflags
  restore_flags(eflags);
}
