#include "include/asm.h"
#include "include/mem.h"
#include "include/floppy.h"
#include "include/trace.h"
#include "include/process.h"
#include "include/error.h"
#include "include/io.h"
#include "include/loader.h"

#define PROCESS_DISK_START 400
/*
typedef struct process_header{
  unsigned char main[4],header[4];
  unsigned int base,end;
}process_header;

void print_header(process_header* ph)
{
  printk("main = %x\n",ph->main[0]);
  printk("header = %c %c %c\n",ph->header[1],ph->header[2],ph->header[3]);
  printk("base = %x\n",ph->base);
  printk("end = %x\n",ph->end);
}*/

void loader_initialize()
{
  printk("Start Loading Processes...\n");
  unsigned int offset = 0,result,loaded = LOADING,size,operations,pages,i,base,end;
  page_directory* pd;
  page_table* pt;
  process_control_block* pcb; 
  /*enable interrupts to make sure floppy and clock work properly*/
  unsigned int eflags = eflags();
  enable_interrupts();
  
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
        
        //create page table for process
        pt = (struct page_table*)mem_allocate_page();
        
        for (i=0;i< PTES;i++)
        {
          pt->entry[i] = 0x0;
        }
        
        for (i=0;i< (pages + 0x6);i++)
        {
          if ((i == pages + 0x5) || (i== pages))
          {
            //pt->entry[i] = 0x0;
          }else
          {
            pt->entry[i] = (unsigned int)mem_allocate_page() | USER_PAGE;
          }
        }
        
        //add page table into page directory
        pd->table[base >> 22] = (unsigned int)pt | USER_PAGE;
        
        //switch to page directory
        i386_set_page_directory(pd);
        
        //copy the disk image
        
        if (operations > 1)
        {          
          copy_4(buf,base,SECTOR_SIZE/4);
          floppy_read(0,PROCESS_DISK_START + offset + 1,(operations-1),(void *)((unsigned int)base + SECTOR_SIZE),(operations-1) * SECTOR_SIZE);          
        }else
        {
          copy_4(buf,base,SECTOR_SIZE/4);
        }
        //make sure we work in superuser mode
        mem_switch_to_kernel_directory();
        
        //install the process
        //allocate all neccessary structs
        pcb = (struct process_control_block*)mem_alloc(sizeof(struct process_control_block));
        pcb->tss = (struct task_state_segment*)mem_alloc(sizeof(struct task_state_segment));
        pcb->tr = (unsigned short*)mem_alloc(sizeof(unsigned short));
        pcb->state = (struct cpu_state*)mem_alloc(sizeof(struct cpu_state));
        
        //set page directory
        pcb->pd = pd;
        //set base address
        pcb->base = base;
        
        //intialize tts into gdt and get a task register
        i386_initialize_tss(pcb->tss,pcb->tr);
        
        //initialize context info        
        i386_set_cpu_state(pcb->state,base + (pages+0x5)*PAGE_SIZE,base);

        //install into ready queue
        install_process(pcb);
        
        printk("Process %d loaded\n",offset/100 + 1);
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
      //free memory used by buffer
      mem_free(buf);
      printk("Disk error %d\n",result);
    } 
  }  
  //free memory used by buffer
  mem_free(buf);  
  
  if (offset == 0)
  {
    need_screensaver();
    printk("No processes on disk\n");
    screensaver();
  }else
  {
    printk("Done loading processes\n");
    //set the processes to start
    processes_start();
    //restore eflags
  }
  restore_flags(eflags);
}
