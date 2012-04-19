;******************************************************************************************
;* Bootstrap loader for CS314 (include file), 28.12.2003                                  *
;* University of Stellenbosch                                                             *
;* Jacques Eloff (eloff@cs.sun.ac.za)                                                     *
;******************************************************************************************
  
; Segment:Offset of entry point to second stage of bootstrap loader
  SEGMENT_2ND             EQU 8000h
  OFFSET_2ND              EQU 0200h
  
; Segment:Offset for new stack
  STACK_SEGMENT           EQU 8000h
  STACK_OFFSET            EQU 9000h

; Maximum horizontal count of characters in alpanumeric display mode 3 (80x25)
  DISPLAY_WIDTH           EQU 80

; Maximum vertical count of characters in alpanumeric display mode 3 (80x25)
  DISPLAY_HEIGHT          EQU 25

; Number of bytes required for a single screen in alphanumeric display mode 3 (80x25).
;  This is used to determine when to wrap back to the start of the display. 
  DISPLAY_SIZE            EQU DISPLAY_WIDTH*DISPLAY_HEIGHT*2

; Segment:Offset in real mode where the runtime system will be loaded
  KERNEL_SEGMENT          EQU 0000h
  KERNEL_OFFSET           EQU 1000h

; Offsets in kernel image header for various fields
  LINK_BASE               EQU 08h
  HEAP_START              EQU 0ch

; Size of second stage in 512 byte blocks (sectors)
  BOOTSTRAP_SECTORS       EQU 3

; 512 bytes per sector (given the disk geometry of a 1.44MB diskette)
  SECTOR_SIZE             EQU 200h

; 18 sectors per track (given the disk geometry of a 1.44MB diskette)
  SECTOR_PER_TRACK        EQU 12h

; Total number of sectors read in a single read opertion
  SECTOR_PER_READ         EQU 12h
  
; Total number of tracks to read when loading the runtime system
  TRKS_KERNEL             EQU 7h
  
; Maximum buffer size for a single read operation used by LOAD_IMAGE
  BUFFER_SIZE             EQU SECTOR_PER_READ*SECTOR_SIZE
  
; Maximum buffer size in double words (32-bit words)
  BUFFER_SIZE_4           EQU BUFFER_SIZE/4
  
; Image size of the whole bootstrap loader in 32-bit words
  BOOTSTRAP_SIZE          EQU (SECTOR_SIZE*(BOOTSTRAP_SECTORS+1))/4
  
; Maximum number of SMAP entries supported
  MAX_SMAP_ENTRIES        EQU 20h
  
; ID used by int 15h, function e820h ('SMAP')
  SMAP_ID                 EQU 534d4150h

;******************************************************************************************
;* BIOS FUNCTION NUMBERS                                                                  *
;******************************************************************************************

; BIOS Video Services (10H)
  BIOS_INIT_DISPLAY       EQU 00h
  BIOS_READ_SVGA_INFO     EQU 4Fh
  
; BIOS Disk Services (13H)
  BIOS_RESET_DISKETTE     EQU 00h
  BIOS_READ_SECTOR        EQU 02h
  
; BIOS Extended Services (15H)
  BIOS_GET_EXT_MEMORY     EQU 88h
  BIOS_GET_EXT_MEMORY_64  EQU 0e801h
  BIOS_GET_SMAP           EQU 0e820h
  
;******************************************************************************************
;* DISPLAY ATTRIBUTES FOR COLOUR DISPLAYS                                                 *
;* Note: Foreground and background attributes must be ORed to form the display attribute. *
;*       Only the lower three bits of the upper nibble can be used to specify the         *
;*       the background attribute since the upper bit is used to control blinking.        *
;******************************************************************************************

  FG_BLACK                EQU 00h
  FG_BLUE                 EQU 01h
  FG_GREEN                EQU 02h
  FG_CYAN                 EQU 03h
  FG_RED                  EQU 04h
  FG_MAGENTA              EQU 05h
  FG_BROWN                EQU 06h
  FG_GRAY                 EQU 07h
  FG_DARKGRAY             EQU 08h
  FG_LIGHT_BLUE           EQU 09h
  FG_LIGHT_GREEN          EQU 0ah
  FG_LIGHT_CYAN           EQU 0bh
  FG_LIGHT_RED            EQU 0ch
  FG_LIGH_MAGENTA         EQU 0dh
  FG_YELLOW               EQU 0eh
  FG_WHITE                EQU 0fh

  BG_BLACK                EQU 00h
  BG_BLUE                 EQU 10h
  BG_GREEN                EQU 20h
  BG_CYAN                 EQU 30h
  BG_RED                  EQU 40h
  BG_MAGENTA              EQU 50h
  BG_BROWN                EQU 60h
  BG_GRAY                 EQU 70h

;******************************************************************************************
;* I/O PORT NUMBERS FOR SYSTEM DEVICES: Floppy Disk Controller (FDC)                      *
;******************************************************************************************

; Digital output register (DOR) for the primary diskette controller
  FDC_DOR1                EQU 3f2h

; Bit mask for Controller Reset enable bit in DOR
  FDC_DOR_REST            EQU 4

; Bit mask for DMA enable and IRQ channel bit in DOR
  FDC_DOR_DMA             EQU 8
  
;******************************************************************************************
;* I/O PORT NUMBERS FOR SYSTEM DEVICES: Keyboard controller (8042/8255)                   *
;******************************************************************************************

; Keyboard input port 0
  KBD_INPUT_PORT_0        EQU 60h

; 8255 programmable peripheral interface (PPI) command register
  KBD_COMMAND_REGISTER    EQU 63h             

; Keyboard status port
  KBD_STATUS_PORT         EQU 64h
 
; Bit mask indicating that keyboard buffer is full
  KBD_BUFFER_FULL         EQU 02h
  
; Keyboard command: The next byte written to I/O port 60h will be written to the
;  micro-controller output port
  KBD_CMD_WRITE_OUTPUT    EQU 0d1h
  
;******************************************************************************************
;* I/O PORT NUMBERS FOR SYSTEM DEVICES: Miscellaneous                                     *
;******************************************************************************************

; System control port on MCA/EISA bus
  SYSTEM_CONTROL_PORT     EQU 092h
  
; Bit mask for A20 status field
  A20_STATUS              EQU 02h

;******************************************************************************************
;* PROTECTED MODE RELATED CONSTANTS                                                       *
;******************************************************************************************
  
; Protection Enable bit in control register 0 (CR0)
  CR0_PROTECTION_ENABLE   EQU 01h               

; Selector indices for GDT  
  GDT_NULL_SELECTOR       EQU 00h
  GDT_CODE_SELECTOR       EQU 08h
  GDT_DATA_SELECTOR       EQU 10h
  
; Physical address of entry point to the runtime environment
  ENTRY_POINT             EQU 1000h
  
; This value should always be aligned to a 4K boundary (end with 000)
  KERNEL_STACK_SIZE       EQU 4000h

;******************************************************************************************
;* The current layout of the boot table is as follows:                                    *
;*                                                                                        *
;* +----------+-------------------------------------------------+                         *
;* | Address  | Entry                                           |                         *
;* +----------+-------------------------------------------------+                         *
;* | 00000ffc | Display base address                            |                         *
;* | 00000ff8 | Diskette installed                              |                         *
;* | 00000ff4 | Diskette total                                  |                         *
;* | 00000ff0 | Serial total                                    |                         *
;* | 00000fec | COM4 base address                               |                         *
;* | 00000fe8 | COM3 base address                               |                         *
;* | 00000fe4 | COM2 base address                               |                         *
;* | 00000fe0 | COM1 base address                               |                         *
;* | 00000fdc | Display adapter address register                |                         *
;* | 00000fd8 | Address of kernel stack top                     |                         *
;* | 00000fd4 | Kernel stack size                               |                         *
;* | 00000fd0 | Extended memory in Kb (int 15h, function 88h)   |                         *
;* | 00000fcc | Extended memory in Kb (int 15h, function e801h) |                         *
;* | 00000fc8 | Number of SMAP entries                          |                         *
;* +----------+-------------------------------------------------+                         *
;******************************************************************************************
  BT_DISPLAY_BASE         EQU 00000ffch      ; Display base address for text mode
  BT_DISKETTE_INSTALLED   EQU 00000ff8h      ; Set to 1 there are any diskette drives
  BT_DISKETTE_TOTAL       EQU 00000ff4h      ; Total number of diskette drives attached
  BT_SERIAL_TOTAL         EQU 00000ff0h      ; Total number of serial cards attached
  BT_COM4                 EQU 00000fech      ; Base address for COM4. The next
  BT_COM3                 EQU 00000fe8h      ;  three entries at 0fe4h, 0fe0h and 0fdch
  BT_COM2                 EQU 00000fe4h      ;  are used for COM3, COM2 and COM1
  BT_COM1                 EQU 00000fe0h
  BT_DISPLAY_ADR_REG      EQU 00000fdch      ; Display adapter address register
  BT_KERNEL_STACK_TOP     EQU 00000fd8h      ; Address of kernel stack top
  BT_KERNEL_STACK_SIZE    EQU 00000fd4h      ; Size of kernel stack
  BT_MEM_88H              EQU 00000fd0h      ; Extended memory in Kb (BIOS interrupt 15h,
                                             ;  function 88h
  BT_MEM_E801H            EQU 00000fcch      ; Extended memory in Kb (BIOS interrupt 15h,
                                             ;  function e801h
  BT_MEM_SMAP_COUNT       EQU 00000fc8h

; Offset in memory for BIOS data area fields
  BIOS_COM4               EQU 00000406h      ; Base address for COM4 in BIOS data area.
                                             ;  The previous three entries at 0404, 0402h
					     ;  and 0400 are used for COM3, COM2 and COM1
  BIOS_DISPLAY_ADR_REG    EQU 00000463h      ; Base address for display adapter address
                                             ;  register
					       
