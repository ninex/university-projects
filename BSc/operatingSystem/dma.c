#include "include/asm.h"
#include "include/error.h"
#include "include/io.h"
#include "include/dma.h"
#include "include/trace.h"
#include "include/i386.h"

/* ----- Total number of DMA controller chips --------------------------------------------------- */
#define DMA_MAX_CONTROLLERS 2

/* ----- Total number of DMA channels ----------------------------------------------------------- */
#define DMA_MAX_CHANNELS 8

/* ----- Upper bound for DMA buffers (16MB) ----------------------------------------------------- */
#define DMA_LIMIT 0x1000000

/* ----- DMA Mode register fields --------------------------------------------------------------- */
#define DMA_MODE_DEMAND    0x00
#define DMA_MODE_SINGLE    0x40
#define DMA_MODE_BLOCK     0x80
#define DMA_MODE_CASCADE   0xC0
#define DMA_MODE_INCREMENT 0x00
#define DMA_MODE_DECREMENT 0x20
#define DMA_MODE_AUTO      0x10
#define DMA_MODE_NO_AUTO   0x00
#define DMA_MODE_VERIFY    0x00
#define DMA_MODE_WRITE     0x04
#define DMA_MODE_READ      0x08

/* ----- DMA Mask register fields --------------------------------------------------------------- */
#define DMA_MASK_SET 0x04

#ifdef DEBUG_DMA
  #define PRINT_DMA(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_DMA(x, ...)
#endif

/* ----- Intel 8237/8237A DMA controller registers ---------------------------------------------- */
static unsigned char dma_page_register[DMA_MAX_CHANNELS] = {0x87, 0x83, 0x81, 0x82,
                                                            0x8f, 0x8b, 0x89, 0x8a};
static unsigned char dma_countdown_register[DMA_MAX_CHANNELS] = {0x01, 0x03, 0x05, 0x07,
                                                                 0xc2, 0xc6, 0xca, 0xce};
static unsigned char dma_offset_register[DMA_MAX_CHANNELS] = {0x00, 0x02, 0x04, 0x06,
                                                              0xc0, 0xc4, 0xc8, 0xcc};
static unsigned char dma_status_register[DMA_MAX_CONTROLLERS] = {0x08, 0xd0};
static unsigned char dma_mode_register[DMA_MAX_CONTROLLERS] = {0x0b, 0xd6};
static unsigned char dma_clear_flip_flop_register[DMA_MAX_CONTROLLERS] = {0x0c, 0xd8};
static unsigned char dma_mask_register[DMA_MAX_CONTROLLERS] = {0x0a, 0xd4};

/* ----- DMA channel registery used to track allocations ---------------------------------------- */
static unsigned int dma_channels,dma_allocated;

int dma_request_channel(int channel) {
 
  /* ----- Check if a valid channel number was requested ---------------------------------------- */
  if ((channel < 0) || (channel >= DMA_MAX_CHANNELS))
  {
    return -ERR_DMA_INVALID_CHANNEL;
  }
  /* ----- Check if the requested channel is reserved ------------------------------------------- */
  if (bt(dma_channels,channel))
  {
    return -ERR_DMA_RESERVED_CHANNEL;
  }
  /* ----- Check if the requested channel is available ------------------------------------------ */
  if (bt(dma_allocated,channel))
  {
    return -ERR_DMA_CHANNEL_ALLOCATED;
  }
  
  /* ----- Mark the channel as allocated -------------------------------------------------------- */
  bts(dma_allocated,channel);
  
  return OK;
}

int dma_release_channel(int channel) {

  /* ----- Check if a valid channel number was requested ---------------------------------------- */
  if ((channel < 0) || (channel >= DMA_MAX_CHANNELS))
  {
    return -ERR_DMA_INVALID_CHANNEL;
  }
  /* ----- Check if the requested channel is reserved ------------------------------------------- */
  if (bt(dma_channels,channel))
  {
    return -ERR_DMA_RESERVED_CHANNEL;
  }
  /* ----- Check if the requested channel is allocated ------------------------------------------ */
  if (bt(dma_allocated,channel) == 0)
  {
    return -ERR_DMA_CHANNEL_RELEASED;
  }
  /* ----- Mark the channel as available -------------------------------------------------------- */
  btr(dma_allocated,channel);
  
  return OK;
}

int dma_start(int channel, int operation, void *buffer, unsigned int length) {
  //get which controller we're using
  unsigned int controller = channel / 4;

  //get the type of operation we want to do
  if (operation == DMA_MEMORY_TO_IO)
  {
    operation = DMA_MODE_READ;
  }else
  {
    operation = DMA_MODE_WRITE;
  }
  
  /* ----- Check if a valid channel number is requested ----------------------------------------- */
  if ((channel < 0) || (channel >= DMA_MAX_CHANNELS))
  {
    return -ERR_DMA_INVALID_CHANNEL;
  }
  
  /* ----- Check if the requested channel is reserved ------------------------------------------- */
  if (bt(dma_channels,channel))
  {
    return -ERR_DMA_RESERVED_CHANNEL;
  }

  /* ----- Verify that the buffer length is valid for the specified channel --------------------- */
  if (channel < 4)
  {
    //we want bigger than 64k for a 64k buffer
    if (length > 0x10000)
    {
      return -ERR_DMA_INVALID_BUFFER_LENGTH;
    }
  }else
  {
    //we want bigger than 128k for a 128k buffer
    if (length > 0x20000)
    {
      return -ERR_DMA_INVALID_BUFFER_LENGTH;
    }
  }
  
  /* ----- Test address against upper memory limit ---------------------------------------------- */
  if ((unsigned int)buffer + length >= 0x1000000)
  {
    return -ERR_DMA_LIMIT;
  }
  
  /* ----- Are the buffers properly aligned for the specific channel ---------------------------- */
  /*
   * Test for a possible boundary overrun by checking whether the start and end addresses fall
   * within the same DMA page (64K/128K block)
   */
  if (channel < 4)
  {
    //is the 64k buffer not aligned on a 64k page
    if ((i386_align((unsigned int)buffer,0x20000) == (unsigned int)buffer) && (i386_align((unsigned int)buffer,0x10000) != (unsigned int)buffer))
    {
      return -ERR_DMA_BUFFER_ALIGN;
    }
  }else
  {
    //is the 128k buffer not aligned on a 128k page
    if (i386_align((unsigned int)buffer,0x20000) != (unsigned int)buffer)
    {
      return -ERR_DMA_BUFFER_ALIGN;
    }
  }  

  /* ----- Always select single mode, no auto-initialization and address increment -------------- */
   
  
  /* ----- Disable interrupts ------------------------------------------------------------------- */
  unsigned int eflags = disable_interrupts();
  
  /* ----- Reset the channel and disable DMA ---------------------------------------------------- */
  outb((channel % 4) | DMA_MASK_SET,dma_mask_register[controller]);
  
  /* ----- Set Clear flip-flop register --------------------------------------------------------- */
  outb(0x0,dma_clear_flip_flop_register[controller]);
  
  /* ----- Set the Mode register ---------------------------------------------------------------- */
  outb(DMA_MODE_SINGLE | DMA_MODE_NO_AUTO | DMA_MODE_INCREMENT | operation | (channel % 4),dma_mode_register[controller]);
  
  /* ----- Set the Page register ---------------------------------------------------------------- */
  outb((unsigned int)buffer >> 16,dma_page_register[channel]);
  
  /* ----- Set the Offset and Countdown registers ----------------------------------------------- */
  length--;
  outb(length,dma_countdown_register[channel]);
  outb(length >> (8 + (channel / 4)),dma_countdown_register[channel]);
  
  outb(buffer,dma_offset_register[channel]);
  outb((unsigned int)buffer >> (8 + (channel / 4)),dma_offset_register[channel]);
  
  /* ----- Enable DMA and restore interrupt status ---------------------------------------------- */
  outb((channel % 4),dma_mask_register[controller]);
  restore_flags(eflags);

  return OK;
}

void dma_initialize() {
  /* ----- Mark all channels as available (7, 6, 5, 3 and 2) ------------------------------------ */
  dma_channels = 0x13;
  dma_allocated = 0x0;
}
