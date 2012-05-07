;*****************************************************************************
;* CS252: Assembler implementation of external functions defined in          *
;*        'tut3.h'.                                                          *
;*****************************************************************************

extern free
extern printf

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
binary_sort:
  push  ebp
  mov   ebp, esp

sub esp,40

cmp dword list,0
je near .einde

cld

;while (i < n)
mov ebx,1
.while:
cmp ebx,n
jge near .einde

pushad
push dword [esp]
push esp
push msg_stack
call printf
add esp,12
popad

;temp = list[i]
std
mov edx,list
lea esi,[(ebx)*8+ebx]
lea esi,[edx+esi*4+32]
lea edi,[ebp-4]
mov ecx,9
rep movsd

pushad
push dword [esp-4]
push esp
push msg_stack
call printf
add esp,12
popad
;bottom = 0
;top = i -1
mov eax,0
mov top,ebx
dec dword top

;while (bottom <= top)
.while1:
cmp eax,top
jg .ewhile1

;middle = (bottom + top)/1
mov edx,eax
add edx,top
shr edx,1

; pushad
; push msg_p
; call printf
; add esp,4
; popad
;if (temp.number < list[middle].number)
lea edi,[edx*4]
lea edi,[ebp+4+edi*8]
lea esi,temp

; pushad
; push msg_p2
; call printf
; add esp,4
; popad

cmpsd
jge .else
;top = middle-1;
mov top,edx
dec dword top

jmp .while1
.else:
;bottom = middle+1;
mov eax,edx
dec eax

; pushad
; push msg_p2
; call printf
;  add esp,4
; popad


jmp .while1
.ewhile1:
pushad
push msg_p2
call printf
 add esp,4
popad


;j = i-1;
mov edx,ebx
dec edx

;while (j >= bottom)
.while2:
cmp edx,eax
jl .ewhile2

;list[j+1] = list[j];


lea edi,[edx*4]
lea edi,[ebp+4+edi*8]
lea esi,[esi+36]
mov ecx,9
rep movsd



;j = j-1;
dec edx

jmp .while2
.ewhile2:

;list[bottom] = temp;
lea edi,[eax*4]
lea edi,[ebp+4+edi*8]
lea esi,temp
mov ecx,9
rep movsd


inc ebx
jmp .while

.einde:
pushad
push msg_p
call printf
add esp,4
popad

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
  
  mov   esp, ebp
  pop   ebp
  ret

.data
msg_p db "bleh" ,0x0a ,0
msg_p2 db "bleh2" ,0x0a ,0
msg_stack db "stack address %d has value %d", 0x0a ,0