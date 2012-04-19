#include "include/stddef.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/mem.h"
#include "include/keyboard.h"
#include "include/memtest.h"
#include "include/mem.h"
#include "include/random.h"
#include "include/i8253.h"

#define MAX_ELEMENTS 20000

void test_key(char ch);


typedef struct lls {
  void *ptr;
  struct lls *next;
} lls;

typedef struct T1 {
  void *page;
  void *dma_buffer;
  struct T1 *next;
} T1;

typedef struct T2 {
  char data[MAX_ELEMENTS];
  struct T2 *next;
} T2;

void insert_T2(T2 **first, T2 *new) {
  T2 *n;

  if (*first == NULL)
    *first = new;
  else {
    n = *first;
    while (n->next != NULL)
      n = n->next;
    new->next = n->next;
    n->next = new;
  }
}

void insert_T1(T1 **first, T1 *new) {
  T1 *n;

  if (*first == NULL)
    *first = new;
  else {
    n = *first;
    while (n->next != NULL)
      n = n->next;
    new->next = n->next;
    n->next = new;
  }
}
  T1 *t1;
  T1 *temp_t1;
  T2 *t2;
  T2 *temp_t2;
  lls *lls_start;
  void *test_ptr;
  
void mem_test() {
//  char ch;
  printk("[mem_test] press 'h' for help\n");
  t1 = NULL;
  t2 = NULL;
  lls_start = NULL;
  test_ptr = NULL;

  while (1) {
    if (keyboard_available()) {
      test_key(keyboard_read());
//      printk("{%c}\n",ch);
//      
    }
  }
}


void test_key(char ch)
{
  int i,j,max;
  unsigned int before;
  lls **lls_cur, *lls_node;
      switch (ch) {
	  
        case '1':
          printk("[mem_test] mem_available() = %d bytes\n", mem_available());
          break;

        case '2':
          printk("[mem_test] mem_largest_available() = %d bytes\n", mem_largest_available());
          break;

        case '3':
          temp_t2 = mem_alloc(sizeof(T2));
          if (temp_t2 == NULL)
            printk("[mem_test] mem_alloc failed for option 3\n");
          else {
            printk("[mem_test] memory allocated for T2 at %#.8x\n", temp_t2);
            temp_t2->next = NULL;
            insert_T2(&t2, temp_t2);
          }
          break;

        case '4':
          temp_t2 = t2;
          if (temp_t2 == NULL)
            printk("[mem_test] no more elements in linked list\n");
          else {
            printk("[mem_test] freeing memory allocated for T2 at %#.8x\n", temp_t2);
            t2 = t2->next;
            mem_free(temp_t2);
          }
          break;

        case '5':
          temp_t2 = t2;
          while (temp_t2 != NULL) {
            printk("[mem_test] current node=%#.8x, next=%#.8x\n", temp_t2, temp_t2->next);
            temp_t2 = temp_t2->next;
          }
          break;

        case '6':
          temp_t1 = mem_alloc(sizeof(T1));
          if (temp_t1 == NULL)
            printk("[mem_test] mem_alloc failed for option 6\n");
          else {
            temp_t1->page = NULL;
            temp_t1->dma_buffer = NULL;
            temp_t1->next = NULL;
            temp_t1->page = mem_allocate_page();
            if (temp_t1->page == NULL) {
              printk("[mem_test] mem_allocate_page failed\n");
              mem_free(temp_t1);
            } else {
              printk("[mem_test] page allocated at %#.8x through %#.8x\n", temp_t1->page,temp_t1);
              insert_T1(&t1, temp_t1);
            }
          }
          break;

        case '7':
          temp_t1 = mem_alloc(sizeof(T1));
          if (temp_t1 == NULL)
            printk("[mem_test] mem_alloc failed for option 7\n");
          else {
            temp_t1->page = NULL;
            temp_t1->dma_buffer = NULL;
            temp_t1->next = NULL;
		seed();
	    i = uniform(8);
            printk("Allocation on channel %d\n",i);
            temp_t1->dma_buffer = mem_allocate_dma(i);
            if (temp_t1->dma_buffer == NULL) {
              printk("[mem_test] mem_allocate_dma failed\n");
              mem_free(temp_t1);
	    } else if ((unsigned int)temp_t1->dma_buffer > 0x1000000) {
		    printk("[mem_test] Got a dma pointer %#.8x on channel %d that was not below 16MB!!!!\n",temp_t1->dma_buffer,i);
		    while(1);
            } else {

              printk("[mem_test] dma buffer allocated at %#.8x on channel %d\n", temp_t1->dma_buffer,i);
              insert_T1(&t1, temp_t1);
            }
          }
          break;

        
        case '8':
          temp_t1 = t1;
          if (temp_t1 == NULL)
            printk("[mem_test] no more elements in linked list\n");
          else {
            if (temp_t1->page != NULL) {
              printk("[mem_test] freeing memory allocated for page at %#.8x\n", temp_t1->page);
              mem_free_page(temp_t1->page);
            }
            if (temp_t1->dma_buffer != NULL) {
              printk("[mem_test] freeing memory allocated for dma_buffer at %#.8x\n", temp_t1->dma_buffer);
              mem_free_dma(temp_t1->dma_buffer);
            }
            printk("[mem_test] freeing memory allocated for T1 at %#.8x\n", temp_t1);
            t1 = t1->next;
            mem_free(temp_t1);
          }
          break;

        case '9':
          temp_t1 = t1;
          while (temp_t1 != NULL) {
            printk("[mem_test] node=%#.8x, page=%#.8x, dma_buffer=%#.8x, next=%#.8x\n", temp_t1,
                   temp_t1->page, temp_t1->dma_buffer, temp_t1->next);
            temp_t1 = temp_t1->next;
          }
          break;

        case 'c':
          clear_display();
          break;

        case 'h':
          printk("[mem_test] Options:\n");
          printk("           '1' - display available memory\n");
          printk("           '2' - display largest available memory\n");
          printk("           '3' - mem_alloc()\n");
          printk("           '4' - mem_free()\n");
          printk("           '5' - traverse linked list created by options 3 and 4\n");
          printk("           '6' - mem_alloc() & mem_allocate_page() \n");
          printk("           '7' - mem_alloc() & mem_allocate_dma() \n");
          printk("           '8' - mem_free(), mem_free_page() and mem_free_dma()\n");
          printk("           '9' - traverse linked list created by options 6, 7 and 8\n");
          printk("           'c' - clear display\n");
          printk("           'h' - help\n");
	  printk("           'l' - show block list\n");
	  printk("           'u' - uber test\n");
	  printk("           'f' - fill all memory\n");
          break;

	case 'l':
	  print_blocks();
	  break;
          
        case 't':
           //test();
           break;
          
	case 'u':
	  	before = mem_available();
		seed();
		max = uniform(5000);
		for (i = 0; i < max; i++)
		{
			if (i % 10 == 0)
			{
				printk("[%d of %d]\n",i,max);
			}
			j = uniform(50);
			if (j > 4) j = j % 5;
			if (j == 0) test_key('3');
			else if (j == 1) test_key('4');
			else if (j == 2) test_key('6');
			else if (j == 3) test_key('8');
			else if (j == 4) test_key('7');
		}
		while (t2 != NULL)
			test_key('4');
		while (t1 != NULL)
			test_key('8');
		printk("RESULT:: \nDifference between before and after = %d\n",before-mem_available());
				
	  break;
	case 'f':
	  if (lls_start == NULL)
	  {
		  printk("Filling Memory\n");
		  lls_cur = &lls_start;
		  while (mem_largest_available() > 8)
		  {
                          *lls_cur = mem_alloc(sizeof(lls));
                          (*lls_cur)->ptr = (void *)mem_alloc(mem_largest_available()-4);
                          printk("  Allocated %u K\n",*(int *)((unsigned int)((*lls_cur)->ptr) - 4)/1024);
                          lls_cur = &((*lls_cur)->next);
		  }
//		  printk("Setting next to null of [%.8x].next = [%.8x]\n",(*lls_cur),(*lls_cur)->next);
		  printk("Setting last to null\n");
		  (*lls_cur) = NULL;
		  printk("Done Allocating, checking for any extra mem\n");
                  
		  if (mem_largest_available() > 4)
		  {   
                          test_ptr = mem_alloc(mem_largest_available() - 4);
                          printk("test_ptr = 0x%x",(unsigned int)test_ptr);
		  }
		  printk("List: \n");
		  lls_cur = &lls_start;
		  while (*lls_cur != NULL)
		  {
			  printk("[%.8x] = [%.8x] -> [%.8x]\n",*lls_cur, (*lls_cur)->ptr, (*lls_cur)->next);
			  lls_cur = &((*lls_cur)->next);
		  }
	  }
	  else //free all
	  {
//#define pause() while(!keyboard_available()); keyboard_read();
		  printk("Freeing Memory\n");
//		  test_key('l');
//		  printk("\n");
//		  pause();
		  if (test_ptr != NULL)
		  {
			  printk("Freeing Extra Pointer\n");
			  mem_free(test_ptr);
//			  test_key('l');
//			  printk("\n");
//			  pause();
		  }
		  while (lls_start != NULL)
		  {
                          test_ptr = lls_start->ptr;
                          lls_node = (lls_start->next);
			  printk("  Freeing lls node [%x] with next [%x]\n",lls_start,lls_node);
			  mem_free(lls_start);
//			  test_key('l');
//			  printk("\n");
			  printk("  Freeing %u K\n",*(int *)((unsigned int)test_ptr - 4)/1024);
			  lls_start = lls_node;
//                        test_key('l');
			  mem_free(test_ptr);
//			  test_key('l');
//			  printk("\n");
//			  pause();
		  }
		  test_ptr = NULL;
	  }
	  break;
        default:
          printk("[mem_test] unknown command\n");
      }
}

