;*****************************************************************************
;* CS252: Assembler implementation of external functions defined in          *
;*        'tut1.h'.                                                          *
;*****************************************************************************

global max
global power
global gcd

;*****************************************************************************
;* int max(int x, int y);                                                    *
;*****************************************************************************
%define y [ebp+12]
%define x [ebp+8]
max:
  push  ebp
  mov   ebp, esp
  
  mov   eax, x                     ; if (x > y)
  cmp   eax, y                     ;   return x;
  jg    .else                      ; else
  mov   eax, y                     ;   return y;
                                   
.else:
  mov   esp, ebp
  pop   ebp
  ret

;*****************************************************************************
;* int power(int x, int y);                                                  *
;*****************************************************************************
%define y [ebp+12]
%define x [ebp+8]
power:
  push ebp
  mov ebp,esp

  mov eax,1
  push eax
.while:

  mov ebx,y               
  cmp ebx,0               ;while y > 0
  jna .endwhile

  mov eax,y
  mov edx,0                ;y mod 2
  mov ebx,2
  div ebx

  cmp edx,1                ;(y mod 2) == 1
  je .equal

.else:
  mov eax,y
  shr eax,1               ;y=y/2
  mov y,eax 

  mov eax,x
  imul eax                 ;x=x*x
  mov x,eax

  jmp .while
  
.equal:
  mov eax,y
  sub eax,1                    ;y=y-1
  mov y,eax

  mov ebx,x
  pop eax                     ;z=z*x
  imul ebx
  push eax
  
  jmp .while
  
.endwhile:

  pop eax;
  mov esp,ebp
  pop ebp
  ret
  
;*****************************************************************************
;* int gcd(int m, int n);                                                    *
;*****************************************************************************
%define n [ebp+12]
%define m [ebp+8]
gcd:
  push bp
  mov bp,sp

.do_start:                 ;do

  mov eax,m
  mov edx,0                ;ebx = m mod n
  mov ecx,n
  div ecx
  mov ebx,edx

  cmp ebx,0
  je .do_start              ;if ebx != 0

  mov eax,n
  mov m,eax                ;m=n
  mov n,ebx                ;n=r
  

  cmp ebx,0
  je .end_while           ;while ebx!=0
  jmp .do_start
  
.end_while:
  mov eax,n

  mov sp,bp
  pop bp
  ret
