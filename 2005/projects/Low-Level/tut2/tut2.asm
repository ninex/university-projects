;*****************************************************************************
;* CS252: Assembler implementation of external functions defined in          *
;*        'tut2.h'.                                                          *
;*****************************************************************************

global fact
global power
global ackerman
global swap
global binary_search

;*****************************************************************************
;* int fact(int n);                                                          *
;*****************************************************************************
%define n [ebp+8]
fact:
  push  ebp
  mov   ebp, esp

  cmp dword n, 0
  je .return_1           ;if n==0

  mov eax, n
  dec eax
  push eax              ;eax = n-1
  call fact             ;fact(n-1)
  add esp, 4
  imul dword n          ;n*fact(n-1)
  jmp .end_recursive
  
.return_1:
  mov eax,1
  
.end_recursive:

  mov   esp, ebp
  pop   ebp
  ret
;*****************************************************************************
;* int power(int x, int y);                                                  *
;*****************************************************************************
%define x [ebp+8]
%define y [ebp+12]
power:
  push  ebp
  mov   ebp, esp

  cmp dword y, 0
  je .return_1             ;if y==0  goto return_1

  dec dword y
  push dword y
  push dword x
  call power
  add esp,8
  imul dword x
  
  jmp .return
.return_1:
  mov eax,1
.return:      
  mov   esp, ebp
  pop   ebp
  ret
;*****************************************************************************
;* int ackerman(int x, int y);                                               *
;*****************************************************************************
%define x [ebp+8]
%define y [ebp+12]
ackerman:
  push  ebp
  mov   ebp, esp

  cmp dword x, 0
  je .return_y1                         ;if x==0 goto return y+1

  cmp dword y, 0
  jne .recursive_two                     ;if y==0

  mov eax,x
  dec eax                                ;return ackerman(x-1,1)
  push dword 1
  push eax
  call ackerman
  add esp,8
  jmp .return
  
.recursive_two:                           ;else

  mov eax,y
  dec eax
  push eax
  push dword x
  call ackerman
  add esp,8
  push eax
  mov eax,x                                ;return ackerman(x-1,ackerman(x,y-1))
  dec eax
  push eax
  call ackerman
  add esp,8
  jmp .return
  
.return_y1:
  inc dword y
  mov eax, y
  
.return:  
  mov   esp, ebp
  pop   ebp
  ret
;*****************************************************************************
;* void swap(int *x, int *y);                                                *
;*****************************************************************************
%define x_ptr [ebp+8]
%define y_ptr [ebp+12]
swap:
  push  ebp
  mov   ebp, esp
  
  mov   edi, x_ptr
  mov   esi, y_ptr
  
  mov   eax, [edi]
  xchg  eax, [esi]
  xchg  eax, [edi]
  
  mov   esp, ebp
  pop   ebp
  ret
  
;*****************************************************************************
;* int binary_search_c(int n, int list[], int low, int high);                *
;*****************************************************************************
%define n [ebp+8]
%define list [ebp+12]
%define low [ebp+16]
%define high [ebp+20]
binary_search:
  push  ebp
  mov   ebp, esp

  mov eax, low
  cmp eax, dword high            ;if low > high return -1
  jg .return_error

  add eax, high                  ;middle = (low+high)/2
  shr eax,1

  mov esi, list
  mov ebx, [esi+eax*4]             ;if (n == list[middle])  return middle
  cmp dword n,ebx
  
  je .einde
  

  jg .n_bigger                  ;if (n < list[middle])
  dec eax                        ;  high = middle-1
  mov high, eax
  jmp .recursive_call
  
.n_bigger:
  inc eax                        ;else low = middle+1
  mov low,eax
  
.recursive_call:
  push dword high
  push dword low
  push dword list
  push dword n                   ;return binary_search(n,list,low,high)
  call binary_search
  add esp,16
  jmp .einde
  
.return_error:
  mov eax, -1                    ;return -1
  
.einde:
      
  mov   esp, ebp
  pop   ebp
  ret