/*
 *        MODULE: floppy.c
 *   DESCRIPTION: This module provides a simple floppy driver for NEC765 compatible
 *                devices.
 *        AUTHOR: Jacques Eloff
 *                University of Stellenbosch
 *                eloff@cs.sun.ac.za
 * LAST MODIFIED: 07.03.2005
 *     CHANGELOG:
 *                07.03.2005 - Added floppy_write() function and media cycling
 *                17.01.2005 - Modified most of the code to work with the
 *                             static drive/media structures.
 *                14.12.2004 - Ported from original Oberon implementation
 *    REFERENCES: - 82077AA CHMOS Single-Chip Floppy Disk Controller, Intel, Order-no: 290166-007
 *                - PC87311A/PC87312 (Super I/O II/III) Floppy Disk Controller with Dual UARTs,
 *                  Parallel Port and IDE Interface, National Semiconductor, 1993
 *                - Frank van Gilluwe, Undocumented PC, Addison-Wesley
 *                - GFloppy.Mod (Gneiss microkernel)
 *                - Diskette.Mod (Oberon kernel)
 *                - floppy.c, floppy.h and fd.h (Linux kernel)
 */

#include "include/asm.h"
#include "include/io.h"
#include "include/error.h"
#include "include/stddef.h"
#include "include/boottable.h"
#include "include/trace.h"
#include "include/i386.h"
#include "include/i8259.h"
#include "include/i8253.h"
#include "include/mem.h"
#include "include/dma.h"
#include "include/cmos.h"
#include "include/floppy.h"

/*
 * Maximum number of drives supported by a single controller. Starting with the AT, the BIOS no
 * longer supports four drives. Consequently, support is limited to 2 diskette drives
 */
#define MAX_DRIVES  2

#define MAX_SECTORS (65536/SECTOR_SIZE)
#define MAX_RESULTS 7
#define MAX_ERRORS  3

/* ----- FDC's DMA channel is fixed ------------------------------------------------------------- */
#define FDC_DMA_CHANNEL 2

/* ----- Controller I/O base address ------------------------------------------------------------ */
#define FDC_BASE 0x3f0

/* ----- Digital Output Register fields --------------------------------------------------------- */
#define DOR_DR0  0x01  /* Drive selection bit 0 */
#define DOR_DR1  0x02  /* Drive selection bit 1 */
#define DOR_REST 0x04  /* Reset */
#define DOR_DMA  0x08  /* DMA Gate */
#define DOR_MOTA 0x10  /* Motor enable drive 0 */
#define DOR_MOTB 0x20  /* Motor enable drive 1 */
#define DOR_MOTC 0x40  /* Motor enable drive 2 */
#define DOR_MOTD 0x80  /* Motor enable drive 3 */

/* ----- Main Status Register fields ------------------------------------------------------------ */
#define MSR_ACTA 0x01 /* Drive 0 active */
#define MSR_ACTB 0x02 /* Drive 1 active */
#define MSR_ACTC 0x04 /* Drive 2 active */
#define MSR_ACTD 0x08 /* Drive 3 active */
#define MSR_BUSY 0x10 /* FDC busy executing a command */
#define MSR_NDMA 0x20 /* Non-DMA */
#define MSR_DIO  0x40 /* Data I/O direction */
#define MSR_RQM  0x80 /* Request for Master */

/* ----- Status Register 0 bit fields (result phase) -------------------------------------------- */
#define ST0_DS0 0x01 /* Drive selection */
#define ST0_DS1 0x02 /* Drive selection */
#define ST0_H   0x04 /* Head address */
#define ST0_RES 0x80 /* Reserved - should always be 0 */
#define ST0_EC  0x10 /* Equipment check - TRK0 pin failed to become 1 */
#define ST0_SE  0x20 /* Seek end - SEEK, RECALIBRATE, READ/WRITE with implied seek completed */
#define ST0_IC0 0x40 /* Interrupt code 0 - 00: Normal termination, 01, Abnormal termination */
#define ST0_IC1 0x80 /* Interrupt code 1 - 10: Invalid command, 11 Abnormal termination - polling */

/* ----- Diskette controller commands ----------------------------------------------------------- */
#define CMD_SPECIFY                0x03 /* All controllers */
#define CMD_SENSE_DRIVE_STATUS     0x04 /* All controllers */
#define CMD_RECALIBRATE            0x07 /* All controllers */
#define CMD_SENSE_INTERRUPT_STATUS 0x08 /* All controllers */
#define CMD_DUMP_REGISTERS         0x0e /* Enhanced controllers */
#define CMD_SEEK                   0x0f /* All controllers */
#define CMD_VERSION                0x10 /* All controllers */
#define CMD_PERPENDICULAR_MODE     0x12 /* Enhanced controllers */
#define CMD_CONFIGURE              0x13 /* Enhanced controllers */
#define CMD_UNLOCK_FIFO_FUNCTIONS  0x14 /* Enhanced controllers */
#define CMD_EXIT_STANDBY_MODE      0x34 /* 72065 only */
#define CMD_ENTER_STANDBY_MODE     0x35 /* 72065 only */
#define CMD_HARD_RESET             0x36 /* 72065 only */
#define CMD_READ_TRACK             0x42 /* All controllers */
#define CMD_READ_ID                0x4a /* All controllers */
#define CMD_FORMAT_TRACK           0x4d /* All controllers */
#define CMD_RELATIVE_SEEK_OUTWARD  0x8f /* Enhanced controllers */
#define CMD_LOCK_FIFO_FUNCTIONS    0x94 /* Enhanced controllers */
#define CMD_WRITE_DATA             0xc5 /* All controllers */
#define CMD_READ_DATA              0xc6 /* All controllers */
#define CMD_WRITE_DELETED_DATA     0xc9 /* All controllers */
#define CMD_READ_DELETED_DATA      0xcc /* All controllers */
#define CMD_RELATIVE_SEEK_INWARD   0xcf /* Enhanced controllers */
#define CMD_SCAN_EQUAL_ALL         0xd1 /* All controllers */
#define CMD_VERIFY                 0xd6 /* Enhanced controllers */
#define CMD_READ_NORMAL_DATA       0xe6 /* All controllers */
#define CMD_SCAN_EQUAL             0xf1 /* All controllers */
#define CMD_VERIFY_F6              0xf6 /* Enhanced controllers */

/* ----- Disk operations ------------------------------------------------------------------------ */
#define OP_READ 0
#define OP_WRITE 1

/* ----- Drive types according to CMOS register 0x10 -------------------------------------------- */
#define T0      0 /* No drive */
#define T360    1 /* 360K 5.25" drive */
#define T1200   2 /* 1.2Mb 5.25# drive */
#define T720    3 /* 720K 3.5" drive */
#define T1440   4 /* 1.44Mb 3.5" drive */
#define T2880_1 5 /* 2.88Mb 3.5" drive */
#define T2880_2 6 /* 2.88Mb 3.5" drive */

#define TIME_OUT_MRQ 4000
#define TIME_OUT_RESULT_PHASE 4000
#define MAX_CONTROLLER_TYPES 7
#define MAX_MEDIA_CYCLE 8          /* Maximum number of media that will be cycled */

/* ----- Floppy drive controller types ---------------------------------------------------------- */
#define T_FDC_NONE     0 /* No controller present */
#define T_FDC_UNKNOWN  1 /* Assume that FDC is a very basic controller */
#define T_FDC_8272A    2 /* Intel 8272a or NEC 765 */
#define T_FDC_82072    3 /* Intel 82072: 8272a supporting FIFO and Dump Registers command */
#define T_FDC_82077AA  4 /* Original 82077AA controller */
#define T_FDC_82077AA1 5 /* 82077AA-1 controller */

/* ----- Floppy drive controller states (refer to 'do_request') --------------------------------- */
#define STATE_RESET       0
#define STATE_READY       1
#define STATE_RECALIBRATE 2

#define MAX_LOOPS   18
#define MAX_RETRIES 1

#ifdef DEBUG_FLOPPY
  #define PRINT_FLOPPY(x, ...) printk(x, ## __VA_ARGS__)
#else
  #define PRINT_FLOPPY(x, ...)
#endif

typedef struct media_parameters {
  int size;     /* Total number of sectors (sectors*heads*tracks) */
  int sectors;  /* Sectors per track */
  int heads;    /* Number of disk heads */
  int tracks;   /* Number of tracks */
  int stretch;
  char gap;     /* Gap 1 size */
  char rate;    /* Data rate ( | 0x40 for perpendicular mode) */
  char spec1;   /* SPEC 1 */
  char spec2;   /* SPEC 2 */
} media_parameters;

typedef struct drive_parameters {
  /* ----- General information ------------------------------------------------------------------ */
  unsigned char drive_type;                   /* CMOS Drive type */
  unsigned int media_type;                    /* Default media type to use */

  /* ----- Fixed parameters for drive type ------------------------------------------------------ */
  char name[16];
  unsigned int max_dtr;                       /* */
  unsigned int hlt;                           /* Head load time */
  unsigned int hut;                           /* */
  unsigned int srt;                           /* */
  unsigned int spin_up;                       /* */
  unsigned int spin_down;                     /* */
  unsigned char spin_down_offset;             /* */
  unsigned char select_delay;                 /* */
  unsigned int rps;                           /* Rotations per seccond */
  unsigned int max_tracks;                    /* */
  unsigned int irq_timeout;                   /* */
  unsigned char cycle_media[MAX_MEDIA_CYCLE]; /* List of media types to use when cycling */
} drive_parameters;

typedef struct drive_state {
  int media_index;             /* Current media index to use for cycle_media[] */
  unsigned char motor;         /* Current drive motor state */
  unsigned int  current_track; /* Last known track */
  drive_parameters d_par;      /* Fixed parameters for drive */
  media_parameters m_par;      /* Variable parameters for media */
} drive_state;

typedef struct fd_controller {
  /* ------ Registers --------------------------------------------------------------------------- */
  unsigned int SRA;  /* Status Register A (PS/2 models) */
  unsigned int DR;   /* Diagnostic Register */
  unsigned int SRB;  /* Status Register B (PS/2 models) */
  unsigned int DOR;  /* Digital Output Register (All controllers) */
  unsigned int TDR;  /* Tape Drive Register */
  unsigned int DSR;  /* Drive Status Register (PS/2 models) */
  unsigned int MSR;  /* Main Status Register (All controllers) */
  unsigned int DRSR; /* Data Rate Select Register */
  unsigned int DATA; /* Data Register */
  unsigned int DIR;  /* Digital Input Register (AT+) */
  unsigned int CCR;  /* Configuration Control Register (AT+) */

  char id[32];
  int state;                          /* Current controller state */
  unsigned char interrupt;
  unsigned char installed;
  unsigned int drives;                /* Bit mask indicating detected drives */
  unsigned char results[MAX_RESULTS]; /* */
  unsigned char errors[MAX_ERRORS];
  drive_state drive[MAX_DRIVES];      /* Individual drive information */
  void *dma_buffer;                   /* Pointer to DMA buffer (shared between drives) */
} fd_controller;

/* ----- Fixed parameters related to specific drive types, irrespective of media ---------------- */
static drive_parameters dp[] = {
  {T0,      0, "unknown", 500,  16, 16, 8000, 1000, 3000, 0,  20, 5, 80, 3000,
   {7, 4, 8, 2, 1, 5, 3, 10}},
  {T360,    1, "360K",    300,  16, 16, 8000, 1000, 3000, 0,  20, 5, 40, 3000,
   {1, 0, 0, 0, 0, 0, 0, 0}},
  {T1200,   2, "1.2Mb",   500,  16, 16, 6000, 400,  3000, 0,  20, 5, 83, 3000,
   {2, 5, 6, 23, 10, 20, 12, 0}},
  {T720,    4, "720K",    250,  16, 16, 3000, 1000, 3000, 0,  20, 5, 83, 3000,
   {4, 22, 21, 30, 3, 0, 0, 0}},
  {T1440,   7, "1.44Mb",  500,  16, 16, 4000, 400,  3000, 10, 20, 5, 83, 3000,
   {7, 4, 25, 22, 31, 21, 29, 11}},
  {T2880_1, 8, "2.88Mb",  1000, 15, 8,  3000, 400,  3000, 10, 20, 5, 83, 3000,
   {7, 8, 4, 25, 28, 22, 31, 21}},
  {T2880_2, 8, "2.88Mb",  1000, 15, 8,  3000, 400,  3000, 10, 20, 5, 83, 3000,
   {7, 8, 4, 25, 28, 22, 31, 21}}
};

/* ----- Fixed media parameters ("floppy.c" - Linux kernel) ------------------------------------- */
static media_parameters mp[] = {
  {    0,  0, 0,  0, 0, 0x00, 0x00, 0x00, 0x00},
  {  720,  9, 2, 40, 0, 0x2a, 0x02, 0xdf, 0x50}, /*  1 - 360KB PC */
  { 2400, 15, 2, 80, 0, 0x1b, 0x00, 0xdf, 0x54}, /*  2 - 1.2MB AT */
  {  720,  9, 1, 80, 0, 0x2a, 0x02, 0xdf, 0x50}, /*  3 - 360KB SS 3.5" */
  { 1440,  9, 2, 80, 0, 0x2a, 0x02, 0xdf, 0x50}, /*  4 - 720KB 3.5" */
  {  720,  9, 2, 40, 1, 0x23, 0x01, 0xdf, 0x50}, /*  5 - 360KB AT */
  { 1440,  9, 2, 80, 0, 0x23, 0x01, 0xdf, 0x50}, /*  6 - 720KB AT */
  { 2880, 18, 2, 80, 0, 0x1b, 0x00, 0xcf, 0x6c}, /*  7 - 1.44MB 3.5" */
  { 5760, 36, 2, 80, 0, 0x1b, 0x43, 0xaf, 0x54}, /*  8 - 2.88MB 3.5" */
  { 6240, 39, 2, 80, 0, 0x1b, 0x43, 0xaf, 0x28}, /*  9 - 3.12MB 3.5" */
  { 2880, 18, 2, 80, 0, 0x25, 0x00, 0xdf, 0x02}, /* 10 - 1.44MB 5.25" */
  { 3360, 21, 2, 80, 0, 0x1c, 0x00, 0xcf, 0x0c}, /* 11 - 1.68MB 3.5" */
  {  820, 10, 2, 41, 1, 0x25, 0x01, 0xdf, 0x2e}, /* 12 - 410KB 5.25" */
  { 1640, 10, 2, 82, 0, 0x25, 0x02, 0xdf, 0x2e}, /* 13 - 820KB 3.5" */
  { 2952, 18, 2, 82, 0, 0x25, 0x00, 0xdf, 0x02}, /* 14 - 1.48MB 5.25" */
  { 3444, 21, 2, 82, 0, 0x25, 0x00, 0xdf, 0x0c}, /* 15 - 1.72MB 3.5" */
  {  840, 10, 2, 42, 1, 0x25, 0x01, 0xdf, 0x2e}, /* 16 - 420KB 5.25" */
  { 1660, 10, 2, 83, 0, 0x25, 0x02, 0xdf, 0x2e}, /* 17 - 830KB 3.5" */
  { 2988, 18, 2, 83, 0, 0x25, 0x00, 0xdf, 0x02}, /* 18 - 1.49MB 5.25" */
  { 3486, 21, 2, 83, 0, 0x25, 0x00, 0xdf, 0x0c}, /* 19 - 1.74 MB 3.5" */
  { 1760, 11, 2, 80, 0, 0x1c, 0x09, 0xcf, 0x00}, /* 20 - 880KB 5.25" */
  { 2080, 13, 2, 80, 0, 0x1c, 0x01, 0xcf, 0x00}, /* 21 - 1.04MB 3.5" */
  { 2240, 14, 2, 80, 0, 0x1c, 0x19, 0xcf, 0x00}, /* 22 - 1.12MB 3.5" */
  { 3200, 20, 2, 80, 0, 0x1c, 0x20, 0xcf, 0x2c}, /* 23 - 1.6MB 5.25" */
  { 3520, 22, 2, 80, 0, 0x1c, 0x08, 0xcf, 0x2e}, /* 24 - 1.76MB 3.5" */
  { 3840, 24, 2, 80, 0, 0x1c, 0x20, 0xcf, 0x00}, /* 25 - 1.92MB 3.5" */
  { 6400, 40, 2, 80, 0, 0x25, 0x5b, 0xcf, 0x00}, /* 26 - 3.20MB 3.5" */
  { 7040, 44, 2, 80, 0, 0x25, 0x5b, 0xcf, 0x00}, /* 27 - 3.52MB 3.5" */
  { 7680, 48, 2, 80, 0, 0x25, 0x63, 0xcf, 0x00}, /* 28 - 3.84MB 3.5" */
  { 3680, 23, 2, 80, 0, 0x1c, 0x10, 0xcf, 0x00}, /* 29 - 1.84MB 3.5" */
  { 1600, 10, 2, 80, 0, 0x25, 0x02, 0xdf, 0x2e}, /* 30 - 800KB 3.5" */
  { 3200, 20, 2, 80, 0, 0x1c, 0x00, 0xcf, 0x2c}, /* 31 - 1.6MB 3.5" */
};

static fd_controller fdc;

void show_dp(drive_parameters *drv_par) {
  printk("[show_dp]\n");
  printk("  drive_type=%x, media_type=%x, name=%s\n", drv_par->drive_type, drv_par->media_type, drv_par->name);
  printk("  max_dtr=%d, hlt=%d, hut=%d, srt=%d\n", drv_par->max_dtr, drv_par->hlt, drv_par->hut, drv_par->srt);
  printk("  spin_up=%d, spin_down=%d, spin_down_offset=%d, select_delay=%d\n", drv_par->spin_up, drv_par->spin_down,
         drv_par->spin_down_offset, drv_par->select_delay);
  printk("  rps=%d, max_tracks=%d, irq_timeout=%d\n", drv_par->rps, drv_par->max_tracks, drv_par->irq_timeout);
}

int put_byte(unsigned char c) {
  int t;

  t = timer_set(4000);
  do {
    /* ----- Only write to controller when DIO is cleared and RQM is set ------------------------ */
    if ((inb(fdc.MSR) & (MSR_RQM | MSR_DIO)) == MSR_RQM) {
      outb(c, fdc.DATA);
      return OK;
    }
  } while (!timer_expired(t));
  return -ERR_FDC_TIMEOUT;
}

void stop_motor(int drive_number) {
  outb((DOR_DMA | DOR_REST), fdc.DOR);
  fdc.drive[drive_number].motor = 0;
  //fdc.drive[drive_number].m_par = mp[0];
}

void wait_for_interrupt() {
  int ticks;
  if (fdc.state != STATE_RESET) {
    ticks = timer_get_ticks();
    do {
    } while ((!fdc.interrupt) && (timer_get_ticks()-ticks < 100));
    if (!fdc.interrupt)
      fdc.state = STATE_RESET;
    fdc.interrupt = 0;
  }
}

int get_results() {
  int ticks;
  int i;
  unsigned char msr;
  unsigned char data;

  if (fdc.state != STATE_RESET) {
    ticks = timer_get_ticks();
    i = 0;
    do {
      /* ----- Read the main status register ---------------------------------------------------- */
      msr = inb(fdc.MSR);
      if ((msr & (MSR_BUSY | MSR_DIO | MSR_RQM)) == MSR_RQM) {
        return i;
      } else if ((msr & (MSR_DIO | MSR_RQM)) == (MSR_DIO | MSR_RQM)) {
        data = inb(fdc.DATA);
        if (i < MAX_RESULTS)
          fdc.results[i] = data;
        i++;
      }
    } while ((timer_get_ticks() - ticks) < 50);
    fdc.state = STATE_RESET;
  }
  return -1;
}

unsigned char handler_floppy(cpu_state *state, int n) {
  fdc.interrupt = 1;
  return 0;
}

void error() {
  int i;

  for (i = 0; i < MAX_RESULTS; i++)
    fdc.results[i] = 0;
  for (i = 0; i < MAX_ERRORS; i++)
    fdc.errors[i] = 0;
  fdc.state = STATE_RESET;
}

void cycle_media(drive_state *drive) {
  PRINT_FLOPPY("[cycle_media] media_index: %d, media_type: %d\n", drive->media_index,
               drive->d_par.cycle_media[drive->media_index]);
  do {
    drive->media_index = (drive->media_index+1) % MAX_MEDIA_CYCLE;
  } while (drive->d_par.cycle_media[drive->media_index] == 0);
  drive->m_par = mp[drive->media_index];
  PRINT_FLOPPY("[cycle_media] new media_index: %d, new media_type: %d\n", drive->media_index,
               drive->d_par.cycle_media[drive->media_index]);
}

int do_request(int drive_number, int operation, int sector,
               int head, int track, int n, void *buffer) {
  int ticks;
  unsigned char dor;
  unsigned char done;
  int t;
  int res;
  int loops;
  int try;
  int result;

  media_parameters *media;
  drive_state *drive;

  media = &fdc.drive[drive_number].m_par;
  drive = &fdc.drive[drive_number];

  /* ----- Verify disk operation ---------------------------------------------------------------- */
  if ((operation != OP_READ) && (operation != OP_WRITE))
    return -ERR_FDC_ILLEGAL_OPERATION;
  /* ----- Check physical sector, track and head against media specification -------------------- */
  if ((sector < 0) || (sector > media->sectors))
    return -ERR_FDC_ILLEGAL_SECTOR;
  if ((track < 0) || (track > media->tracks))
    return -ERR_FDC_ILLEGAL_TRACK;
  if ((head < 0) || (head > media->heads))
    return -ERR_FDC_ILLEGAL_HEAD;

  done = 0;
  loops = 0;
  try = 0;
  res = OK;

  while (done == 0) {
    /* ----- Power-up the drive motor and wait for it to spin-up ------------------------------- */
    outb((DOR_MOTA << drive_number) | DOR_DMA | DOR_REST, fdc.DOR);
    t = timer_set(800);
    do {
    } while (!timer_expired(t));
    drive->motor = 1;

    switch (fdc.state) {
      case STATE_READY:
        PRINT_FLOPPY("[do_request] STATE_READY - track/current_track=%d, %d\n", track, drive->current_track);
        if (drive->current_track != track) {
          /* ----- Execute seek command to locate proper track ---------------------------------- */
          PRINT_FLOPPY("[do_request] Executing SEEK command\n");
          put_byte(CMD_SEEK);
          put_byte((char)((head << 2)+drive_number));
          put_byte((char)(track));
          wait_for_interrupt();
          put_byte(CMD_SENSE_INTERRUPT_STATUS);
          PRINT_FLOPPY("[do_request] Executing SENSE INTERRUPT command\n");
          if ((get_results() < 1) ||
              ((fdc.results[0] & (ST0_IC1|ST0_IC0|ST0_SE|ST0_EC|ST0_RES)) != ST0_SE))
            fdc.state = STATE_RESET;
          else {
            PRINT_FLOPPY("[do_request] SEEK successful\n");
            drive->current_track = track;
          }
        }
        if (fdc.state != STATE_RESET) {
          /* ----- Initialize DMA controller ---------------------------------------------------- */
          switch (operation) {
            case OP_READ:
              result = dma_start(FDC_DMA_CHANNEL, DMA_IO_TO_MEMORY, fdc.dma_buffer, n*SECTOR_SIZE);
              if (result != OK)
                PRINT_FLOPPY("[do_request] dma_start failed for OP_READ! (%d)\n", result);
              else
                put_byte(CMD_READ_NORMAL_DATA);
              break;

            case OP_WRITE:
              result = dma_start(FDC_DMA_CHANNEL, DMA_MEMORY_TO_IO, fdc.dma_buffer, n*SECTOR_SIZE);
              if (result != OK)
                PRINT_FLOPPY("[do_request] dma_start failed for OP_WRITE!\n");
              else {
                copy(buffer, fdc.dma_buffer, n*SECTOR_SIZE);
                put_byte(CMD_WRITE_DATA);
              }
              break;

            default:
              printk("[do_request] Unknown operation (%d)\n", operation);
          }

          /* ----- Send read/write command parameters ----------------------------------------- */
          put_byte((char)((head << 2)+drive_number));
          put_byte((char)(track));
          put_byte((char)(head));
          put_byte((char)(sector));
          put_byte(0x02);
          put_byte((char)(media->sectors));
          put_byte((char)(media->gap));
          put_byte(0xff);
          wait_for_interrupt();
          if ((get_results() < 7) || (fdc.results[0] & (ST0_IC0 | ST0_IC1)))
            fdc.state = STATE_RESET;
        }      
        if (fdc.state == STATE_RESET) {
          try++;
          if (try == MAX_RETRIES) {
            /* ---- Try a different media type -------------------------------------------------- */
            try = 0;
            PRINT_FLOPPY("[do_request] cycling media\n");
            cycle_media(drive);
          }
          if (operation == OP_READ) {
            error();
            fdc.state = STATE_RESET;
            return -ERR_FDC_READ_FAILED;
          } else if (operation == OP_WRITE) {
            error();
            fdc.state = STATE_RESET;
            return -ERR_FDC_WRITE_FAILED;
          }
        } else {
          if (operation == OP_READ) {
            PRINT_FLOPPY("[do_request] copying data for read operation\n");
            copy(fdc.dma_buffer, buffer, n*SECTOR_SIZE);
          }
          done = 1;
        }
        break;

      case STATE_RESET:
        PRINT_FLOPPY("[do_request] STATE_RESET - ");
        drive->current_track = -1;
        fdc.interrupt = 0;
        dor = inb(fdc.DOR) & (~DOR_REST);
        PRINT_FLOPPY("dor=%#.2hhx\n", dor);
        outb(dor, fdc.DOR);
        ticks = timer_get_ticks();

        do {
        } while (timer_get_ticks()-ticks < 2);

        outb(dor | DOR_REST, fdc.DOR);
        fdc.state = STATE_RECALIBRATE;
        wait_for_interrupt();
        put_byte(CMD_SENSE_INTERRUPT_STATUS);
        if (get_results() < 1) {
          fdc.state = STATE_RESET;
          return -ERR_FDC_RESET_FAILED;
        }
        put_byte(CMD_SPECIFY);
        put_byte(media->spec1);
        put_byte(media->spec2);
        if (fdc.state == STATE_RESET) {
          return -ERR_FDC_SPECIFY_FAILED;
        }
        outb(fdc.CCR, media->rate);
        break;

      case STATE_RECALIBRATE:
        PRINT_FLOPPY("[do_request] STATE_RECALIBRATE\n");
        put_byte(CMD_RECALIBRATE);
        put_byte((char)(drive_number));
        wait_for_interrupt();
        put_byte(CMD_SENSE_INTERRUPT_STATUS);
        if ((get_results() < 1) || (fdc.results[0] & (ST0_IC0 | ST0_IC1))) {
          fdc.state = STATE_RESET;
          PRINT_FLOPPY("[do_request] st0=%#.hh2x, st1=%#.hh2x, st3=%#.hh2x\n", fdc.results[0], fdc.results[1], fdc.results[2]);
        } else {
          fdc.state = STATE_READY;
          drive->current_track = 0;
        }
        break;
    }
    loops++;
    if (loops == MAX_LOOPS)
      return -ERR_FDC_TOO_MANY_RETRIES;
  }
  return res;
}

int floppy_read(int drive_number, int sector, int n, void *buffer, int buffer_size) {
  int sectors_left;
  int sectors_to_read;
  int physical_sector;
  int physical_head;
  int physical_track;
  int start;
  int count;
  unsigned int offset;
  int res;
  media_parameters *media;
  drive_state *drive;

  drive = &fdc.drive[drive_number];

  /* ----- Check for a valid drive number ------------------------------------------------------- */
  if ((drive_number < 0) || (drive_number > MAX_DRIVES) || (bt(fdc.drives, drive_number) == 0))
    return -ERR_FDC_INVALID_DRIVE_NUMBER;
  /* ----- Check number of sectors requested ---------------------------------------------------- */
  if ((n < 0) || (n > MAX_SECTORS))
    return -ERR_FDC_INVALID_SECTORS;
  /* ----- The buffer should be large enough to hold all the data ------------------------------- */
  if ((n*SECTOR_SIZE) > buffer_size)
    return -ERR_FDC_BUFFER_TOO_SMALL;
  /* ----- Buffer should be a valid address ----------------------------------------------------- */
  if (buffer == NULL)
    return -ERR_FDC_NULL_BUFFER;
  /* ----- Get current media type or cycle to next type ----------------------------------------- */
  if (drive->d_par.cycle_media[drive->media_index] == 0)
    cycle_media(drive);
  media = &fdc.drive[drive_number].m_par;
  PRINT_FLOPPY("[floppy_read] media=%d\n", drive->d_par.cycle_media[drive->media_index]);

  start = sector;
  count = 0;
  offset = 0;

  do {
    /*
     * Calculate the physical sector, head and track based on the logical sector
     * number and the amount of sectors that have already been read
     */
    physical_sector = ((start+count) % media->sectors)+1;
    physical_head = ((start+count) / media->sectors) % media->heads;
    physical_track = (start+count) / (media->sectors*media->heads);
    /* ----- Determine how many sectors are left on the track and read the maximum amount ------- */
    sectors_left = media->sectors-physical_sector+1;
    if (n > sectors_left)
      sectors_to_read = sectors_left;
    else
      sectors_to_read = n;

    PRINT_FLOPPY("[floppy_read] physical_sector=%d, physical_head=%d, physical_track=%d\n",
                 physical_sector, physical_head, physical_track);
    PRINT_FLOPPY("               sectors_left=%d, sectors_to_read=%d, n=%d\n", sectors_left,
                 sectors_to_read, n);

    if (sectors_to_read == 0)
      while(1);

    /* ----- Issue the request to the controller ------------------------------------------------ */
    res = do_request(drive_number, OP_READ, physical_sector, physical_head, physical_track,
                     sectors_to_read, (void *)((unsigned int)(buffer)+offset));
    if (res == OK) {
      /* ----- Adjust the buffer offset and counters -------------------------------------------- */
      offset = offset+(SECTOR_SIZE*sectors_to_read);
      start = start+sectors_to_read;
      n = n-sectors_to_read;
    }
  } while ((res == OK) && (n > 0));

  /* ----- Turn off the drive motor ------------------------------------------------------------- */
  stop_motor(drive_number);

  return res;
}

int floppy_write(int drive_number, int sector, int n, void *buffer, int buffer_size) {
  int sectors_left;
  int sectors_to_write;
  int physical_sector;
  int physical_head;
  int physical_track;
  int start;
  int count;
  unsigned int offset;
  int res;
  media_parameters *media;
  drive_state *drive;

  drive = &fdc.drive[drive_number];

  /* ----- Check for a valid drive number ------------------------------------------------------- */
  if ((drive_number < 0) || (drive_number > MAX_DRIVES) || (bt(fdc.drives, drive_number) == 0))
    return -ERR_FDC_INVALID_DRIVE_NUMBER;
  /* ----- Check number of sectors requested ---------------------------------------------------- */
  if ((n < 0) || (n > MAX_SECTORS))
    return -ERR_FDC_INVALID_SECTORS;
  /* ----- The buffer should be large enough to hold all the data ------------------------------- */
  if ((n*SECTOR_SIZE) > buffer_size)
    return -ERR_FDC_BUFFER_TOO_SMALL;
  /* ----- Buffer should be a valid address ----------------------------------------------------- */
  if (buffer == NULL)
    return -ERR_FDC_NULL_BUFFER;
  /* ----- Get current media type or cycle to next type ----------------------------------------- */
  if (drive->d_par.cycle_media[drive->media_index] == 0)
    cycle_media(drive);
  media = &fdc.drive[drive_number].m_par;
  PRINT_FLOPPY("[floppy_write] media=%d\n", drive->d_par.cycle_media[drive->media_index]);

  start = sector;
  count = 0;
  offset = 0;

  do {
    /*
     * Calculate the physical sector, head and track based on the logical sector
     * number and the amount of sectors that have already been read
     */
    physical_sector = ((start+count) % media->sectors)+1;
    physical_head = ((start+count) / media->sectors) % media->heads;
    physical_track = (start+count) / (media->sectors*media->heads);
    /* ----- Determine how many sectors are left on the track and read the maximum amount ------- */
    sectors_left = media->sectors-physical_sector+1;
    if (n > sectors_left)
      sectors_to_write = sectors_left;
    else
      sectors_to_write = n;

    PRINT_FLOPPY("[floppy_write] physical_sector=%d, physical_head=%d, physical_track=%d\n",
                 physical_sector, physical_head, physical_track);
    PRINT_FLOPPY("               sectors_left=%d, sectors_to_write=%d, n=%d\n", sectors_left,
                 sectors_to_write, n);

    if (sectors_to_write == 0)
      while(1);

    /* ----- Issue the request to the controller ------------------------------------------------ */
    res = do_request(drive_number, OP_WRITE, physical_sector, physical_head, physical_track,
                     sectors_to_write, (void *)((unsigned int)(buffer)+offset));
    if (res == OK) {
      /* ----- Adjust the buffer offset and counters -------------------------------------------- */
      offset = offset+(SECTOR_SIZE*sectors_to_write);
      start = start+sectors_to_write;
      n = n-sectors_to_write;
    }
  } while ((res == OK) && (n > 0));

  /* ----- Turn off the drive motor ------------------------------------------------------------- */
  stop_motor(drive_number);

  return res;
}

int read_version() {

  int result;

  put_byte(CMD_DUMP_REGISTERS);
  if (fdc.state == STATE_RESET)
    return T_FDC_NONE;

  result = get_results();
  if (result <= 0)
    return T_FDC_NONE;

  if ((result == 1) && (fdc.results[0] == ST0_IC1))
    return T_FDC_8272A;

  if (result != 10)
    return T_FDC_UNKNOWN;

/*The version command is used to determine if the controller is a standard diskette controller
  or an enhanced version. The NEC765 and Intel 82072 are standard controllers, while other controllers
  such as the NEC72065B and Intel 82077 are considered enhanced controllers*/
  put_byte(CMD_VERSION);

  result = get_results();
  if ((result == 1) && (fdc.results[0] == ST0_IC1))
    return T_FDC_82072;

  if ((result != 0) && (fdc.results[0] != (ST0_IC1|ST0_EC)))
    return T_FDC_UNKNOWN;

/*The unlock command is only supported by enhanced controllers*/
  put_byte(CMD_UNLOCK_FIFO_FUNCTIONS);

  result = get_results();
  if ((result == 1) && (fdc.results[0] == ST0_IC1))
    return T_FDC_82077AA;

  if ((result != 1) && (fdc.results[0] == 0))
    return T_FDC_UNKNOWN;

  return T_FDC_82077AA1;
}

void floppy_initialize() {
  int i;
  int res;
  unsigned char ddt;

  /* ----- Initialize the controller registers -------------------------------------------------- */
  fdc.SRA = FDC_BASE;
  fdc.DR = FDC_BASE+1;
  fdc.SRB = FDC_BASE+1;
  fdc.DOR = FDC_BASE+2;
  fdc.TDR = FDC_BASE+3;
  fdc.DSR = FDC_BASE+3;
  fdc.MSR = FDC_BASE+4;
  fdc.DRSR = FDC_BASE+4;
  fdc.DATA = FDC_BASE+5;
  fdc.DIR = FDC_BASE+7;
  fdc.CCR = FDC_BASE+7;
  /* ----- Assign default configuration values to FDC ------------------------------------------- */
  fdc.drives = 0;
  fdc.installed = 0;
  fdc.state = STATE_RESET;
  fdc.interrupt = 0;
  /*
   * If the BIOS did not report any drives, we don't bother to do anything else and simply
   * ignore the FDC
   */
  if (bt_entry(BT_DISKETTE_INSTALLED) == 0)
    return;
  /* ----- Reset the controller by enabling the DMA and REST fields of the DOR ------------------ */
  outb(DOR_REST | DOR_DMA, fdc.DOR);
  /* ----- Initialize the internal structures of the controller --------------------------------- */
  for (i = 0; i < MAX_RESULTS; i++)
    fdc.results[i] = 0;
  for (i = 0; i < MAX_ERRORS; i++)
    fdc.errors[i] = 0;
  /* ----- Read fd0 parameters from CMOS and add the drive if it is valid ----------------------- */
  ddt = cmos_read_register(CMOS_DDT);
  PRINT_FLOPPY("[floppy_initialize] ddt = %#.2hhx\n", ddt);
  if ((ddt & CMOS_DDT0_MASK) != 0) {
    bts(fdc.drives, 0);
    fdc.drive[0].media_index = 0;
    fdc.drive[0].motor = 0;
    fdc.drive[0].current_track = 0;
    fdc.drive[0].d_par = dp[ddt >> 4];
    fdc.drive[0].m_par = mp[fdc.drive[0].d_par.media_type];
    printk("[floppy_initialize] fd0 is %s\n", fdc.drive[0].d_par.name);
#ifdef DEBUG_FLOPPY
    show_dp(&fdc.drive[0].d_par);
#endif
  }
  /* ----- Read fd1 parameters from CMOS and add the drive if it is valid ----------------------- */
  if ((ddt & CMOS_DDT1_MASK) != 0) {
    bts(fdc.drives, 1);
    fdc.drive[1].media_index = 0;
    fdc.drive[1].motor = 0;
    fdc.drive[1].current_track = 0;
    fdc.drive[1].d_par = dp[ddt & CMOS_DDT1_MASK];
    fdc.drive[1].m_par = mp[fdc.drive[0].d_par.media_type];
    printk("[floppy_initialize] fd1 is %s\n", fdc.drive[1].d_par.name);
#ifdef DEBUG_FLOPPY
    show_dp(&fdc.drive[1].d_par);
#endif
  }
  /* ----- Allocate DMA channel 2 --------------------------------------------------------------- */
  res = dma_request_channel(FDC_DMA_CHANNEL);
  if (res != OK) {
    printk("[floppy_initialize] error allocting dma channel %d (%d)\n", FDC_DMA_CHANNEL, res);
    return;
  }
  /* ----- Allocate 64K buffer for DMA ---------------------------------------------------------- */
  fdc.dma_buffer = mem_allocate_dma(FDC_DMA_CHANNEL);
  if (fdc.dma_buffer == NULL) {
    printk("[floppy_initialize] error allocating dma buffer\n");
    return;
  }
  PRINT_FLOPPY("[floppy_initialize] dma buffer allocated at %#.8x\n", fdc.dma_buffer);
  /* ----- Install interrupt handler on IRQ6 (fixed for diskette controllers) ------------------- */
  res = i8259_install_interrupt_handler(IRQ6, handler_floppy, 0);
  if (res != OK) {
    printk("[floppy_initialize] error installing interrupt handler (%d)\n", res);
    return;
  }
  fdc.installed = 1;
}
