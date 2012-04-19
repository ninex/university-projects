#ifndef DMA_H
#define DMA_H

/* ----- DMA transfer operations ---------------------------------------------------------------- */
#define DMA_MEMORY_TO_IO 0 /* Mode register selection: DMA Read */
#define DMA_IO_TO_MEMORY 1 /* Mode register selection: DMA Write */

/*
 * DESCRIPTION: Allocates a DMA channel.
 *       INPUT: channel - DMA that must be allocated.
 *      OUTPUT: Returns OK if successful, otherwise an error code indicating
 *              why the operation failed.
 */
int dma_request_channel(int channel);

/*
 * DESCRIPTION: Releases a DMA channel previously allocated with 'dma_request_channel',
 *              making it available to other devices.
 *       INPUT: channel - DMA channel that must be released.
 *      OUTPUT: Returns OK if successful, otherwise an error code indicating
 *              why the operation failed.
 */
int dma_release_channel(int channel);

/*
 * DESCRIPTION: Initialize the DMA controller for a transfer operation
 *       INPUT: channel - DMA channel that will be used for transfer
 *              operation - Type of DMA transfer (DMA_MEMORY_TO_IO or DMA_IO_TO_MEMORY)
 *              buffer - Pointer to DMA buffer
 *              length - Number of bytes that will be transferred
 *      OUTPUT: Returns OK if successful, otherwise an error code indicating
 *              why the operation failed.
 */
int dma_start(int channel, int operation, void *buffer, unsigned int length);

/*
 * DESCRIPTION: Initialize DMA structures to perform channel management
 *       INPUT: NONE
 *      OUTPUT: NONE
 */
void dma_initialize();

#endif
