global insert


;void insert(node **first, node *n);
%define first [ebp+8]
%define n [ebp+12]
insert:

push ebp
mov ebp,esp

mov esi,n
mov edi,first

mov eax,[edi]
cmp eax,0
jne .not_empty

mov [edi],esi

jmp .einde

.not_empty:
cmp eax,0
je .einde

mov ebx,[eax+4]
mov ecx,[eax]

cmp ecx,[esi]
jl .not_smaller

mov [esi+4],ebx
mov [eax+4],esi


jmp .einde
mov eax,[eax+4]
jmp .klaar

.not_smaller:
mov ecx,eax
mov eax,[eax+4]

jmp .not_empty
.einde:
mov [ecx+4],esi
.klaar:

mov esp,ebp
pop ebp
ret
