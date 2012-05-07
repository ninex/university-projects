/*
 *  MODULE: memdrill
 *  DESCRIPTION: strain test 4 memory manager
 *
 *  AUTHOR: Heinrich du Toit
 *
 */


#include "include/stddef.h"
#include "include/trace.h"
#include "include/memdrill.h"
#include "include/i8253.h"

#include "include/mem.h"

#include "include/asm.h"

/*
 Tune these numbers for the system :)  
 */
/*
For big pc (256MB AMD athlon 1.3GHZ (266 FSB))

#define MAX 200
#define SUPERMAX 1000
#define MAXPIECE 350
#define PAGESIZE 0x1000
#define GOFORIT 1000000

results:
 mem avail 229296KB's
 link list = 28157
 madness ratings ~ 482400
 time 933 ticks

*/

//smal system
#define MAX 50
#define SUPERMAX 250
#define MAXPIECE 450
#define PAGESIZE 0x1000
#define GOFORIT 150000
/*
 ratings:
 

 */


#define PRIME1 10103
#define PRIME2 100103


typedef struct linklist * listlist_p;
typedef struct linklist{
  void * page;
  listlist_p next;
}linklist;

linklist *liststart;
unsigned int listcount;

typedef struct level2{
  void * ptr[MAX];

}level2;

typedef struct level1{
  level2 * sub[SUPERMAX];
}level1;

level1 * madness;
unsigned int madness_rating;

#define DMANR 5
#define DMAMAXCHAN 8 //yes this is 2 high but it makes sure the system knows this!

void * dma_ptr[DMANR];
int dma_blocks;

void godma(){
  dma_blocks = 0;
  int i;
  for (i = 0; i < DMANR ; i++){
    dma_ptr[i] = mem_allocate_dma(i % DMAMAXCHAN);
    if (dma_ptr[i])
      dma_blocks++;
  }
  //note this thing assumes that the memory are correctly allocated
  //boundaries and sizes are not checked
  //printk("Got %d dma blocks\n",dma_blocks);
}
void free1dma(){
  int i;
  for (i = 0; i < DMANR ; i++)
    if (dma_ptr[i]){ 
      mem_free_dma(dma_ptr[i]);
      dma_ptr[i] = NULL;
      return;
    }
}
void freedma(){
  int i;
  for (i = 0; i < DMANR ; i++)
    if (dma_ptr[i]) mem_free_dma(dma_ptr[i]);
}


void gomemmadness(unsigned int seed){
  unsigned int random = seed;
  unsigned int count;
  unsigned int val1;
  unsigned int val2;
  unsigned int maxsize = MAXPIECE;
  level2 *lvl2;

  madness = (level1 *)mem_alloc(sizeof(level1));
  if (!madness) return;
  fill_4(madness,0,sizeof(level1)/4);


  for (count=0; count < GOFORIT ; count++){
    random = (random * PRIME2) + 432;
    val1 = random % SUPERMAX;
    random = (random * PRIME2) + 13;
    if (madness->sub[val1]){
      lvl2 = madness->sub[val1];
      val2 = random % MAX;
      random = (random * PRIME2) + 1023;
      if (lvl2->ptr[val2]){
	mem_free(lvl2->ptr[val2]);
	lvl2->ptr[val2] = NULL;
      }else{
	lvl2->ptr[val2] = mem_alloc((random % maxsize)+1);
	random = (random * PRIME2) + 3843;
	if (lvl2->ptr[val2]){ 
	  madness_rating++;
	  maxsize++;
	}else{
	  maxsize -= 10;
	  if (maxsize < 40) maxsize = 40;
	}
      }
    }else{
      madness->sub[val1] = (level2 *)mem_alloc(sizeof(level2));
      if (madness->sub[val1]){
	madness_rating++;
	fill_4(madness->sub[val1],0,sizeof(level2)/4);
      }      
    }
        
  }  
  //printk("hier\n");
  //cleanup:
  for (count = 0; count < SUPERMAX ; count++)
    if (madness->sub[count]){
      lvl2 = madness->sub[count];
      unsigned int i;
      for (i=0; i < MAX ; i++)
	if(lvl2->ptr[i]){
	  mem_free(lvl2->ptr[i]);	
	}
      mem_free(madness->sub[count]);
    }
  mem_free(madness);
}

void golinklist(){
  unsigned int size;
  linklist * t;
  size = 3047;
  liststart = (linklist *)mem_alloc(size);
  if (!liststart) return;
  t= liststart;
  do{
    t->page = mem_allocate_page();
    if (!(t->page)) return;
    listcount++;
    t->next = (linklist *)mem_alloc(size);
    size += 3;
    if (size > 4050)
      size = 3047;
    t = t->next;
  }while(t);
  
}

void freelinklist(){
  linklist * t;
  linklist * p;
  t= liststart;
  if (!t) return;
  while (t){
    p = t;
    t = t->next;
    if (p->page)
      mem_free_page(p->page);
    mem_free(p);
  }
}

void mem_drill(){
  unsigned int memtotal;
  unsigned int memmax;
  unsigned int starttime;
  printk("Mem drill commencing....\n");
  memtotal = mem_available();
  memmax = mem_largest_available();
  starttime = timer_get_ticks();
  printk("Mem avail: %#x Max block: %#x \n",memtotal,memmax);

  //dma
  godma();  

  //linklist
  listcount = 0;
  golinklist();
  printk("Link list count = %d\n",listcount);

  free1dma(); //makes space for 1 big alloc :)

  //mem madness
  
  madness_rating = 0;printk("hier2\n");
  gomemmadness(7);  printk("hier\n");
  printk("Madness rating = %d\n",madness_rating);
  madness_rating = 0;
  gomemmadness(PRIME1);
  printk("Madness rating = %d\n",madness_rating);
  madness_rating = 0;
  gomemmadness(43823); 
  printk("Madness rating = %d\n",madness_rating);


  //free linklist
  freelinklist();
  //free dma
   freedma();
  
  printk("Mem drill complete (time = %d ticks)\n",timer_get_ticks()-starttime);
  printk("Mem avail: %#x Max block %#x \n",mem_available(),mem_largest_available());
  if (memtotal != mem_available()){
    printk("ERROR: mem total before & after differ!");
  }
  if (memmax != mem_largest_available()){
    printk("ERROR: mem max before & after differ!");
  }

} 
