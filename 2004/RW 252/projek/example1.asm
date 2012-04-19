;*******************************************************************************
;* Example program for CS252 project - File operations                         *
;* Jacques Eloff (eloff@cs.sun.ac.za)                                          *
;* Modified: 07.09.2004                                                        *
;* Instructions:                                                               *
;*  - Assemble with 'nasm -f elf example1.asm'                                 *
;*  - Compile with 'gcc example1.o -o example1'                                *
;*  - Usage: ./example1 <filename>                                             *
;*******************************************************************************

;*******************************************************************************
;* External symbols                                                            *
;*******************************************************************************
extern printf

;*******************************************************************************
;* Global symbols                                                              *
;*******************************************************************************
global main

;*******************************************************************************
;* Code                                                                        *
;*******************************************************************************
section .text

;*******************************************************************************
;* int main(int argc, char *argv)                                              *
;*******************************************************************************
%define argc [ebp+8]
%define argv [ebp+12]
main:
  push  ebp
  mov   ebp, esp
  push  ebx
  push  esi
  push  edi

  cmp   dword argc, 1              ; if (argc <= 1) {
  jg   .continue
  push  msg_no_args
  call  printf                     ;  printf("No arguments specified \n");
  add   esp, 4                     ;  return 0;
  jmp   .done                      ; }
				   
.continue:
  mov   eax, SYS_OPEN
  mov   ebx, argv
  mov   ebx, [ebx+4]
  mov   ecx, O_CREAT | O_TRUNC | O_RDWR
  mov   edx, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
  int   0x80                       ; fd = open(argv[1], O_CREAT|O_TRUNC|O_RDWR,
                                   ;           S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  
  cmp   eax, -1                    ; if (fd < 0) {
  jg    .close_file
  push  eax
  push  msg_creat_error            
  call  printf                     ;   printf("error creating file %d\n", fd);
  add   esp, 8                     ;   return 0;
  jmp   .done                      ; }
  
.close_file:
  mov   ebx, eax
  mov   eax, SYS_CLOSE
  int   0x80                       ; close(fd);

.done:
  pop   edi
  pop   esi
  pop   ebx
  mov   esp, ebp
  pop   ebp
  xor   eax, eax
  ret                              ; return 0;

;*******************************************************************************
;* Initialized data                                                            *
;*******************************************************************************
section .data
  msg_no_args     db "No arguments specified", 0x0a, 0
  msg_creat_error db "Error creating file %d", 0x0a, 0

;*******************************************************************************
;* Constants
;*******************************************************************************

; Flags for open system call (octal) (refer to '/usr/include/bits/fcntl.h')
O_RDONLY  EQU 0q
O_WRONLY  EQU 1q
O_RDWR    EQU 2q
O_CREAT   EQU 100q
O_TRUNC   EQU 1000q

; File permission bits (octal)
S_IRUSR   EQU 400q                 ; User read
S_IWUSR   EQU 200q                 ; User write
S_IRGRP   EQU 40q                  ; Group read
S_IROTH   EQU 4q                   ; Other read

; System call numbers
SYS_OPEN  EQU 5
SYS_CLOSE EQU 6
