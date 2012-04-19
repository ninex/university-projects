global main
global print_hea
global print_huf

extern printf
extern malloc
extern free
extern strlen

extern heap_initialize
extern heap_insert

extern huffman_build_tree
extern huffman_initialize_table
extern huffman_build_table

;***********************************************
;int print_huf(root);
;***********************************************
%define root [ebp+8]
print_huf:
push  ebp
mov   ebp, esp

mov esi,root
mov esi,[esi]
mov ebx,0

.whilet:
cmp ebx,255
jg .endit
	lea edi,[esi+8*ebx]
	cmp [edi+4],dword 0
je .ref
  mov edx,[edi]
  mov ecx,[edi+4]              ;size
  ;mov edx,[edx]                ;code
  pushad
    push ecx
    push edx
    push ebx
    push msg_12
    call printf
    add esp,16
  popad
.ref:
  inc ebx

jmp .whilet
.endit:

mov   esp, ebp
pop   ebp
ret


;***********************************************
;int print_hea(root);
;***********************************************
%define h [ebp+8]
print_hea:
push  ebp
mov   ebp, esp

mov esi,h
;mov esi,[esi]
mov edx,[esi]


pushad
push dword [esi]
push msg_13
call printf
add esp,8
popad

lea esi,[esi+4]

mov ebx,0
.wh:
cmp ebx,edx
jge near .ein

  pushad
    lea edi,[8*ebx]
    lea edi,[esi+(edi*2)]


    push dword [edi+4]
    push dword [edi]
    push ebx
    push msg_14
    call printf
    add esp,16
popad

  inc ebx
jmp .wh
.ein:

pushad
push msg_ma
call printf
add esp,4

mov   esp, ebp
pop   ebp
ret

;***********************************************
;int main(int argc,char *argv[]);
;***********************************************
%define argc [ebp+8]
%define argv [ebp+12]
%define h [ebp-4100]
%define temp [ebp-4116]
%define temp2 [ebp-4132]
%define code [ebp-4136]
%define size [ebp-4140]
%define buf [ebp-4144]
%define file1 [ebp-4148]
%define file2 [ebp-4152]
%define tempHnode [ebp-4168]


main:
  push  ebp
  mov   ebp, esp
  push  ebx
  push  esi
  push  edi

  sub esp,4168

; pushad
; push dword [esp]
; push msg_13
; call printf
; add esp,8
; popad

  cmp dword argc,2
  je .have_input               ;if argc != 2
    push msg_1
    call printf                  ;printf(msg_1)
    add esp,4
    jmp .end_prog
  .have_input:                 ;else
    mov esi,argv
    mov eax,5
    mov ebx,[esi+4]             ;try opening the file
    mov ecx,0
    mov edx,0
    int 0x80

    cmp eax,0
    jl near .file_error               ;if no file error

      mov ebx,eax                  ;put file handle in ebx

      push dword 1025              ;allocate memory for 256*4 bytes for table and 1 byte for buffer
      call malloc
      add esp,4

      mov size,eax			;skryf in size vir free later


      cld
      mov edi,eax
      push edi
      mov eax,0                         ;clear the memory
      mov ecx,256
      rep stosd
      stosb
      pop edi

      mov ecx,edi                  ;setup to read a byte to buffer
      mov edx,1

;=============================  READ INTO FREQUENCY TABLE  ===========================

      .do_while:
        mov eax,3
        int 0x80                  ;read a byte
      cmp eax,0
      jl near .read_error		;file error?
      je .end_while			;al die bytes gelees?
      pushad
        mov eax,0
        mov al, [ecx]			;sit in frequency table
        inc dword [ecx+1+(4*(eax))]

      jmp .do_while
      .end_while:

      mov eax,6
      int 0x80                      ;close input file
      cmp eax,0
      jne near .file_error

;============================== EVERYTHING IN FILE HAS BEEN READ ==========================



       ; mov eax,h

	pushad
        lea eax,h
        push eax;dword h                  ;create an empty heap
        call heap_initialize
        add esp,4
        popad

        lea edi,[edi+1]			;beweeg na begin van frequency table

; pushad
; lea eax,h
; push eax
; call print_hea
; add esp,4
; popad
;=============================SIT CHAR MET FREQ > 0 IN HEAP  =============================

        mov buf,dword 0			;vir gebruik van heap count in die header later
 	mov code,edi			;vir header: begin van frequency table

	mov ebx,0
        .for_loop:
        cmp ebx,255
        jg .end_for                     ;for (i=0;i<256;i++)

          mov eax,[edi+(4*(ebx))]        ;kry die frequency vir karakter ebx van 256


          cmp eax,0
          jle .continue                 ;if frequency > 0

            mov ecx,[edi+(4*(ebx))]
	    lea eax,tempHnode
            mov [eax],ecx                ;node.frequency = frequency
            mov [eax+4],ebx               ;node.c = ebx
            mov [eax+8],dword 0
            mov [eax+12],dword 0
; pushad
; push dword [edi+4]
; push dword [edi]
; push dword 0
; push msg_14
; call printf
; add esp,16
; popad
            pushad
            lea eax,tempHnode
            lea ebx,h
            push eax
            push ebx                     ;heap_insert(h,node)
            call heap_insert
            add esp,8
	    popad

	    inc dword buf

        .continue:
          inc ebx
          jmp .for_loop

        .end_for:
pushad
lea eax, h
push dword eax
call print_hea
add esp,4
popad
;========================= BUILD HUFFMAN TREE  ================================
       lea eax,temp
       lea ebx,h
       push eax
       push ebx
       call huffman_build_tree                 ;build a huffman tree from the heap
       add esp,8
pushad
push dword [esp]
push msg_13
call printf
add esp,8
popad
; pushad
; lea eax, h
; push dword eax
; call print_hea
; add esp,4
; popad
; pushad
; push dword [esp]
; push msg_13
; call printf
; add esp,8
; popad
;========================= CREATE EMPTY HUFFMAN TABLE  ================================
lea eax,temp2
       push eax
       call huffman_initialize_table           ;initialize a table
       add esp,4

;========================= BUILD HUFFMAN TABLE FROM TREE  ================================

       lea ebx,temp2
       lea eax,temp
       push dword -1
       push dword 0
       push dword ebx
       push dword eax                         ;build the table from the huffman tree
       call huffman_build_table
       add esp,16

;========================= FREE HEAP  ================================
;
;        push dword h
;        call free
;        add esp,4

;============              START CREATING FILE             ================
      mov esi,argv
      mov ebx,[esi+4]			;get the filename

      push ebx
      call strlen                          ;get the length of the filename string
      add esp,4

      mov [ebx+eax],byte 46                ;add the .cz extension
      mov [ebx+eax+1],byte 99
      mov [ebx+eax+2],byte 122
      mov [ebx+eax+3],byte 0

      pushad
      push ebx
      push msg_5
      call printf                          ;print the compress to <file> message
      add esp,8
      popad

      mov eax,8
      mov ecx,7666                             ;create the file
      int 0x80

      cmp eax,0					;check if created
      jg .created

      mov eax,5
      mov edx,ecx                              ;truncate file
      mov ecx,O_trunc
      int 0x80
      cmp eax,0
      jle near .file_error


      .created:
      mov file2,eax				;move filedescriptor into file2
      mov ebx,eax

;============================    WRITE HEADER    =============================
      lea ecx,buf
      mov edx,4
      mov eax,4				;write freq table count
      int 0x80
      cmp eax,0
      jl near .file_error

      mov edi,code			;adres van freq table

      mov esi,0
        .co:
        cmp esi,255
        jg near .con

          cmp  dword [edi+(4*(esi))],0
          jle .bleh
            mov edx,1
            mov buf,esi                                  ;skryf character
            mov eax,4
            int 0x80
            cmp eax,0
            jl near .write_error

            mov eax,[edi+(4*(esi))]
            mov edx,4
            mov buf,eax                                 ;skryf frequency
            mov eax,4
            int 0x80
            cmp eax,0
            jl near .write_error

          .bleh:
        inc esi
        jmp .co
       .con:

;================================== FREE FREQUENCY TABLE ================================
;
;       push dword size
;       call free				;free memory van frequency table
;       add esp,4

;================================= OPEN SOURCE =====================================
      mov esi,argv
      mov ebx,[esi+4]

      push ebx
      call strlen			;remove .cz extension
      add esp,4
      mov [ebx+eax-3],byte 0

      mov eax,5             ;open the source file
      mov ecx,0
      mov edx,0
      int 0x80
      cmp eax,0
      jle near .file_error


      mov file1,eax


      mov code,dword 0		  ;clear dit
      mov size,dword 0

      ;=========================            START BIT STREAM           ======================

     .while_reading:

        mov buf, dword 0
        lea ecx,buf                 ;adress of buffer to read to
        mov ebx,file1
	mov edx,1                   ;bytes to read
        mov eax,3                   ;read a byte
        int 0x80
        cmp eax,0
        jl near .read_error	;read error
        je near .pad		;alles gelees, gaan pad laaste byte


        mov ecx,temp2		     ;adres van huffman table na ecx
	mov ecx,[ecx]
        mov edx,buf		     ;character wat gelees is
        lea edx,[ecx+8*edx]	     ;sy posisie in huffman table
        mov ecx,[edx+4]              ;kry size
        mov edx,[edx]                ;kry code


	add size,ecx
	mov ebx,code		;insert character se binary code in code
	shl ebx, cl
	add ebx,edx
	mov code,ebx		;skryf nuwe code terug

;====== DUMP WRITEBUFFER NA DISK =======
.write_loop:
	cmp dword size,8
	jl .while_reading;do_not_write	;nodig om te dump?

	sub dword size,8	;size - 8


	mov edx,code		;move code na eax en edx
	mov eax,edx
	mov ecx,size		;move size na ecx

	shr edx,cl		;bring overflow terug
	mov buf,dword 0		;clear buffer wat gewrite word
	mov buf,edx		;move char wat geskryf word

	mov edx,32		;edx =32
	sub edx,ecx		;32 - wat die overflow was
	mov ecx,edx
	shl eax,cl		;clear wat gewrite is
	shr eax,cl

	mov code,eax		;stoor nuwe code

        lea ecx,buf		;adress of buffer to write from
	mov ebx,file2
        mov edx,1		;number of bytes to write
        mov eax,4
        int 0x80		;write the byte
        cmp eax,0
        jl near .write_error

      jmp .write_loop

;===================   PAD DIE LAASTE BYTE  =====================
.pad:

	cmp dword size,0		;nie nodig om te pad
	je near .close_files

	mov ecx,8
	sub ecx,size		;hoeveel bits te min
	mov edx,code

	shl edx,cl		;shift tot bits te min aan einde sit
	;mov code,edx
	mov ebx,0
	mov bl,dl		;sit code wat geskryf word in buffer
	mov buf,ebx

	lea ecx,buf                 ;adress of buffer to write from
	mov ebx,file2
        mov edx,1
        mov eax,4                   ;write a byte
        int 0x80
        cmp eax,0
	jl .write_error


      jmp .close_files


;=========================== MOONTLIKE ERRORS =======================
    .file_error:                 ;else file error
    push eax
    push msg_2
    call printf
    add esp,8
    jmp .end_prog

    .read_error:		;else read error
    push eax
    push msg_3
    call printf
    add esp,8
    jmp .end_prog

    .write_error:		;else write error
    push eax
    push msg_4
    call printf
    add esp,8
    jmp .end_prog


;============================= CLOSE DIE FILES WAT GEBRUIK IS =============================
  .close_files:

  mov eax,6
  mov ebx,file1
  int 0x80

  mov eax,6
  mov ebx,file2
  int 0x80

;============================ VERLAAT PROGRAM =================================
  .end_prog:
pushad
push dword [esp]
push msg_13
call printf
add esp,8
popad
  pop   edi
  pop   esi
  pop   ebx

  mov   esp, ebp
  pop   ebp
  xor   eax, eax    ; return 0;
  ret

.data
  msg_1 db "Usage: compress <filename>", 0x0a, 0
  msg_2 db "file error code:%d", 0x0a, 0
  msg_3 db "read error code:%d", 0x0a, 0
  msg_4 db "write error code:%d", 0x0a, 0
  msg_5 db "Compressing to %s", 0x0a, 0

  O_trunc EQU 1000q
  O_rdwr  EQU 2q


  msg_voor db "Voor jy dit doen", 0x0a, 0
  msg_na db "Na jy dit doen", 0x0a, 0
  msg_13 db "H.N = %d", 0x0a, 0
  msg_14 db "Heap node %d has frequency %d and char %c", 0x0a, 0
  msg_ma db "MA SE", 0x0a, 0
  msg_12 db "Huffman node %d has code %d and size %d", 0x0a, 0
msg_22 db "H.c = %c", 0x0a, 0
