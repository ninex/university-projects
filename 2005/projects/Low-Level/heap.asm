global heap_initialize
global heap_remove
global heap_insert

extern malloc
extern printf
;***********************************************
;void heap_initialize(heap *H);
;***********************************************
%define h [ebp+8]

heap_initialize:
  push ebp
  mov ebp,esp

  ;push dword 8
  ;call malloc 			;memory vir n en array
  ;add esp,4

  mov edi,h
  ;mov [esi],eax
  ;mov edi,[esi]			;n = 0
  ;mov [edi],dword 0

;
; 
;   push dword 4100
;   call malloc			;memory vir 256 *16 +4 = 4100
;   add esp,4
;   ;mov edi,[eax]
;   mov edi,[eax]
; 
;   mov [esi],edi


; pushad
; push dword [eax]
; push msg_13
; call printf
; add esp,8
; popad
  ;mov edi,eax
  cld
  mov ecx,1025
  ;lea edi,h			;clear alles
  mov eax,0
  rep stosd

  mov   esp, ebp
  pop   ebp
  ret

;***********************************************
;void heap_insert(heap *H, heap_node *node);
;***********************************************
%define h [ebp+8]
%define node [ebp+12]
%define temp [ebp-4]
heap_insert:

  push ebp
  mov ebp,esp
  sub esp,4


  mov esi,h
  ;mov esi,[esi]

  mov eax,[esi]
  lea edi,[esi+4]
  mov esi,node
;   pushad
;   push eax
; push msg_13
; call printf
; add esp,8
; popad
; pushad
; push dword [esi+4]
; push dword [esi]
; push msg_14
; call printf
; add esp,12
; popad
  pushad
  cld
  lea ecx,[8*eax]
  lea edi,[edi+2*ecx]               ;h.a[h.n]=node
  mov ecx,4
  rep movsd
  popad

  mov ebx,eax                 ;child = h.n
  dec ebx                     ;parent = (child-1)/2
  sar ebx,1

  .while:
    cmp ebx,0
    jl near .end_while              ;while parent >= 0

    lea esi,[8*eax]
    mov ecx,[edi+(2*esi)]      ;if h.a[parent] > h.a[child]

    lea esi,[8*ebx]
    mov edx,[edi+(4*esi)]


    cmp edx,ecx
    jle .else_if

;================================= SWAP ============================
pushad
       lea ecx,[8*eax]
       lea edx,[8*ebx]
       lea esi,[edi + 2*edx]
       lea edi,[edi + 2*ecx]

       mov ecx,[esi]
       xor ecx,[edi]
       xor [edi],ecx
       xor [esi],ecx

       mov ecx,[esi+4]
       xor ecx,[edi+4]
       xor [edi+4],ecx
       xor [esi+4],ecx

       mov ecx,[esi+8]
       xor ecx,[edi+8]
       xor [edi+8],ecx
       xor [esi+8],ecx

       mov ecx,[esi+12]
       xor ecx,[edi+12]
       xor [edi+12],ecx
       xor [esi+12],ecx
popad
;===================================================================

      mov eax,ebx               ;child = parent

      dec ebx                   ;parent = (child-1)/2
      sar ebx,1

      jmp .while
    .else_if:                   ;else
      mov ebx,-1
    jmp .while
  .end_while:

  mov eax,h
  ;mov eax,[eax]
  inc dword [eax]

  mov   esp, ebp
  pop   ebp
  ret


;***********************************************
;void heap_remove(heap *H, heap_node *node);
;***********************************************
%define h [ebp+8]
%define node [ebp+12]
heap_remove:
  push ebp
  mov ebp,esp

  mov esi,h
  ;mov esi,[esi]


  cmp [esi],dword 0            ;if h.n >= 0
  jl near .end_if


    lea edi,[esi+4]		;adres waar array begin
pushad
    cld
    lea esi,[edi]
    mov edi,node               ;node = H.a[0]
    mov ecx,4
    rep movsd
popad
    dec dword [esi]             ;h.n = h.n -1


    mov eax,[esi]

    cld
    pushad                   ;H.a[0] = H.a[H.n]
    lea ecx,[eax*8]
    lea esi,[edi+ecx*2]
    mov ecx,4
    rep movsd
    popad

    mov eax,0                   ;parent = 0
    mov ebx,1                   ;child = 1


    .while:

      mov edx,[esi]
      dec edx
      cmp ebx, edx            ;while (child <= H.n-1)
      jg near .end_while

         lea ecx,[8*ebx]
         lea edx,[edi+(ecx*2)+16]
         mov ecx,[edx-16]


         cmp ecx, [edx]           ;if h.a[child].frequency >= h.a[child+1].frequency
         jl .continue
           inc ebx                       ;child = child +1
	   mov ecx,[edx]
	 .continue:

         lea edx,[8*eax]
         lea edx,[edi+edx*2]

	 cmp ecx,[edx]           ;if h.a[child] <= h.a[parent]
         jg .else

;================= SWAP ====================
pushad
       lea ecx,[8*eax]
       lea edx,[8*ebx]
       lea esi,[edi + 2*edx]
       lea edi,[edi + 2*ecx]

       mov ecx,[esi]
       xor ecx,[edi]
       xor [edi],ecx
       xor [esi],ecx

       mov ecx,[esi+4]
       xor ecx,[edi+4]
       xor [edi+4],ecx
       xor [esi+4],ecx

       mov ecx,[esi+8]
       xor ecx,[edi+8]
       xor [edi+8],ecx
       xor [esi+8],ecx

       mov ecx,[esi+12]
       xor ecx,[edi+12]
       xor [edi+12],ecx
       xor [esi+12],ecx
popad
;===========================================

	  mov eax,ebx		;parent = child
	  shl ebx,1
	  inc ebx		;child = (parent*2) + 1

	  jmp .while

	 .else:

          mov ebx,[esi]                   ;child = h.n

      jmp .while
    .end_while:

  .end_if:
; pushad
; push dword [esp]
; push msg_13
; call printf
; add esp,8
; popad
  mov   esp, ebp
  pop   ebp
  ret
.data
msg_13 db "Stack in HEAP = %d", 0x0a, 0
msg_14 db "POEFGER has frequency %d and char %c", 0x0a, 0
