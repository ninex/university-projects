global min_max

;void min_max(int a[], int n, int *min, int *max);
%define a [ebp+8]
%define n [ebp+12]
%define min [ebp+16]
%define max [ebp+20]
min_max:
push ebp
mov ebp,esp

mov ebx,0

mov esi,a
mov eax,min
mov ecx,max

mov dword [eax],0
mov dword [ecx],0

.while:
cmp ebx,n
jge .einde

mov edi,[esi+ebx*4]

cmp edi,[eax]
jge .trymax
mov [eax],edi

.trymax:
cmp edi,[ecx]
jle .cont
mov [ecx],edi

.cont:
inc ebx

jmp .while

.einde:

mov esp,ebp
pop ebp
ret