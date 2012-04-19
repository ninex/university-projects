;*****************************************************************************
;* CS252: Assembler implementation of external functions defined in          *
;*        'tut3.h'.                                                          *
;*****************************************************************************

extern free

global binary_sort
global delete_node
global bin_to_string

;*****************************************************************************
;* void binary_sort(item *list, int n);                                      *
;*****************************************************************************
%define list   [ebp+8]
%define n      [ebp+12]
%define temp [ebp-36]
%define top [ebp-40]
%define middle [ebp-44]

binary_sort:
  push  ebp
  mov   ebp, esp
  sub esp,44

  cmp list, dword 0
  jz near .end_func

  mov ebx,1
.while:
  cmp ebx, n                       ;while i < n
  jge .end_func
  
    mov ecx,9
    mov esi,list                   ;temp = list[i];
    mov edi,temp
    rep movsd

    push dword 1                   ;bottom = 1
    mov top,ebx                    ;top = i -1
    dec dword top

    .while2:
    pop eax
    cmp eax,top                        ;while bottom <= top
    push eax
    jg .continue

      add eax,top
      shl eax,1                    ;middle = (bottom + top)/2
      mov middle,eax
      
      mov esi,list
      mov ecx,middle                 ;KYK hIER ROND
      mov eax,[esi+4*ecx]
      mov edi,temp                 ;if temp.number < list[middle].number
      cmp eax,[edi]
      mov eax,middle
      jge .else             
        mov top,eax
        dec dword top                    ;top = middle - 1
        jmp .while2
      .else:
        pop ecx
        inc eax                          ;else bottom = middle + 1
        push eax
        jmp .while2

    .continue:
    mov eax,ebx                          ;j = i - 1
    dec eax

    .while3:
    pop edx
    push edx
    cmp eax,edx                          ;while j >= bottom
    jl .continue2
      mov esi,list
      mov edi,[esi+4]                      ;list[j+1] = list[j]
      mov ecx,9                           ;bvdcbvfdbvfdvfdvfvfdvfdvfdvfdvfdv
      rep movsd

      dec eax                             ;j=j-1
      jmp .while3
        
    .continue2:
    
    mov esi,temp
    mov edx,middle
    mov ecx,list                          ;list[bottom] = temp
    mov edi,[ecx+4*edx]
    mov ecx,9
    rep movsd

    inc ebx                               ;i=i+1
    jmp .while
    
.end_func:  
  mov   esp, ebp
  pop   ebp
  ret

;*****************************************************************************
;* void remove_max(node **root, node **max);                                 *
;*****************************************************************************
%define root [ebp+8]
%define max  [ebp+12]
remove_max:
  push  ebp
  mov   ebp, esp
  
  mov   esp, ebp
  pop   ebp
  ret

;*****************************************************************************
;* void delete_node(node **root, char *name);                                *
;*****************************************************************************
%define root [ebp+8]
%define name [ebp+12]
delete_node:
  push  ebp
  mov   ebp, esp
  
  mov   esp, ebp
  pop   ebp
  ret  
  
;*****************************************************************************
;* void bin_to_string(int n, char *s);                                       *
;*****************************************************************************
%define n [ebp+8]
%define s [ebp+12]

bin_to_string:
  push  ebp
  mov   ebp, esp

  mov ecx,0x80000000
  mov edi,s
  
  mov edx,0
.while:
 cmp edx, 32
 jge .end_while

 and ecx,n
 
 cmp ebx,0
 je .else
 
 mov esi,1
 ;movsb
 jmp .continue
.else:
 mov esi,0
 ;movsb
.continue:
  inc edx
  shr ecx,1
  jmp .while
.end_while:
  mov esi,0
  ;movsb
   
  mov   esp, ebp
  pop   ebp
  ret 
