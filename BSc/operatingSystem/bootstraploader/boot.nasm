;******************************************************************************************
;* Bootstrap loader for CS314, 31.01.2005                                                 *
;* University of Stellenbosch                                                             *
;* Jacques Eloff (eloff@cs.sun.ac.za)                                                     * 
;*                                                                                        * 
;* References:                                                                            *
;*  - Allen L. Wyatt, DOS Programmer's Reference, 4th edition, 1994, QUE Publishing       *
;*  - John H. Crawford and Patrick P. Gelsinger, Programming the 80386, 1986, Sybex       *
;*  - Intel Architecture Developer's Manual, Volume 3: System Programming, order no       *
;*    243192, 1997, Intel Corporation (Chapter 8)                                         *
;*  - IBM Personal Computer Technical Reference, 1st edition, 1986, International         *
;*    Business Machines Corporation                                                       *
;*  - Pieter Muller, A Bootstrap Mechanism for the Gneiss Kernel, 1995, Technical Report  *
;*    TR-950100, Department of Computer Science, University of Stellenbosch               *
;*  - Native Oberon bootstrap loader (OBL.ASM) by Pieter Muller, ETH Zurich               *
;*  - van Gilluwe, Undocumented PC, 2nd Edition, Addison Wesley                           *
;*  - BIOS documentation by Ralph Brown                                                   *
;*  - Advanced Configuration and Power Management Interface (ACPI) specification,         *
;*    Version 3.0, September 2004                                                         *
;******************************************************************************************

%include "boot.inc"                          ; Constants
%include "boot.mac"                          ; Macro's

ORG 0
CODESEG

;******************************************************************************************
;* The object code is loaded at address 07c0:0000 (physical address 07c00)                *
;* by the ROM bootstrap routines. At this point, only CS points to segment                *
;* 07c0 so the remaining segment registers must be initialized to satisfy                 *
;* memory references. The following tasks are performed:                                  *
;* - Switch to 80x25 text mode using the BIOS screen services and determine               *
;*   the display type (colour or monochrome)                                              *
;* - Initialize the segment registers                                                     *
;* - Create a small stack at 8000:9000 (physical address 89000)                           *
;* - Check to ensure that the processor is at least an 80386                              *
;******************************************************************************************
   mov    ax, 03h                            ; AH = 0: Initialize display function
                                             ; AL = 3: Select 80x25 text mode
   int    10h                                ; Call BIOS screen services
   
   mov    ax, 07c0h                          ; Setup registers to point to current segment
   mov    ds, ax
   mov    ax, STACK_SEGMENT
   mov    ss, ax
   mov    sp, STACK_OFFSET                   ; Stack starts at 8000:9000

   int    11h                                ; Call BIOS 'Get Equipment Status' service
   mov    [Equipment_Status], ax             ; Store equipment information for later
   and    ax, 30h                            ; Isolate bits 4 and 5 (display adapter 
                                             ;  information)
   cmp    ax, 30h
   jne    detect_386
   mov    dword [Display_Base], 0b0000000h   ; Adjust display address for monochrome 
   
;******************************************************************************************
;* The bootstrap loader is designed for a 80386 or later model processor. The flag        *
;* register of the 80286 contains the IOPL bits, but won't allow us to set them. We can   *
;* use this to determine if we're working with a 80386 or later model CPU. The bootstrap  *
;* loader will stop executing and display an error message if an 80286 processor is       *
;* detected.                                                                              *
;******************************************************************************************
detect_386:
   ClearDisplay BG_BLUE | FG_YELLOW          ; Clear the display, set display attribute
                                             ;  and display location to (0, 0)
   pushf
   pop    ax
   or     ax, 3000h                          ; Set IOPL bits
   push   ax
   popf                                      ; If it is an 80286 processor, then the
                                             ;  IOPL bits will be reset by the CPU
   pushf
   pop    ax
   test   ax, 3000h
   jnz    load_2nd_stage
   WriteString Error_Msg1                    ; Display error message

.no_386:
   jmp    .no_386
   
load_2nd_stage:                              ; Load the remaining portion of the
                                             ;  bootstrap loader into memory

.repeat:
   call   RESET_DISK_DRIVE
   les    bx, [Address_Stage_2]              ; ES:BX points to disk buffer
   mov    ah, BIOS_READ_SECTOR               ; AH = 2: Read Disk Sector function
   mov    al, BOOTSTRAP_SECTORS              ; AL = ?: Number of sectors to read
   mov    cx, 0002h                          ; CH = 0: Track number
                                             ; CL = 2: Starting sector (BIOS numbers disk
					     ;  sectors from 1)
   xor    dx, dx                             ; DH = 0: Head, DL = 0: Drive
   int    13h                                ; Call BIOS disk services
   jnc    relocate_loader                    ; Carry flag (CF) set if error occurred
                                             ;  (AH contains the controller status)
   dec    byte [Retry]                       ; Retry operation at least three times
   jnz    .repeat                            ;  before reporting the disk error
   
   push   eax
   WriteString Error_Msg2                    ; Display error message

   pop    eax
   and    eax, 0ff00h
   shr    eax, 8
   call   WRITE_HEX                          ; Display result code of disk operation
   
.disk_error:
   jmp   .disk_error                         ; Go into endless loop. User must reset
                                             ;  system to try again or use a new disk

;******************************************************************************************
;* Relocate the complete bootstrap loader to the top of memory (128K below the display    *
;* memory).                                                                               *
;******************************************************************************************
relocate_loader:
   les    di, [Relocate_Destination]         
   lds    si, [Relocate_Source]   
   mov    ecx, BOOTSTRAP_SIZE
   cld
   rep    movsd
   jmp    SEGMENT_2ND:OFFSET_2ND             ; Transfer control to 2nd stage

;******************************************************************************************
;* PROCEDURE: RESET_DISK_DRIVE                                                            *
;* INPUT: none                                                                            *
;* Reset the disk drive. This procedure should be called to prepare for disk I/O or when  *
;* encountering disk access errors.                                                       *
;******************************************************************************************
RESET_DISK_DRIVE:
   mov    ah, BIOS_RESET_DISKETTE            ; AH = 0: Reset Disk System function
   xor    dx, dx                             ; DL = 0: Drive
   int    13h                                ; Call BIOS disk services
   ret
   
;******************************************************************************************
;* PROCEDURE: WRITE_CHAR                                                                  *
;* INPUT: AL                                                                              *
;* Writes the character specified in AL to the display at the current screen position     *
;* using the current display attribute.                                                   *
;******************************************************************************************
WRITE_CHAR:
   cld
   les    di, [Display_Base]
   add    di, [Display_Pos]
   mov    ah, [Display_Attribute]
   stosw
   add    word [Display_Pos], 2
   mov    ax, [Display_Pos]
   cwd
   mov    cx, DISPLAY_SIZE
   div    cx
   mov    word [Display_Pos], dx
   ret    

;******************************************************************************************
;* PROCEDURE: WRITE_STRING                                                                *
;* INPUT: SI                                                                              *
;* Writes the character string specified in SI to the display at the current screen       *
;* position using the current display attribute. The string must be NULL terminated.      *
;******************************************************************************************
WRITE_STRING:
.while:
   lodsb
   cmp    al, 0
   jz     .done
   push   si
   call   WRITE_CHAR
   pop    si
   jmp    .while
.done:
   ret
  
;******************************************************************************************
;* PROCEDURE: WRITE_LN                                                                    *
;* INPUT: none                                                                            *
;* Updates the current screen position to the next line. The position will automatically  *
;* wrap to the start of the display if WRITE_LN is called when the current screen         *
;* position references the last line.                                                     *
;******************************************************************************************
WRITE_LN:
.while:
   mov    ax, [Display_Pos]
   cwd
   mov    cx, DISPLAY_WIDTH
   shl    cx, 1
   div    cx
   cmp    dx, 0
   jz     .done
   mov    al, 20h
   call   WRITE_CHAR
   jmp    .while
.done:
   ret
  
;******************************************************************************************
;* PROCEDURE: WRITE_HEX                                                                   *
;* INPUT: EAX                                                                             *
;* Displays the contents specified in EAX in hexadecimal at the current screen position   *
;* using the current display attribute.                                                   *
;******************************************************************************************
WRITE_HEX:
   cld
   mov    bx, Hex_Chars
   mov    ecx, eax
   mov    si, 8         
.while:
   rol    ecx, 4
   mov    al, cl
   and    al, 0fh
   xlatb   
   mov    ah, [Display_Attribute]
   les    di, [Display_Base]
   add    di, [Display_Pos]
   stosw
   add    word [Display_Pos], 2
   mov    ax, [Display_Pos]
   cwd
   mov    di, DISPLAY_SIZE
   div    di
   mov    word [Display_Pos], dx   
   dec    si
   jnz    .while
   ret

;******************************************************************************************
;* PROCEDURE: GOTOXY                                                                      *
;* INPUT: AL, BL                                                                          *
;* Changes the current position to the specified row (AL) and column (BL)                 *
;******************************************************************************************
GOTOXY:
   and    ax, 0ffh
   mov    cl, DISPLAY_WIDTH
   imul   cl
   and    bx, 0ffh
   add    ax, bx
   shl    ax, 1                              ; AX: ((y*DISPLAY_WIDTH)+x)*2
   cwd
   mov    bx, DISPLAY_SIZE
   div    bx
   mov    [Display_Pos], dx
   ret
   
;******************************************************************************************
;* PROCEDURE: CLEAR_DISPLAY                                                               *
;* INPUT: AH                                                                              *
;* Clears the display to the attribute specified in AH. Resets the current position       *
;* to 0,0 and sets the current attribute to AH.                                           *
;******************************************************************************************   
CLEAR_DISPLAY:
   cld
   les    di, [Display_Base]
   mov    cx, DISPLAY_SIZE
   shr    cx, 1
   mov    al, 20h
   rep    stosw
   mov    word [Display_Pos], 0h
   mov    [Display_Attribute], ah
   ret

;******************************************************************************************
;*                             INITIALIZED  VARIABLES                                     *
;******************************************************************************************
DATASEG1
   Error_Msg1           db 'ERROR: CPU is not an 80386', 0
   Error_Msg2           db 'ERROR: Disk error - ', 0
   Hex_Chars            db "0123456789ABCDEF"
   
   Display_Base         dd 0b8000000h
   Display_Attribute    db 07h
   Display_Pos          dw 0000h   

; Segment:Offset where second stage is loaded
   Address_Stage_2      dd 07c00200h

; Number of times to retry disk operations
   Retry                db 03h

; Segment:Offset of bootstrap loader at startup
   Relocate_Source      dd 07c00000h
   
; Segment:Offset where bootstrap loader will be relocated
   Relocate_Destination dd 80000000h

; BIOS equipment status word result (int 11h)
   Equipment_Status     dw 0000h

;******************************************************************************************
;* Some variables are needed to track the progress of loading the kernel from the         *
;* diskette into memory. (Also refer to the LOAD_IMAGE procedure)                         *
;******************************************************************************************
; Destination segment where images are loaded (LOAD_IMAGE)
   D_Segment            dw 0000h

; Destination offset where images are loaded (LOAD_IMAGE)
   D_Offset             dw 0000h

; Segment:Offset in memory where disk buffer resides (LOAD_IMAGE)
   dsk_Buffer           dd 90000000h

; Initially, LOAD_IMAGE starts with head 1
   dsk_Head             db 01h

; Initially, LOAD_IMAGE starts with track 0
   dsk_Track            db 00h

; Total number of tracks read   
   trk_Read             dw 0000h

; Total number of tracks to read
   trk_Total            dw 0000h
   
; Current sector position. The BIOS and FDC number disk sectors from 1, not 0
   sec_Current          db 01h

TIMES  510-($-$$) db 0                       ; Zero-out up to 510-byte boundary
   dw 0xAA55                                 ;  and mark end of sector

;******************************************************************************************
;*                         SECOND STAGE OF BOOTSTRAP LOADER                               *
;*                                                                                        *
;* The second stage of the bootstrap loader spans a number of sectors and contains code   *
;* to perform the following functions:                                                    *
;* - Setting up the boot table                                                            *
;* - Loading the runtime environment into memory                                          *
;* - Switching the processor to protected mode and transferring control to                *
;*   the runtime environment                                                              *
;****************************************************************************************** 
   mov    ax, SEGMENT_2ND
   mov    ds, ax   
   
;******************************************************************************************
;* Gather some additional information and update the boot table                           *
;******************************************************************************************
   xor    ax, ax
   mov    es, ax
   mov    di, BT_DISPLAY_BASE
   mov    eax, [Display_Base]
   shr    eax, 12
   mov    [es:di], eax                       ; Update boot table
   
   mov    ax, [Equipment_Status]
   and    eax, 00000001h                     ; Bit 0 of equipment status indicates
                                             ;  if a diskette drive is installed
   mov    di, BT_DISKETTE_INSTALLED
   mov    [es:di], eax                       ; Update boot table
   
   mov    ax, [Equipment_Status]
   shr    eax, 6
   and    eax, 00000003h                     ; Bits 6 and 7 of equipment status
                                             ;  contain the number of diskette drives
   inc    eax                                ; 00 - 1 drive installed when bit 0 was set
   mov    di, BT_DISKETTE_TOTAL
   mov    [es:di], eax                       ; Update boot table
   
   mov    ax, [Equipment_Status]
   shr    eax, 9
   and    eax, 00000007h                     ; Bits 9, 10 and 11 of equipment status
                                             ;  contain the number of serial adapters
   mov    di, BT_SERIAL_TOTAL
   mov    [es:di], eax                       ; Update boot table

;******************************************************************************************
;* Retrieve base addresses for serial ports from BIOS data area and store the             *
;* information in the boot table                                                          *
;******************************************************************************************
   mov    si, BIOS_COM4
   mov    di, BT_COM4
   mov    cx, 4
   
.read_serial:
   mov    bx, [es:si]
   and    ebx, 0000ffffh
   mov    [es:di], ebx
   sub    si, 2
   sub    di, 4
   dec    cx
   jnz    .read_serial
   
   mov    si, BIOS_DISPLAY_ADR_REG
   mov    ax, [es:si]
   mov    di, BT_DISPLAY_ADR_REG
   mov    [es:di], ax                        ; Update boot table
   
;******************************************************************************************
;* Memory detection using three different methods:                                        *
;* - INT 15h, function 88h should be supported by all PC's. Unfortunately, this function  *
;*   only reports the amount of extended memory between 1Mb and 64Mb and on some systems  *
;*   it will only report the amount between 1Mb and 15Mb                                  *
;* - INT 15h, function e801h reports the number of 1K blocks between 1 and 16Mb in AX     *
;*   and the number of 64K blocks above 16Mb in BX. This function is only supported by    *
;*   newer BIOS implementations (Phoenix v4.0 and AMI 8/23/94 or later for example)       *
;* - INT 15h, function e820h forms part of the Advanced Configuration and Power           *
;*   Management Interface (ACPI) and is only supported by newer BIOS implementations.     *
;*   This function returns a memory map (20 bytes at a time) that can be used to isolate  *
;*   contigious blocks of memory for use by operating systems                             *
;******************************************************************************************
   mov    ah, BIOS_GET_EXT_MEMORY
   int    15h
   mov    di, BT_MEM_88H
   xor    bx, bx
   mov    es, bx
   and    eax, 0000ffffh
   mov    [es:di], eax                       ; Update boot table

   xor    cx, cx                             ; Zero-out CX/DX. Some systems leave their
   xor    dx, dx                             ;  contents unchanged, resulting in
                                             ;  garbage being used to determine
					     ;  the memory size
   mov    di, BT_MEM_E801H
   mov    dword [es:di], 0                   ; Entry contains 0 if not supported
   mov    ax, BIOS_GET_EXT_MEMORY_64
   int    15h
   jc     .smap                              ; Try function e820h if not supported
   
   and    ebx, 0ffffh
   and    eax, 0ffffh
   jz     .try_cx                            ; Try using CX/DX (configured memory)
  
   shl    ebx, 6                             ; Convert 64K blocks to 1K blocks
   add    eax, ebx
   mov    di, BT_MEM_E801H
   mov    [es:di], eax                       ; Update boot table
   jmp    .smap
   
.try_cx:
   and    edx, 0ffffh
   and    ecx, 0ffffh
   jz     .smap
   shl    edx, 6                             ; Convert 64K block to 1K blocks
   add    ecx, edx
   mov    di, BT_MEM_E801H
   mov    [es:di], ecx
   
.smap:
   cld                                       ; Zero-out all the smap entries
   les    di, [smap_ptr]                     ;  in our table
   xor    eax, eax
   mov    ecx, MAX_SMAP_ENTRIES
   lea    ecx, [ecx+ecx*4]
   rep    stosd

   mov    eax, BIOS_GET_SMAP
   mov    edx, SMAP_ID                       ; EDX = "SMAP"
   mov    ebx, 0                             ; EBX = Continuation value, 0 indicates
                                             ;  that operation starts at the beginning of
                                             ;  of the memory map
   mov    ecx, 20                            ; ECX = buffer size, minimum value is 20 bytes
   les    di, [smap_ptr]                     ; ES:DI = pointer to buffer used for result
   int    15h
   
.smap_loop:
   jc    .smap_done
   cmp   eax, SMAP_ID
   jne   .smap_done
   cmp   ecx, 20
   jl    .smap_done

   add   di, 20
   inc   dword [smap_count]
   cmp   dword [smap_count], MAX_SMAP_ENTRIES
   jge   .smap_done

   cmp   ebx, 0                  
   je    .smap_done

   mov   eax, BIOS_GET_SMAP
   mov   edx, SMAP_ID
   int   15h
   jmp   .smap_loop
   
.smap_done:
   mov   di, BT_MEM_SMAP_COUNT
   mov   eax, [smap_count]
   mov   [es:di], eax                        ; Update the boot table

;****************************************************************************************** 
;* Load the runtime environment into memory                                               *
;******************************************************************************************
   ClearDisplay BG_BLUE | FG_YELLOW
   WriteString Load_Msg1
   call   WRITE_LN
   mov    word [D_Segment], KERNEL_SEGMENT
   mov    word [D_Offset], KERNEL_OFFSET
   mov    word [trk_Total], TRKS_KERNEL
   call   LOAD_IMAGE                         ; Load the runtime system into memory
   call   WRITE_LN

   mov    ax, KERNEL_SEGMENT
   mov    es, ax
   mov    eax, [es:KERNEL_OFFSET+LINK_BASE]
   cmp    eax, ENTRY_POINT
   je     setup_kernel_stack
   
   WriteString Error_Msg3
.link_base_error:
   jmp    .link_base_error

;****************************************************************************************** 
;* Create the stack for the runtime environment. The stack is aligned to the nearest 4K   *
;* boundary above the runtime system. For example, consider a runtime system (5724 bytes  *
;* in size) linked at 11000h. The runtime system will stop at address 1265Ch. The nearest *
;* 4K boundary is located at address 13000h. Given a 16K stack for the runtime system     *
;* and the NULL page that separates the kernel and its stack, the new stack top will      *
;* become 18000h (13000h+4000h+1000h).                                                    *
;******************************************************************************************
setup_kernel_stack:
   mov    ax, KERNEL_SEGMENT
   mov    es, ax
   mov    eax, [es:KERNEL_OFFSET+HEAP_START] ; Determine linear address where runtime
   add    eax, 0fffh                         ;  system stops and align it on a 4K boundary
   mov    ebx, 0fffh
   not    ebx
   and    eax, ebx
   add    eax, KERNEL_STACK_SIZE             ; Add the kernel stack size to the aligned
   add    eax, 4096                          ;  address and allocate room for the bottom
                                             ;  NULL page. The runtime system will be
                                             ;  responsible for allocating the top
                                             ;  NULL page and will mark both pages
					     ;  as not-present in the page tables

   xor    bx, bx
   mov    es, bx
   mov    bx, BT_KERNEL_STACK_TOP
   mov    [es:bx], eax                       ; Update the boot table
   mov    bx, BT_KERNEL_STACK_SIZE
   mov    edx, KERNEL_STACK_SIZE
   mov    [es:bx], edx                       ; Update the boot table
   
   mov    ebx, eax
   and    eax, 0ffffh
   mov    sp, ax                             ; Convert the linear address back into
   and    ebx, 0f0000h                       ;  a segment:offset pair since we are still
   shr    ebx, 4                             ;  running in real mode 
   mov    ss, bx
   
;******************************************************************************************
;* The motor of the disk drive may still be on and the BIOS may not switch it off in time * 
;* when the switch to protected mode occurs. The motor may be stopped by sending a        *
;* command to the digital output register (DOR) of the floppy disk controller (FDC). The  *
;* DOR is an 8 bit register and has the following format:                                 *
;*                                                                                        *
;*     7      6      5      4      3     2      1     0                                   *
;*  +------+------+------+------+-----+------+-----+-----+                                *
;*  | MOTD | MOTC | MOTB | MOTA | DMA | REST | DR1 | DR0 |                                *
;*  +------+------+------+------+-----+------+-----+-----+                                *
;*                                                                                        *
;* MOTD, MOTC, MOTB and MOTA control the status of the drive motor (1 = start, 0 = stop), *
;* DR1 and DR0 control drive selection (00 = Drive A, 01 = Drive B, etc). The REST bit    *
;* must be set to allow the controller to accept commands                                 *
;******************************************************************************************
   mov   dx, FDC_DOR1
   mov   al, FDC_DOR_REST+FDC_DOR_DMA          ; Reset controller, DMA/IRQ enabled               
   out   dx, al   
   
;******************************************************************************************   
;* STEP 1                                                                                 *
;* Disable interrupts by clearing the interrupt flag (IF = 0). NMI's are not disabled     *
;******************************************************************************************
   mov    si, PM_Switch_Msg1
   call   WRITE_STRING
   call   WRITE_LN
   cli

;******************************************************************************************      
;* STEP 2                                                                                 *
;* Activate address line 20 (A20). The output of this line is gated through the 8042      *
;* keyboard controller. However, the keyboard input buffer must be empty before A20 may   *
;* be gated on. A simple test is also performed to determine if A20 is really enabled     *
;* because some systems can be problematic, especially those using an MCA bus or          *
;* if the A20 gating option in the CMOS is set to fast. If the test fails, an alternative *
;* method using the System Control port (92h) is applied before testing A20 again. An     *
;* error message is displayed if this method also fails.                                  *
;******************************************************************************************
   mov    si, PM_Switch_Msg2
   call   WRITE_STRING
   call   WRITE_LN
   
   call   FLUSH_KBD_BUFFER
   mov    al, KBD_CMD_WRITE_OUTPUT           ; The next data byte written to port 60h 
                                             ;  will be written to the micro-controller
					     ;  output port
   out    KBD_STATUS_PORT, al                
   call   FLUSH_KBD_BUFFER
   mov    al, 0dfh                           ; Enable A20
   out    KBD_INPUT_PORT_0, al
   call   FLUSH_KBD_BUFFER
   
   call   TEST_A20
   jnc    .init_idt
   
   in     al, SYSTEM_CONTROL_PORT
   or     al, A20_STATUS                     ; Enable A20
   and    al, 0feh                           ; Mask out the reset bit
   out    SYSTEM_CONTROL_PORT, al
   
   call   TEST_A20
   jnc    .init_idt
      
   mov    si, PM_A20_Failed_Str
   call   WRITE_STRING
   call   WRITE_LN
.a20_error:   
   jmp    .a20_error
   
;******************************************************************************************
;* STEP 3                                                                                 *
;* Create an empty interrupt descriptor table (IDT) by loading the address of the table   *
;* into the interrupt descriptor table register (IDTR). Interrupts generated when an      *
;* empty IDT is present will cause a shutdown.                                            *
;******************************************************************************************
.init_idt:
   mov    si, PM_Switch_Msg3
   call   WRITE_STRING
   call   WRITE_LN
   
   db 66h                                    ; Ensure that a 32-bit instruction is executed
   lidt   [ptrIDT]                           ;  and load the IDTR
	
;******************************************************************************************
;* STEP 4                                                                                 *
;* Initialise the global descriptor table (GDT) by loading the global descriptor table    *
;* register (GDTR). The GDTR is a 48-bit register consisting out of a 16-bit limit and    *
;* 32-bit base address. The limit has been pre-computed and only the base address must be *
;* set.                                                                                   *
;******************************************************************************************
   mov    si, PM_Switch_Msg4
   call   WRITE_STRING
   call   WRITE_LN

   xor    eax, eax
   mov    ax, cs
   shl    eax, 4
   mov    ebx, GDT
   add    eax, ebx                           ; EAX contains physical address of GDT
   mov    dword [ptrGDT+2], eax              ; Store base address 
   db 66h                                    ; Ensure that a 32-bit instruction is executed
   lgdt   [ptrGDT]                           ;  and load the GDTR

;******************************************************************************************
;* STEP 5                                                                                 *
;* Store the address of the entry point to the system on the stack and compute value of   *
;* ESP in EDX.                                                                            *
;******************************************************************************************
   mov    si, PM_Switch_Msg5
   call   WRITE_STRING
   call   WRITE_LN

   push   word GDT_CODE_SELECTOR
   push   dword ENTRY_POINT                  ; Store system entry point on the stack
   
   xor    edx, edx
   mov    dx, ss
   shl    edx, 4
   xor    eax, eax
   mov    ax, sp
   add    edx, eax                           ; Store linear address of stack in EDX

;******************************************************************************************
;* STEP 6                                                                                 *
;* Swith to protected mode by settig the Protection Enable bit (PE) and executing a JMP   *
;* instruction to serialize the CPU. There are two ways of doing the switch:              *
;* 1) The traditional 80286 method using SMSW and LMSW:                                   *
;*       smsw ax                                                                          *
;*       or   al, 1                                                                       *
;*       lmsw ax                                                                          *
;*       jmp  continue                                                                    *
;* 2) Setting the bit directly in control register 0 (CR0). The lower 16 bits of CR0      *
;*    contains the old Machine Status Word. Control registers can only be accessed        *
;*    starting with the 80386 processor. At this point it has already been confirmed      *
;*    that at least an 80386 CPU is present, and consequently this method is used.        *
;******************************************************************************************
   mov   eax, cr0
   or    eax, CR0_PROTECTION_ENABLE          ; Set PE bit
   mov   cr0, eax
   jmp   .serialize                          ; Serialize CPU
   
.serialize:
   mov   ax, GDT_DATA_SELECTOR               ; Reload all the segment selectors
   mov   ds, ax                          
   mov   ss, ax
   mov   es, ax                              ; ES, FS and GS can also be loaded with the
   mov   fs, ax                              ;  null descriptor if they are not going to
   mov   gs, ax                              ;  be used
   mov   esp, edx

   db 66h                                    ; Ensure that a 32-bit instruction is 
   retf                                      ;  generated and return to entry point

;******************************************************************************************
;* PROCEDURE TEST_A20                                                                     *
;* INPUT: none                                                                            *
;* Determine if A20 line is active. CF is set if A20 is not enabled                       *
;******************************************************************************************
TEST_A20:
   mov    di, 10h
   mov    ax, 0ffffh
   mov    es, ax
   xor    ax, ax
   mov    si, ax
   mov    fs, ax
   
   mov    ax, [es:di]
   cmp    ax, [fs:si]
   jne    .a20_done
   
.a20_unknown:
   not    word [fs:si]
   mov    ax, [es:di]
   cmp    ax, [fs:si]
   je     .a20_done
   stc                                       ; Set CF to indicate error
   not    word [fs:si]
   ret
   
.a20_done:
   clc
   not    word [fs:si]
   ret

;******************************************************************************************
;* PROCEDURE: FLUSH_KBD_BUFFER                                                            *
;* INPUT: none                                                                            *
;* Waits until the keyboard buffer is empty                                               *
;******************************************************************************************
FLUSH_KBD_BUFFER:
   in    al, KBD_STATUS_PORT                 ; Read Keyboard status port
   and   al, KBD_BUFFER_FULL                 ; Test if input buffer full flag (bit 2)
   jnz   FLUSH_KBD_BUFFER                    ;  is set
   ret

;******************************************************************************************
;* PROCEDURE: LOAD_IMAGE                                                                  *
;* INPUT: D_Segment, D_Offset, dsk_Track, dsk_Head, trk_Total                             *
;* Loads an image from disk into memory starting at the address specified in D_Segment    *
;* and D_Offset. The operation starts at the first sector of the track specified in       *
;* dsk_Track using the head number supplied in dsk_Head. The number of tracks to be read  *
;* are specified in trk_Total.                                                            *
;******************************************************************************************
LOAD_IMAGE:
   cld
   mov    byte [trk_Read], 0                      ; Initialize variables for loading

.while0:
   mov    ax, [trk_Read]
   cmp    ax, [trk_Total]                         ; WHILE trkRead < trkTotal DO
   jl     .continue1
   jmp    .end0

.continue1:
   mov    byte [sec_Current], 1                   ;   secCurrent := 1;
   mov    byte [Retry], 3

.while1:
   cmp    byte [sec_Current], SECTOR_PER_TRACK    ;   WHILE secCurrent < secPerTrack DO
   jl     .read
   jmp    .end1
   
.read:                                            ;     REPEAT
   call   RESET_DISK_DRIVE
   mov    ah, BIOS_READ_SECTOR                             
   mov    al, SECTOR_PER_READ
   les    bx, [dsk_Buffer]                       
   mov    ch, [dsk_Track]
   mov    cl, [sec_Current]                   
   mov    dh, [dsk_Head]
   mov    dl, 0                                     
   int    13h                                     ;       Read data into disk buffer 
   jnc    .copy
   dec    byte [Retry]
   jnz    .read                                   ;     UNTIL (Result = 0) OR (Retry = 0)
                                                  ;     IF Result # 0 THEN
   push   eax
   mov    esi, Error_Msg2
   call   WRITE_STRING                            ;       Display error message

   pop    eax
   and    eax, 0ff00h
   shr    eax, 8
   call   WRITE_HEX
   
.disk_error:
   jmp   .disk_error                              ;       LOOP END

; The data can either fit within the current segment (.copy2) or else the data must be
; transferred using two operations (.copy1 and .copy2). This first operation will fill the
; current segment before the remainder of the disk buffer is transferred to the following
; segment. It is also possible that the complete buffer must be copied to a new
; segment, so D_Offset and D_Segment are first adjusted to compensate for this.
.copy:                                            ;     ELSE
   mov    al, 0b1h
   call   WRITE_CHAR                              
   mov    bx, 0ffffh
   sub    bx, [D_Offset]
   inc    bx                                      ;       n := 0FFFFH-D_Offset+1;
   jnz    .copy1                                  ;       IF n = 0 THEN
   mov    [D_Offset], bx                          ;         D_Offset := n;
   add    word [D_Segment], 1000h                 ;         INC(D_Segment, 1000H)
                                                  ;       END;
.copy1:
   mov    di, [D_Offset]
   mov    ax, [D_Segment]
   mov    es, ax
   cmp    bx, 0
   je     .copy2
   cmp    bx, BUFFER_SIZE                         ;       IF (n # 0) & (n < bufSize) THEN
   jae    .copy2               

   push   ds
   mov    cx, bx
   lds    si, [dsk_Buffer]
   shr    cx, 2
   rep    movsd                                   ;         Copy data from disk buffer to
                                                  ;         the destination buffer
   pop    ds
   add    word [D_Segment], 1000h                 ;         INC(D_Segment, 1000H);
   mov    cx, BUFFER_SIZE
   sub    cx, bx
   push   ds
   xor    di, di                                  ;         D_Offset := 0;
   mov    ax, [D_Segment]
   mov    es, ax   
   lds    si, [dsk_Buffer]
   add    si, bx
   shr    cx, 2
   rep    movsd                                   ;         Copy data from disk buffer to
                                                  ;         to destination buffer
   pop    ds

   mov    word [D_Offset], BUFFER_SIZE
   sub    word [D_Offset], bx                     ;         D_Offset := BufSize-n;
   jmp    .inc2
         
.copy2:                                           ;       ELSE    
   push   ds
   mov    cx, BUFFER_SIZE_4
   lds    si, [dsk_Buffer]
   rep    movsd                                   ;         Copy whole disk buffer to
                                                  ;         to destination buffer
   pop    ds
   add    word [D_Offset], BUFFER_SIZE            ;         INC(D_Offset, bufSize)    
                                                  ;       END
						  ;     END;	     
.inc2:                                            
   add    byte [sec_Current], SECTOR_PER_READ     ;     INC(secCurrent, secPerRead)
   jmp    .while1                                 ;   END;
   
.end1:
   mov    al, [dsk_Head]                          
   add    [dsk_Track], al                         ;   INC(dskTrack, dskHead);
   xor    byte [dsk_Head], 1                      
   add    byte [trk_Read], al                     ;   INC(trkRead)
   jmp    .while0                                 ; END

.end0:
   ret

DATASEG2
   Load_Msg1         db 'Loading osNinex runtime system...', 0
   Error_Msg3        db 'ERROR: Invalid link base in image', 0
   PM_Switch_Msg1    db 'Disabling interrupts', 0
   PM_Switch_Msg2    db 'Activating A20', 0
   PM_Switch_Msg3    db 'Initializing IDT', 0
   PM_Switch_Msg4    db 'Initializing GDT', 0
   PM_Switch_Msg5    db 'Setting entry point and new stack', 0
   
   PM_A20_Failed_Str db 'A20 not enabled',0
   
   smap_ptr          dd 00000600h
   smap_count        dd 0
   
;******************************************************************************************
;* Segment descriptors are 8 bytes (64 bits) long and every descriptor consists out of    * 
;* three parts: the segment base address, segment limit and segment attributes. The       *
;* segment base is 32-bits in size, the segment limit is also 32 bits in size, but only   *
;* 20 bits are used. If the byte granularity is used, only small limits can be specified, *
;* while 4K granularity allows for the addition of large segment limits (See Crawford,    *
;* chapter 5).                                                                            *
;*                                                                                        *
;*  Example (byte granularity)                                                            *
;*    Base  = 12340000                                                                    *
;*    Limit = 0                                                                           *
;*    This example describes a segment consisting of 1 byte stored at                     *
;*    linear address 12340000                                                             *
;*                                                                                        *
;*  Example (4K granularity)                                                              *
;*    Base = 12340000                                                                     *
;*    Limit = 0                                                                           *
;*    This example describes a segment consisting of 4K of memory                         *
;*    starting at linear address 12340000 and ending at address 12340FFF                  *
;*                                                                                        *
;*  The layout of a segment descriptor is shown below                                     *
;*                                                                                        *
;* 63            55                  40 39               16 15              0             *
;* +-----------+ +--------------------+ +-----------------+ +---------------+             *
;* |           | |                    | |                 | |               |             *
;* | 31..24 of | | Segment Attributes | |      0..23      | |   0..15 of    |             *
;* |  segment  | |                    | | of segment base | | segment limit |             *
;* |   base    | |                    | |                 | |               |             *
;* +-----------+ +--------------------+ +-----------------+ +---------------+             *
;*                                                                                        *
;* The segment attribute is 16 bits long and divided into the following fields:           *
;*                                                                                        *
;*  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0                        *
;* +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                      *
;* |   |   |   | A |     19..16    |   |       | D |               |                      *
;* | G | D | 0 | V |   of segment  | P |  DPL  | T |      TYPE     |                      *
;* |   |   |   | L |     limit     |   |       | 1 |               |                      *
;* +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                      *
;*                                                                                        *
;* G    - Granularity attribute. Set to 1 implies 4K granular limit                       *
;* D    - 1 for 80386, 0 for 80286. The interpretation of this bit depends                *
;*        on the type of the segment:                                                     *
;*         1 - Executable segments use the D bit to determine the                         *
;*             default operand size. D=1 defaults to 32-bit addresses and                 *
;*             32-bit and 8-bit operands. D=0 defaults to 16-bit addresses                *
;*             and 16-bit and 8-bit operands                                              *
;*         2 - Expand down segments use this bit to determine the upper limit             *
;*             D=1 indicates a 4G upperlimit. D=0 indicates a 64K upper limit             *
;*         3 - Segments addressed by SS use this bit to determine whether                 *
;*             to use ESP (D=1) or SP (D=0) for explicit stack reference                  *
;*             by way of PUSH and POP instructions                                        * 
;* AVL  - Availabe-for-software. Value of this bit is not interpreted by                  *
;*        80386 and Intel promises that future compatible processors will not             *
;*        define a use for this bit                                                       *
;* P    - Present bit                                                                     *
;* DPL  - Descriptor Privilege Level. Value defines the privilege level                   * 
;*        associated with the segment                                                     *
;* DT   - Distinguishes between system segments (DT=1) and gates (DT=0)                   *
;* TYPE - Defines the type of the memory descriptor.                                      *
;******************************************************************************************
  
;******************************************************************************************
;* Global Descriptor Table (GDT) - This GDT is only a temporary table and will be         *
;* replaced by the runtime system's own GDT. However, a GDT is necessary to operate in    *
;* protected mode until the runtime system's own GDT is created. The following            *
;* descriptors are created:                                                               *
;*                                                                                        *
;*  DESCRIPTOR NAME    BASE  LIMIT  ATTRIBUTES                                            *
;*  null descriptor    0     0      n/a                                                   *
;*  code segment       0     FFFFF  TYPE = Execute/Read, 4K granular limit, DPL = 0       *
;*  data segment       0     FFFFF  TYPE = Read/Write, 4K granular limit, DPL = 0         *
;******************************************************************************************
  
ALIGN 4
  GDT TIMES 8 db 0                                      ; Null descriptor
              db 0ffh, 0ffh, 0, 0, 0, 9ah, 0cfh, 0      ; Code segment descriptor
              db 0ffh, 0ffh, 0, 0, 0, 92h, 0cfh, 0      ; Data segment descriptor
  
  GDT_SIZE               EQU $-GDT
	      
;******************************************************************************************  
;* Both the GDTR and LDTR are composed of two parts. A 16-bit limit and a 32-bit base     *
;* address.                                                                               *
;******************************************************************************************
  ptrGDT dw GDT_SIZE-1, 0, 0                 ; 48-bit pointer to GDT	      
  ptrIDT dw 0, 0, 0                          ; 48-bit null pointer to IDT	      
	      
TIMES  ((BOOTSTRAP_SECTORS+1)*SECTOR_SIZE)-($-$$) db 0
