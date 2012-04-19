/*
 *   DESCRIPTION: Definition of error codes. Functions should always return a negative value, for example
 *                'return -ERR_INVALID_BT_INDEX'
 *
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 27.01.2005
 *     CHANGELOG:
 *                27.01.2005 - Additional error codes for FDC and TTY added
 *                14.09.2004 - Additional comments added to clarify definitions
 *                27.05.2004 - Original version
 */
#ifndef ERROR_H
#define ERROR_H

/* ----- '0' indicates that no error occured ---------------------------------------------------- */
#define OK 0

/* ----- Boot table (1..99) --------------------------------------------------------------------- */
#define ERR_INVALID_BT_INDEX   1 /* Invalid boot table entry specified */
#define ERR_INVALID_SMAP_INDEX 2 /* Invalid SMAP entry requested */

/* ----- i386 (100..199) ------------------------------------------------------------------------ */
#define ERR_I386_GDT_FULL    100 /* GDT contains no empty slots */
#define ERR_I386_INVALID_TSS 101 /* NULL task state segment */

/* ----- i8259 (200..299 ------------------------------------------------------------------------ */
#define ERR_I8259_INVALID_VECTOR 200 /* Vector number out of bounds */
#define ERR_I8259_INSTALLED      201 /* Vector already contains a valid handler */
#define ERR_I8259_REMOVED        202 /* Vector does not contain a valid handler */

/* ----- dma (300..399) ------------------------------------------------------------------------- */
#define ERR_DMA_CHANNEL_ALLOCATED     300  /* DMA channel is already allocated */
#define ERR_DMA_CHANNEL_RELEASED      301  /* DMA channel is already released */
#define ERR_DMA_INVALID_CHANNEL       302  /* Invalid DMA channel number */
#define ERR_DMA_RESERVED_CHANNEL      303  /* DMA channel is reserved */
#define ERR_DMA_INVALID_OPERATION     304  /* Invalid DMA operation */
#define ERR_DMA_INVALID_BUFFER_LENGTH 307  /* Invalid buffer length */
#define ERR_DMA_INVALID_CONTROLLER    306  /* Invalid controller number */
#define ERR_DMA_BUFFER_ALIGN          307  /* DMA buffer not properly aligned */
#define ERR_DMA_BOUNDARY              309  /* DMA buffer and length will cause boundary crossing */
#define ERR_DMA_LIMIT                 310  /* DMA operation will exceed 16MB limit */

/* ----- floppy (400..499) ---------------------------------------------------------------------- */
#define ERR_FDC_INVALID_DRIVE_NUMBER 400 /* */
#define ERR_FDC_INVALID_SECTORS      401
#define ERR_FDC_BUFFER_TOO_SMALL     402
#define ERR_FDC_ILLEGAL_SECTOR       403 /* Physical sector out of bounds */
#define ERR_FDC_ILLEGAL_TRACK        404 /* Physical track out of bounds */
#define ERR_FDC_ILLEGAL_HEAD         405 /* Physical head out of bounds */
#define ERR_FDC_READ_FAILED          406
#define ERR_FDC_WRITE_FAILED         407
#define ERR_FDC_RESET_FAILED         408
#define ERR_FDC_SPECIFY_FAILED       409 /* SPECIFY command failed */
#define ERR_FDC_RECALIBRATE_FAILED   410
#define ERR_FDC_MEDIA_CHANGED        411 /* Disk media unknown */
#define ERR_FDC_TOO_MANY_RETRIES     412 /* Operation failed after 3 retries */
#define ERR_FDC_TIMEOUT              413 /* Hardware operation timed out */
#define ERR_FDC_NULL_BUFFER          414 /* Buffer address is NULL */
#define ERR_FDC_ILLEGAL_OPERATION    415 /* Operation != (OP_READ, OP_WRITE) */

/* ----- tty (500..599) ------------------------------------------------------------------------- */
#define ERR_TTY_INVALID_HANDLE       500

/* ----- comms (600..699) ----------------------------------------------------------------------- */
#define ERR_IPC_DUPLICATE_PORT       600
#define ERR_IPC_NO_SUCH_PORT         601
#define ERR_IPC_INVALID_MSG_SIZE     602
#define ERR_IPC_INVALID_PORT         603
#define ERR_IPC_INVALID_BUFFER       604
#define ERR_IPC_REPLY_MISMATCH       605
#define ERR_IPC_CLOSE_PORT           606

#endif
