global upcase

;extern void upcase(char *s);
%define s [ebp+8]
upcase:
push ebp
mov ebp,esp


mov ebx,-1

.do_while:

inc ebx

mov esi,s
lea esi,[esi+ebx]

mov al,[esi]

cmp al,97
jl .cont
cmp al,122
jg .cont
sub byte [esi],32
.cont:


cmp byte [esi],0
je .einde
jmp .do_while
.einde:

mov esp,ebp
pop ebp
ret
