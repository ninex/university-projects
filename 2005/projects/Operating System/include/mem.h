#ifndef MEM_H
#define MEM_H

#include "i386.h"

#define FOUR_MB 0x400000

/*
 * DESCRIPTION: Switch to the kernel page directory
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void mem_switch_to_kernel_directory();

/*
 * DESCRIPTION: Allocates a 4K page of memory and returns a pointer to the page.
 *              The address of the page is guaranteed to be aligned on a
 *              page boundary (4K)
 *       INPUT: NONE
 *      OUTPUT: A pointer to the allocated pages or NULL if the operation failed
 */
void *mem_allocate_page();

/*
 * DESCRIPTION: Allocates a block of memory and returns a pointer to
 *              start address of the block.
 *       INPUT: size - The size of the requested block in bytes
 *      OUTPUT: A pointer to the allocated block or NULL if the operation failed
 */
void *mem_alloc(unsigned int size);

/*
 * DESCRIPTION: Allocates a block of memory for a DMA buffer and returns
 *              a pointer to the start address of the block.
 *       INPUT: channel - The DMA channel that will be associated with
 *              the DMA buffer. The channel number determines the
 *              location and size of the DMA buffer:
 *              0..3: 64K buffer, aligned on a 64K boundary
 *              4..7: 128K buffer, aligned on a 128K boundary
 *      OUTPUT: A pointer to the allocated block or NULL if the operation failed.
 */
void *mem_allocate_dma(int channel);

void mem_free_page(void *p);

void mem_free_dma(void *p);

void mem_free(void *p);

/*
 * DESCRIPTION: Returns the total amount of available memory in bytes
 *       INPUT: NONE
 *      OUTPUT: Returns the total amount of available memory in bytes
 */
unsigned int mem_available();

/*
 * DESCRIPTION: Returns the size (in bytes) of the largest contigious block
 *              of memory
 *       INPUT: NONE
 *      OUTPUT: Returns the size (in bytes) of the largest contigious block
 *              of memory
 */
unsigned int mem_largest_available();

/*
 * DESCRIPTION:
 *       INPUT:
 *      OUTPUT:
 */
void mem_copy_kernel_page_directory(page_directory *page_dir);


/*
 * DESCRIPTION: Initializes the memory management system
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void mem_initialize();

#endif
