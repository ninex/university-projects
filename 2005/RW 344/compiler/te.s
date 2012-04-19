	.file	"te.c"
	.section	.rodata
.LC0:
	.string	"w"
.LC1:
	.string	"outa.txt"
.LC2:
	.string	"r"
.LC3:
	.string	"waarde %ld\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	andl	$-16, %esp
	movl	$0, %eax
	addl	$15, %eax
	addl	$15, %eax
	shrl	$4, %eax
	sall	$4, %eax
	subl	%eax, %esp
	movl	$2, -4(%ebp)
	movl	$5, -8(%ebp)
	movl	$4, -12(%ebp)
	leal	-4(%ebp), %eax
	sall	$26, (%eax)
	leal	-8(%ebp), %eax
	sall	$21, (%eax)
	leal	-12(%ebp), %eax
	sall	$16, (%eax)
	movl	-8(%ebp), %eax
	orl	-4(%ebp), %eax
	orl	-12(%ebp), %eax
	movl	%eax, -16(%ebp)
	subl	$8, %esp
	pushl	$.LC0
	pushl	$.LC1
	call	fopen
	addl	$16, %esp
	movl	%eax, -24(%ebp)
	pushl	-24(%ebp)
	pushl	$1
	pushl	$4
	leal	-16(%ebp), %eax
	pushl	%eax
	call	fwrite
	addl	$16, %esp
	subl	$12, %esp
	pushl	-24(%ebp)
	call	fclose
	addl	$16, %esp
	subl	$8, %esp
	pushl	$.LC2
	pushl	$.LC1
	call	fopen
	addl	$16, %esp
	movl	%eax, -28(%ebp)
	pushl	-28(%ebp)
	pushl	$1
	pushl	$4
	leal	-20(%ebp), %eax
	pushl	%eax
	call	fread
	addl	$16, %esp
	subl	$8, %esp
	pushl	-20(%ebp)
	pushl	$.LC3
	call	printf
	addl	$16, %esp
	leave
	ret
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.1 (Mandrakelinux 10.1 3.4.1-4mdk)"
