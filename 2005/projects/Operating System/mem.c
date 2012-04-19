#include "include/io.h"
#include "include/asm.h"
#include "include/stddef.h"
#include "include/error.h"
#include "include/boottable.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/i8253.h"

#ifdef DEBUG_MEM
  #define PRINT_MEM(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_MEM(x, ...)
#endif

static page_directory *kernel_page_directory;

//structure used for linked list
typedef struct mem_block {
  unsigned int size;
  struct mem_block *prev, *next;
}mem_block;


static unsigned int mem,total_mem;
static mem_block* blocks;

#ifdef DEBUG_MEM
/*print a entry in linked list*/
void print_block(mem_block* block){  
  printk("Block adress = 0x%x, size = %d, prev = 0x%x and next = 0x%x\n",block,block->size,block->prev,block->next);  
}

/* prints out the linked list*/
void print_blocks()
{
  gotoxy(0,15);
  printk("\n");
  mem_block* first = blocks;
  while ((first != NULL))
  {
    print_block(first);
    first = first->next;
  }
  gotoxy(0,0);
}
#endif

void mem_switch_to_kernel_directory() {
  i386_set_page_directory(kernel_page_directory);
}

unsigned int mem_available() {
  //return the amount of mem available
  return mem;
}

unsigned int mem_largest_available() {
  mem_block* first = blocks;
  unsigned int size = 0;
  //traverse list en kry largest free block
  while (first != NULL)
  {
    if (first->size > size)
    {
      //found a bigger block
      size = first->size;
    }
    //goto next block
    first = first->next;
  }
  //return the size of the largest block we found
  return size;
}

void mem_copy_kernel_page_directory(page_directory *page_dir) {
  copy_4(kernel_page_directory, page_dir, PAGE_SIZE/4);
}

void* get_align(mem_block* first,unsigned int align_to)
{
  //do we want to align on a 64K boundary?
  if (align_to == 0x10000)
  {
    unsigned int size = 0;
    //is there space left to check for an alignment
    while (i386_align((unsigned int)first+size,0x10000) < ((unsigned int)first + first->size))
    {    
      //we don't want a 64k page aligned on a 128k boundary
      if (i386_align((unsigned int)first + size,0x20000) == i386_align((unsigned int)first+size,0x10000))
      {
        //goto the next boundary
        size = size + 0x10000;        
      }else
      {
        //we found a 64k alignment that's not a 128k alignment. so we return the adress
        return (void*)(i386_align((unsigned int)first+size,0x10000));
      }
    }
    //there isn't a 64k alignment in this block
    return NULL;
  }else
  {
    if (align_to == 0)
    {
      return first;
    }else
    {
      //return a aligned 4k or 128k adress
      return (void*)(i386_align((unsigned int)first,align_to));
    }
  }
}


/*general allocate function used by mem_aloc_page,mem_alloc_dma and mem_alloc*/
void *allocate(unsigned int size,unsigned int align_to,int tag)
{
  mem_block *temp,* first = blocks;
  
  //is alignment neccessary?
  if (align_to != 0)
  {
    //check for an aligned adress in a block that is the correct size
    while ((first != NULL) && ( (get_align(first,align_to) == 0x0) || ((int)first->size - (int)(get_align(first,align_to) - (int)first - sizeof(struct mem_block))) < (int)size) ) 
    {
      //there is none goto next block
      first = first->next;
    }
    
  }else
  {
    //look for adress in list that we can fit block into
    while ((first != NULL) && (first->size < size))
    {
      //there is none. goto next block
      first = first->next;
    }   
  }
  
  // did we find a adress to allocate to?
  if ((first == NULL) || (get_align(first,align_to) == 0x0))
  {
    //no. return null    
    PRINT_MEM("Out of memory\n");    
    while(1);
    return NULL;
  }else
  {
    //found an adress    
    //are we aligning? then we need to split the block in two
    if ((align_to != 0) && (first != get_align(first,align_to)))
    {
      //temp = the adress we're aligning to
      temp = (struct mem_block *)(get_align(first,align_to));      
      //setup the 2 new blocks out of the old one
      temp->next = first->next;
      temp->prev = first;
      first->next = temp;
      temp->size = first->size - ((unsigned int)temp - (unsigned int)first);       
      first->size = (unsigned int)temp - (unsigned int)first;
      //set first as the aligned adress we're going to allocate to
      first = temp;
      
    }
    //are we allocating at the last bit of lower memory under 0xa0000
    if (((unsigned int)first + size) == 0xa0000)
    {
      //was this the first entry in the linked list
      if (first->prev == NULL)
      {
        //just move the start of the linked list to the next node
        blocks = first->next;
        if (blocks != NULL)
        {
          blocks->prev = NULL;
        }
      }else
      {
        //reference the two nodes previous and next to each other
        first->prev->next = first->next;
        //check if there is a next;
        if (first->next != NULL)
        {
          first->next->prev = first->prev;
        }
      }
      //decrement the size of free memory left
      mem = mem - size;
      //check if we need to tag the block
      if (tag == 1)
      {
        //write the size
        writel(size,first);
      }
      //return the block and move it on 4 bytes if it's being tagged
      return (struct mem_block*)((unsigned int)first + sizeof(unsigned int)*tag);
    }
    
    //are we removing a block of memory that's the last block of the list?
    if (((unsigned int)first + size) == total_mem)
    {
      //is there othet blocks
      if (first->prev != NULL)
      {
        //the previous block is now the last
        first->prev->next = NULL;
      }else
      {
        //the list is empty
        blocks = NULL;
      }
      //decrement the size of free memory left
      mem = mem - size;
      //check if we need to tag the block
      if (tag == 1)
      {
        //write the size
        writel(size,first);
      }      
      //return the block and move it on 4 bytes if it's being tagged
      return (struct mem_block*)((unsigned int)first + sizeof(unsigned int)*tag);
    }
    
    //are we allocating exactly the size of a block?
    if (first->size == size)
    {
      //is there a block before it
      if (first->prev != NULL)
      {
        //previous now points to the next
        first->prev->next = first->next;
        //is there a next?
        if (first->next != NULL)
        {
          //next points to previous
          first->next->prev = first->prev;
        }
      }else
      {
        //is there a next?
        if (first->next != NULL)
        {
          //next now doesn't have a previous
          first->next->prev = NULL;
        }
        //move start of list on to next
        blocks = first->next;
      }
      //decrement the size of free memory left
      mem = mem - size;
      //check if we need to tag the block
      if (tag == 1)
      {
        //write the size
        writel(size,first);
      }      
      //return the block and move it on 4 bytes if it's being tagged
      return (struct mem_block*)((unsigned int)first + sizeof(unsigned int)*tag);  
    }
    
    //do we remove at the start of the list
    if (first->prev == NULL)
    {
 
      //blocks = the adress where blocks will start now after allocation
      blocks = (struct mem_block *)((unsigned int)first + size);

      //setup the new start of the list
      blocks->prev = NULL; 
      blocks->next = first->next;

      //check if there is a next
      if (blocks->next != NULL)
      {
        //change next to the new start
        blocks->next->prev = blocks;
      }
      //change the block size
      blocks->size = first->size - size;
    }else
    {
      //temp = adress of where the block will lie after allocating
      temp = (struct mem_block *)((unsigned int)first + size);
      //setup the new struct
      temp->size = first->size - size;
      temp->prev = first->prev;
      temp->next = first->next;      
      //is there a next?
      if (first->next != NULL)
      {
        //point next to the new previous
        first->next->prev = temp;
      }
      first->prev->next = temp;
    }
    
    //decrement the size of free memory left
    mem = mem - size;
    //check if we need to tag the block
    if (tag == 1)
    {
      //write the size
      writel(size,first);
    }
    //return the block and move it on 4 bytes if it's being tagged
    return (struct mem_block*)((unsigned int)first + sizeof(unsigned int)*tag);
  } 
}

/*when we free this checks if we can merge two blocks in the list*/
void merge(mem_block* ret) {
  //fifth check, can we merge to front?
  if (ret->prev != NULL)
  {
    if (((unsigned int)ret->prev + ret->prev->size) == (unsigned int)ret)
    {
      ret->prev->next = ret->next;
      ret->prev->size = ret->prev->size + ret->size;      
      if (ret->next != NULL)
      {
        ret->next->prev = ret->prev;
      }
      ret = ret->prev;
    }
  }
  //sixth check, can we merge to back?  
  if (ret->next != NULL)
  {
    if (((unsigned int)ret + ret->size) == (unsigned int)ret->next)
    {
      ret->size = ret->next->size + ret->size;
      ret->next = ret->next->next;
      if (ret->next != NULL)
      {
        ret->next->prev = ret;
      }
    }
  }
}

void free(void *p, unsigned int size)
{
  
  //cast pointer and set the size
  mem_block* ret = (mem_block *)p;
  ret->size = size;
  
  //we have more memory available
  mem = mem + size;
  
  //first check, memory was full and pointer returns
  if (blocks == NULL)
  {
    //add entry to start of linked list
    blocks = ret;
    blocks->next = NULL;
    blocks->prev = NULL;    
    return;
  }
  
  // get suitable position in list  
  mem_block *last,*first = blocks;
  while ((first < ret) && (first != NULL))
  {
    last = first;
    first = first->next;    
  }
  
  //second check, adding to front of list
  if (first == blocks)
  {
    ret->prev = NULL;
    ret->next = first;
    first->prev = ret;
    blocks = ret;
  }
  
  //third check, adding to back of list
  else if (first == NULL)
  {
    last->next = ret;
    ret->next = NULL;
    ret->prev = last;
  }
  
  //fourth check, adding between blocks
  else
  {
    ret->next = first;
    ret->prev = last;
    first->prev = ret;
    last->next = ret;
  }
  
  //fifth check, can we merge to front?  
  //sixth check, can we merge to back?
  merge(ret);
}

void *mem_allocate_page() {
  //return an aligned 4K page without a tag
  return allocate(0x1000,0x1000,0);
}

void *mem_alloc(unsigned int size) {
  //set size + tag
  size = size + sizeof(unsigned int);
  if (size < sizeof(struct mem_block))
  {
    //set to minimum size if needed
    size = sizeof(struct mem_block);
  }
  
  //return free blovk of memory of size size that's tagged
  return allocate(size,0,1);  
}

void *mem_allocate_dma(int channel) {
  unsigned int size,align_to;
  //get correct size
  if ((channel >= 0) && (channel < 4))
  {
    //we want a 64k aligned page
    size = 0x10000;
    align_to = 0x10000;
  }else if ((channel >= 4) && (channel < 8))
  {
    //we want a 128k aligned page
    size = 0x20000;
    align_to = 0x20000;
  }else
  {
    //channel doesn't exist
    return NULL;
  }

  //get aligned block of free memory of size size
  void * p =  allocate(size,align_to,0);
  //check if it's below 16MB
  if ((unsigned int)p >= 0x1000000)
  {
    //it's not. so free block and return no suitable free memory available
    free(p,align_to);
    return NULL;
  }
  //return block
  return p;
}
  
void mem_free_page(void *p) {
  if (p == NULL)
  {
    PRINT_MEM("Trying to free null pointer\n");
  }else
  {
    //free 4K page
    free(p,0x1000);
  }
}

void mem_free_dma(void *p) {
  if (p == NULL)
  {
    PRINT_MEM("Trying to free null pointer\n");
  }else
  {
    //get the size of the dma_buffer
    if (i386_align((unsigned int)p,0x20000) == (unsigned int)p)
    {
      //free a 128K page
      free(p,0x20000);
    }else
    {
      //free a 64K page
      free(p,0x10000);
    }  
  }
}

void mem_free(void *p) {
  if (p == NULL)
  {
    PRINT_MEM("Trying to free null pointer\n");
  }else
  {
    //cast pointer to include the tag to get the size
    mem_block* ret = (mem_block *)((unsigned int)p - sizeof(unsigned int));
    //free the block
    free(ret,ret->size);
  }
}

void mem_initialize() {
  unsigned int count,i,lo_mem,hi_mem = 0;
  
  smap_entry* smap;
  
  
  //Get the lower memory starting adress 4K above the kernel stack
  lo_mem = bt_entry(BT_KERNEL_STACK_TOP) + 0x1000;
  
  //Kry die aantal extended memory
  
  //Can we use extended bios service 0xe820 to get the memory out of the SMAP entries?
  //get the amount of smap entries
  count = bt_entry(BT_SMAP_TOTAL);
  //is there any?
  if (count > 0)
  {
    //loop trough all the smap entries
    for (i = 0;i < count;i++)
    {
      //read the smap entry
      if (bt_read_smap_entry(i,smap) == OK)
      {
        //check if it's of type SMAP_TYPE_ARM and in higher memory
        if ((smap->type == SMAP_TYPE_ARM) && (smap->base >= 0x100000))
        {
          //it is. increment the size of himem
          hi_mem = hi_mem + smap->size;          
        }
      }
    }
  }else
  {
    //there is no smap entries
    //try reading BT_EXTENDED_MEMORY_E801
    hi_mem = bt_entry(BT_EXTENDED_MEMORY_E801);
    
    //do we need to use the BT_EXTENDED_MEMORY entry?
    if (hi_mem == 0)
    {
      //read the bt_entry in KB and convert to bytes
      hi_mem = bt_entry(BT_EXTENDED_MEMORY) * 1024;      
    }
  }
    
  //the total memory available
  mem = 0xa0000 - lo_mem + hi_mem;
  
  //Create a linked list for the empty blocks in memory
  //lower memory list entry
  blocks = (mem_block*)lo_mem;
  blocks->size = 0xa0000 - lo_mem;
  blocks->prev = NULL;
  //check if there is extended memory
  if (hi_mem > sizeof(struct mem_block*))
  {
    //add to list
    blocks->next = (mem_block*)0x100000;
    blocks->next->size = hi_mem;
    blocks->next->next = NULL;
    blocks->next->prev = blocks;
  }else
  {
    //only one entry in list
    blocks->next = NULL;
  }
  
  //allocate a page for the kernel page directory    
  if ((kernel_page_directory = mem_allocate_page()) != NULL)
  {
    
    //how much memory does the system have?
    total_mem = hi_mem + 0x100000;
    //set the page directory and empty page table
    page_directory* pd = kernel_page_directory;
    page_table* pt = NULL;
        
    //get the amount of 4k pages we need for the system    
    unsigned int to_map = (hi_mem + 0x100000) / 0x1000;
    //control variables and the start of memory where we're mapping from
    unsigned int j,base = 0x0;
    i = 0;
    
    //first mark all entries in page directory as not present
    for (j=0; j < PTES; j++)
    {
      pd->table[j] = 0x0;
    }
    
    //do this while we have memory that needs to be mapped
    while (to_map > 0)
    {
      //there is more than 4GB of memory
      if (i == PTES)
      {
        //we don't handle it
        PRINT_MEM("page directory is vol\n");
        break;
      }
      
      //get a page table
      pt = (struct page_table*)(mem_allocate_page());
      //add to page directory
      pd->table[i] = ((unsigned int)pt ) | KERNEL_PAGE;
      
      // if there is more of 4MB of memory to allocate currently
      if (to_map >= PTES)
      {
        //loop through a page table
        for (j = 0;j < PTES;j++)
         {
           //check if it's one of the 3 adresses that must be marked as null pages
           if ((base == 0x0) | (base == (lo_mem - 0x1000)) | (base == lo_mem - 2* 0x1000 - bt_entry(BT_KERNEL_STACK_SIZE)))
           {
             //mark as null
             pt->entry[j] = 0x0;             
           }else
           {
             //mark as kernel page
             pt->entry[j] = base | KERNEL_PAGE;
           }
	   //goto next page
           base = base + 0x1000;
	   //one less 4K block to map
           to_map--;
         }
        //goto next page table    
        i++;
      }else
      {
	//there is less than 4MB of memory to map
        //loop through a page table        
        for (j = 0;j < PTES;j++)
        {
          // do we need to map an page?
          if (to_map > 0)
          {
            //check if it's one of the 3 adresses that must be marked as null pages
            if ((base == 0x0) | (base == (lo_mem - 0x1000)) | (base == lo_mem - 2* 0x1000 - bt_entry(BT_KERNEL_STACK_SIZE)))
            {
              //mark as null
              pt->entry[j] = 0x0;
            }else
            {
              //mark as kernel page
              pt->entry[j] = base | KERNEL_PAGE;
            }
	    //goto next page
            base = base + 0x1000;
	    //one less 4K block to map
            to_map--;
          }else
          {
            //mark as not present
            pt->entry[j] = 0x0;
          }
        }
      }
    }
    
    //enable paging
    enable_paging(kernel_page_directory);
    PRINT_MEM("Paging enabled\n");    
  }else
  {
    //serious error, go on without paging
    PRINT_MEM("Kernel page not loaded\n");
  }  
  PRINT_MEM("Initialized memory\n");
}
