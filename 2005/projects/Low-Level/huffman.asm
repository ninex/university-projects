global huffman_build_tree
global huffman_initialize_table
global huffman_build_table

extern heap_remove
extern heap_insert

extern malloc
extern printf

;***********************************************
;void huffman_build_tree(heap *h, heap_node **t);
;***********************************************
%define h [ebp+8]
%define t [ebp+12]
%define Z [ebp-16]
%define left [ebp-20]
%define right [ebp-24]

huffman_build_tree:
  push ebp
  mov ebp,esp
  sub esp,24

  mov esi,h
  lea edi,[esi+4]

  .while:
    mov esi,h
    cmp [esi],dword 1

    jl near .end_while            ;while (h.n >= 0)


        jg .else            ;if h.n = 0

        push dword 16
        call malloc
        add esp,4

	mov esi,t
	mov [esi],eax

        mov ebx,h
        push eax
        push ebx	;remove node and make root of t
        call heap_remove
	add esp,8

        jmp near .end_while

      .else:

;============== REMOVE 2 ENTRIES FROM HEAP ================

pushad

         push dword 16
         call malloc
         add esp,4
         mov left,eax

	push eax
        mov eax,h
        push eax
        call heap_remove             ;remove first entry from heap
        add esp,8
popad


pushad
	push dword 16
	call malloc
	add esp,4
	mov right,eax

        push eax
	mov eax,h
        push eax
        call heap_remove             ;remove second entry from heap
        add esp,8
popad



;============= MAKE LEFT AND RIGHT CHILDREN OF NEW NODE Z =============

	lea edx,Z
	mov ebx,left
	mov ecx,right

        mov [edx+4],dword -1
        mov [edx+8],ebx                  ;assign children X en Y
        mov [edx+12],ecx

        mov ebx,[ebx]
        add ebx,[ecx]                    ;z.frequency = x.frequency + y.frequency
        mov [edx],ebx


;====================== INSERT NEW NODE Z =============================

        lea eax,Z
        push eax
        mov eax,h
        push eax
        call heap_insert		;insert z
        add esp,8


        jmp .while

  .end_while:


  mov   esp, ebp
  pop   ebp
  ret

;***********************************************
;void huffman_initialize_table(huffman_node *t);
;***********************************************
%define t [ebp+8]
huffman_initialize_table:
  push ebp
  mov ebp,esp

;===================== MAAK MEMORY VIR 256 nodes =====================
  push dword 2048
  call malloc
  add esp,4

  cld
  mov edi,t
  mov [edi],eax
  mov edi,eax                       ;clear the memory
  mov eax,0
  mov ecx,512
  rep stosd

  mov   esp, ebp
  pop   ebp
  ret

;***********************************************
;void huffman_build_table(heap_node *root, huffman_node *t, int code, int size);
;***********************************************
%define root [ebp+8]
%define t [ebp+12]
%define code [ebp+16]
%define size [ebp+20]

huffman_build_table:
  push ebp
  mov ebp,esp

  mov esi,root
  mov esi,[esi]

  cmp esi,dword 0                 ;if root != null
  je  near .not_a_node

  mov edi,t
  mov edi,[edi]
  mov ebx,code
  mov ecx,size

  inc ecx			;increase size before traverse
  shl ebx,1			;make place for traverse left code

;============================= TRAVERSE LEFT ====================
pushad
  lea eax,[esi+8]
  mov edi,t
  push ecx
  push ebx
  push edi
  push eax		                 ;huffman_build_table(root.left,t,code,size)
  call huffman_build_table
  add esp,16
popad
;======================= WRITE THE CODE AND SIZE =====================
pushad
  mov eax,root
  mov esi,[eax]
  mov eax,[esi+4]

  cmp eax,-1                          ;if root.char != -1
  je .not_a_char
  	shr ebx,1
	lea esi,[edi+8*(eax)]
	mov dword [esi],ebx		;t[char].huffman_code = code
	mov dword [esi+4],ecx		;t[char].bit_size = size
  .not_a_char:
popad

;=================== TRAVERSE RIGHT =======================
  inc ebx			;add traverse right code

  mov edi,t
  lea eax,[esi+12]

  push ecx
  push ebx
  push edi
  push eax	                 ;huffman_build_table(root.right,t,code,size)
  call huffman_build_table
  add esp,16


  .not_a_node:

  mov   esp, ebp
  pop   ebp
  ret
.data
msg_13 db "Stack H.N = %d", 0x0a, 0
