	.file	"parser.c"
	.text
.globl parse
	.type	parse, @function
parse:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	set_count
	call	getToken
	movl	%eax, token
	cmpl	$0, token
	je	.L2
	movl	$1, need_priority
	call	Program
	movl	%eax, -4(%ebp)
	jmp	.L1
.L2:
	movl	$0, -4(%ebp)
.L1:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	parse, .-parse
	.section	.rodata
.LC0:
	.string	"%s"
	.text
.globl expect
	.type	expect, @function
expect:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	token, %eax
	movl	32(%eax), %eax
	cmpl	8(%ebp), %eax
	jne	.L5
	call	getToken
	movl	%eax, token
	movl	$1, -4(%ebp)
	jmp	.L4
.L5:
	cmpl	$0, 12(%ebp)
	je	.L7
	cmpl	$0, need_priority
	je	.L7
	subl	$4, %esp
	subl	$8, %esp
	pushl	8(%ebp)
	call	set_error
	addl	$12, %esp
	pushl	%eax
	pushl	$.LC0
	pushl	$last_expected
	call	sprintf
	addl	$16, %esp
	subl	$4, %esp
	subl	$8, %esp
	movl	token, %eax
	pushl	32(%eax)
	call	set_error
	addl	$12, %esp
	pushl	%eax
	pushl	$.LC0
	pushl	$last_found
	call	sprintf
	addl	$16, %esp
	subl	$12, %esp
	pushl	$5
	call	ERR_HANDLER
	addl	$16, %esp
	movl	$0, need_priority
.L7:
	movl	$0, -4(%ebp)
.L4:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	expect, .-expect
.globl expect_err
	.type	expect_err, @function
expect_err:
	pushl	%ebp
	movl	%esp, %ebp
	movl	$last_expected, %eax
	popl	%ebp
	ret
	.size	expect_err, .-expect_err
.globl found
	.type	found, @function
found:
	pushl	%ebp
	movl	%esp, %ebp
	movl	$last_found, %eax
	popl	%ebp
	ret
	.size	found, .-found
.globl closeItem
	.type	closeItem, @function
closeItem:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax
	cmpl	$3, (%eax)
	jne	.L11
	cmpl	$0, 12(%ebp)
	je	.L11
	subl	$12, %esp
	movl	8(%ebp), %eax
	pushl	12(%eax)
	call	clearReg
	addl	$16, %esp
.L11:
	subl	$12, %esp
	pushl	8(%ebp)
	call	free
	addl	$16, %esp
	leave
	ret
	.size	closeItem, .-closeItem
.globl clear_list
	.type	clear_list, @function
clear_list:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
.L13:
	cmpl	$0, list
	je	.L12
	movl	list, %eax
	movl	%eax, -4(%ebp)
	movl	list, %eax
	movl	4(%eax), %eax
	movl	%eax, list
	subl	$8, %esp
	pushl	$1
	movl	-4(%ebp), %eax
	pushl	(%eax)
	call	closeItem
	addl	$16, %esp
	subl	$12, %esp
	pushl	-4(%ebp)
	call	free
	addl	$16, %esp
	jmp	.L13
.L12:
	leave
	ret
	.size	clear_list, .-clear_list
.globl newItem
	.type	newItem, @function
newItem:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	$0, 4(%eax)
	movl	-4(%ebp), %eax
	movl	$0, 8(%eax)
	movl	-4(%ebp), %eax
	movl	$0, 16(%eax)
	movl	-4(%ebp), %eax
	movl	$0, (%eax)
	movl	-4(%ebp), %eax
	movl	$0, 32(%eax)
	movl	-4(%ebp), %eax
	movl	$0, 24(%eax)
	movl	-4(%ebp), %eax
	leave
	ret
	.size	newItem, .-newItem
	.section	.rodata
.LC1:
	.string	"_main"
.LC2:
	.string	"true"
.LC3:
	.string	"false"
	.text
.globl Program
	.type	Program, @function
Program:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, level
	movl	$0, list
	subl	$8, %esp
	pushl	$.LC1
	pushl	$proc
	call	sprintf
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$proc
	call	lookup_proc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	$1, 36(%eax)
	pushl	$1
	pushl	$6
	pushl	$.LC2
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	pushl	$0
	pushl	$6
	pushl	$.LC3
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	movl	$0, -8(%ebp)
	call	Block
	testl	%eax, %eax
	je	.L17
	subl	$8, %esp
	pushl	$1
	pushl	$39
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L17
	subl	$8, %esp
	pushl	$1
	pushl	$-1
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L17
	movl	$1, -8(%ebp)
.L17:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	Program, .-Program
.globl Block
	.type	Block, @function
Block:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$1
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L19
	incl	level
	call	DefinitionPart
	testl	%eax, %eax
	je	.L20
	subl	$8, %esp
	pushl	$0
	pushl	$proc
	call	lookup_proc
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	40(%eax), %eax
	movl	%eax, -12(%ebp)
	subl	$8, %esp
	pushl	$proc
	pushl	$.LC1
	call	strcmp
	addl	$16, %esp
	testl	%eax, %eax
	je	.L21
	movl	-8(%ebp), %ebx
	subl	$12, %esp
	movl	-12(%ebp), %eax
	negl	%eax
	pushl	%eax
	call	procEnter
	addl	$16, %esp
	movl	%eax, 44(%ebx)
	jmp	.L22
.L21:
	subl	$12, %esp
	movl	-12(%ebp), %eax
	negl	%eax
	pushl	%eax
	call	start
	addl	$16, %esp
.L22:
	call	StatementPart
	testl	%eax, %eax
	je	.L23
	decl	level
	subl	$8, %esp
	pushl	$proc
	pushl	$.LC1
	call	strcmp
	addl	$16, %esp
	testl	%eax, %eax
	je	.L24
	subl	$12, %esp
	movl	-12(%ebp), %eax
	negl	%eax
	pushl	%eax
	call	procReturn
	addl	$16, %esp
	jmp	.L25
.L24:
	call	Close
.L25:
	subl	$12, %esp
	pushl	$proc
	call	remove_proc
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$6
	call	expect
	addl	$16, %esp
	movl	%eax, -16(%ebp)
	jmp	.L18
.L23:
	movl	$0, -16(%ebp)
	jmp	.L18
.L20:
	movl	$0, -16(%ebp)
	jmp	.L18
.L19:
	movl	$0, -16(%ebp)
.L18:
	movl	-16(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	Block, .-Block
.globl DefinitionPart
	.type	DefinitionPart, @function
DefinitionPart:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	Definition
	testl	%eax, %eax
	je	.L30
	subl	$8, %esp
	pushl	$1
	pushl	$19
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L31
	call	DefinitionPart
	movl	%eax, -4(%ebp)
	jmp	.L29
.L31:
	movl	$0, -4(%ebp)
	jmp	.L29
.L30:
	movl	$1, -4(%ebp)
.L29:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	DefinitionPart, .-DefinitionPart
.globl Definition
	.type	Definition, @function
Definition:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	call	ConstantDefinition
	testl	%eax, %eax
	jne	.L36
	call	VariableDefinition
	testl	%eax, %eax
	jne	.L36
	call	ProcedureDefinition
	testl	%eax, %eax
	jne	.L36
	jmp	.L35
.L36:
	movl	$1, -4(%ebp)
.L35:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	Definition, .-Definition
.globl ConstantDefinition
	.type	ConstantDefinition, @function
ConstantDefinition:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$56, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$4
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L38
	subl	$8, %esp
	pushl	$1
	pushl	$0
	call	ConstantName
	addl	$16, %esp
	testl	%eax, %eax
	je	.L39
	subl	$8, %esp
	pushl	$1
	pushl	$21
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L39
	call	newItem
	movl	%eax, -44(%ebp)
	subl	$4, %esp
	pushl	$const_name
	pushl	$.LC0
	leal	-40(%ebp), %eax
	pushl	%eax
	call	sprintf
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	-44(%ebp)
	call	Constant
	addl	$16, %esp
	testl	%eax, %eax
	je	.L40
	pushl	val
	pushl	type
	leal	-40(%ebp), %eax
	pushl	%eax
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L41
	subl	$12, %esp
	pushl	$6
	call	ERR_HANDLER
	addl	$16, %esp
.L41:
	subl	$8, %esp
	pushl	$1
	pushl	-44(%ebp)
	call	closeItem
	addl	$16, %esp
	movl	$1, -48(%ebp)
	jmp	.L37
.L40:
	movl	$0, -48(%ebp)
	jmp	.L37
.L39:
	movl	$0, -48(%ebp)
	jmp	.L37
.L38:
	movl	$0, -48(%ebp)
.L37:
	movl	-48(%ebp), %eax
	leave
	ret
	.size	ConstantDefinition, .-ConstantDefinition
.globl VariableDefinition
	.type	VariableDefinition, @function
VariableDefinition:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	call	TypeSymbol
	testl	%eax, %eax
	je	.L46
	call	VariableList
	testl	%eax, %eax
	je	.L47
	movl	$1, -16(%ebp)
	jmp	.L45
.L47:
	call	newItem
	movl	%eax, -4(%ebp)
	movl	$0, -8(%ebp)
	subl	$8, %esp
	pushl	$1
	pushl	$0
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L49
	subl	$8, %esp
	pushl	$1
	pushl	-4(%ebp)
	call	VariableName
	addl	$16, %esp
	testl	%eax, %eax
	je	.L49
	subl	$8, %esp
	pushl	$1
	pushl	$22
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L49
	subl	$8, %esp
	leal	-8(%ebp), %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	Constant
	addl	$16, %esp
	testl	%eax, %eax
	je	.L49
	subl	$8, %esp
	pushl	$1
	pushl	$23
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L49
	movl	$0, -12(%ebp)
	cmpl	$9, sym_type
	jne	.L50
	pushl	val
	pushl	$3
	pushl	$def_name
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	jmp	.L51
.L50:
	cmpl	$2, sym_type
	jne	.L51
	pushl	val
	pushl	$4
	pushl	$def_name
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	movl	%eax, -12(%ebp)
.L51:
	cmpl	$0, -12(%ebp)
	jne	.L53
	subl	$12, %esp
	pushl	$7
	call	ERR_HANDLER
	addl	$16, %esp
.L53:
	cmpl	$0, val
	jg	.L54
	subl	$12, %esp
	pushl	$8
	call	ERR_HANDLER
	addl	$16, %esp
.L54:
	subl	$8, %esp
	pushl	$1
	pushl	-4(%ebp)
	call	closeItem
	addl	$16, %esp
	movl	$1, -16(%ebp)
	jmp	.L45
.L49:
	subl	$8, %esp
	pushl	$1
	pushl	-4(%ebp)
	call	closeItem
	addl	$16, %esp
	movl	$0, -16(%ebp)
	jmp	.L45
.L46:
	movl	$0, -16(%ebp)
.L45:
	movl	-16(%ebp), %eax
	leave
	ret
	.size	VariableDefinition, .-VariableDefinition
.globl TypeSymbol
	.type	TypeSymbol, @function
TypeSymbol:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	token, %eax
	movl	32(%eax), %eax
	movl	%eax, sym_type
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$9
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L59
	subl	$8, %esp
	pushl	$0
	pushl	$2
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L59
	jmp	.L58
.L59:
	movl	$1, -4(%ebp)
.L58:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	TypeSymbol, .-TypeSymbol
.globl VariableList
	.type	VariableList, @function
VariableList:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$0
	call	VariableName
	addl	$16, %esp
	testl	%eax, %eax
	je	.L61
	movl	$0, -4(%ebp)
	cmpl	$9, sym_type
	jne	.L62
	pushl	$0
	pushl	$1
	pushl	$def_name
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	jmp	.L63
.L62:
	cmpl	$2, sym_type
	jne	.L63
	pushl	$0
	pushl	$2
	pushl	$def_name
	pushl	$proc
	call	insert_var
	addl	$16, %esp
	movl	%eax, -4(%ebp)
.L63:
	cmpl	$0, -4(%ebp)
	jne	.L66
	subl	$12, %esp
	pushl	$9
	call	ERR_HANDLER
	addl	$16, %esp
.L66:
	subl	$8, %esp
	pushl	$0
	pushl	$18
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L67
	call	VariableList
	movl	%eax, -8(%ebp)
	jmp	.L60
.L67:
	movl	$1, -8(%ebp)
	jmp	.L60
.L61:
	movl	$0, -8(%ebp)
.L60:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	VariableList, .-VariableList
.globl ProcedureDefinition
	.type	ProcedureDefinition, @function
ProcedureDefinition:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$11
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L70
	subl	$8, %esp
	pushl	$1
	pushl	$0
	call	ProcedureName
	addl	$16, %esp
	testl	%eax, %eax
	je	.L70
	subl	$4, %esp
	pushl	$def_name
	pushl	$.LC0
	pushl	$proc
	call	sprintf
	addl	$16, %esp
	subl	$8, %esp
	movl	level, %eax
	incl	%eax
	pushl	%eax
	pushl	$def_name
	call	insert_proc
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L71
	subl	$12, %esp
	pushl	$10
	call	ERR_HANDLER
	addl	$16, %esp
.L71:
	movl	$0, -8(%ebp)
	call	FormalParameters
	testl	%eax, %eax
	je	.L72
	call	Block
	testl	%eax, %eax
	je	.L72
	movl	$1, -8(%ebp)
.L72:
	movl	-8(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L69
.L70:
	movl	$0, -4(%ebp)
.L69:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	ProcedureDefinition, .-ProcedureDefinition
.globl FormalParameters
	.type	FormalParameters, @function
FormalParameters:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$37
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L75
	call	FPSection
	testl	%eax, %eax
	je	.L76
.L77:
	subl	$8, %esp
	pushl	$0
	pushl	$19
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L76
	call	FPSection
	testl	%eax, %eax
	jne	.L77
	subl	$12, %esp
	pushl	$30
	call	ERR_HANDLER
	addl	$16, %esp
	jmp	.L77
.L76:
	subl	$8, %esp
	pushl	$1
	pushl	$38
	call	expect
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	jmp	.L74
.L75:
	movl	$1, -4(%ebp)
.L74:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	FormalParameters, .-FormalParameters
.globl FPSection
	.type	FPSection, @function
FPSection:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	call	TypeSymbol
	testl	%eax, %eax
	je	.L82
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$15
	call	expect
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$0
	call	VariableName
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L84
	movl	$0, -12(%ebp)
	jmp	.L81
.L84:
	cmpl	$9, sym_type
	jne	.L85
	pushl	$0
	movl	-4(%ebp), %eax
	incl	%eax
	pushl	%eax
	pushl	$def_name
	pushl	$proc
	call	insert_param
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	jmp	.L86
.L85:
	cmpl	$2, sym_type
	jne	.L86
	pushl	$0
	movl	-4(%ebp), %eax
	addl	$2, %eax
	pushl	%eax
	pushl	$def_name
	pushl	$proc
	call	insert_param
	addl	$16, %esp
	movl	%eax, -8(%ebp)
.L86:
	cmpl	$0, -8(%ebp)
	jne	.L88
	subl	$12, %esp
	pushl	$7
	call	ERR_HANDLER
	addl	$16, %esp
.L88:
	movl	$1, -12(%ebp)
	jmp	.L81
.L82:
	movl	$0, -12(%ebp)
.L81:
	movl	-12(%ebp), %eax
	leave
	ret
	.size	FPSection, .-FPSection
.globl StatementPart
	.type	StatementPart, @function
StatementPart:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	Statement
	testl	%eax, %eax
	je	.L90
	subl	$8, %esp
	pushl	$1
	pushl	$19
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L91
	call	StatementPart
	movl	%eax, -4(%ebp)
	jmp	.L89
.L91:
	movl	$0, -4(%ebp)
	jmp	.L89
.L90:
	movl	$1, -4(%ebp)
.L89:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	StatementPart, .-StatementPart
.globl Statement
	.type	Statement, @function
Statement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	movl	$0, -8(%ebp)
	subl	$12, %esp
	leal	-4(%ebp), %eax
	pushl	%eax
	call	ProcedureStatement
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L96
	call	EmptyStatement
	testl	%eax, %eax
	jne	.L96
	call	ReadStatement
	testl	%eax, %eax
	jne	.L96
	call	WriteStatement
	testl	%eax, %eax
	jne	.L96
	subl	$12, %esp
	pushl	-4(%ebp)
	call	AssignmentStatement
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L96
	call	IfStatement
	testl	%eax, %eax
	jne	.L96
	call	DoStatement
	testl	%eax, %eax
	jne	.L96
	jmp	.L95
.L96:
	movl	$1, -8(%ebp)
.L95:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	Statement, .-Statement
.globl EmptyStatement
	.type	EmptyStatement, @function
EmptyStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$13
	call	expect
	addl	$16, %esp
	leave
	ret
	.size	EmptyStatement, .-EmptyStatement
.globl ReadStatement
	.type	ReadStatement, @function
ReadStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	clear_list
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$12
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L99
	subl	$12, %esp
	pushl	$0
	call	VariableAccessList
	addl	$16, %esp
	testl	%eax, %eax
	je	.L99
	movl	$1, -4(%ebp)
.L99:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	ReadStatement, .-ReadStatement
.globl VariableAccessList
	.type	VariableAccessList, @function
VariableAccessList:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	newItem
	movl	%eax, -8(%ebp)
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	-8(%ebp)
	call	VariableAccess
	addl	$16, %esp
	testl	%eax, %eax
	je	.L101
	subl	$12, %esp
	pushl	$8
	call	malloc
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %ebx
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, (%ebx)
	subl	$4, %esp
	pushl	$36
	pushl	-8(%ebp)
	movl	-12(%ebp), %eax
	pushl	(%eax)
	call	memcpy
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	$0, 4(%eax)
	cmpl	$0, list
	jne	.L102
	movl	-12(%ebp), %eax
	movl	%eax, list
	jmp	.L103
.L102:
	movl	list, %eax
	movl	%eax, -16(%ebp)
.L104:
	movl	-16(%ebp), %eax
	cmpl	$0, 4(%eax)
	je	.L105
	movl	-16(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -16(%ebp)
	jmp	.L104
.L105:
	movl	-16(%ebp), %edx
	movl	-12(%ebp), %eax
	movl	%eax, 4(%edx)
.L103:
	movl	-8(%ebp), %eax
	movl	$0, (%eax)
	subl	$8, %esp
	pushl	$0
	pushl	-8(%ebp)
	call	closeItem
	addl	$16, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$18
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L106
	subl	$12, %esp
	pushl	$0
	call	VariableAccessList
	addl	$16, %esp
	movl	%eax, -20(%ebp)
	jmp	.L100
.L106:
	movl	$1, -20(%ebp)
	jmp	.L100
.L101:
	movl	$0, -20(%ebp)
.L100:
	movl	-20(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	VariableAccessList, .-VariableAccessList
.globl WriteStatement
	.type	WriteStatement, @function
WriteStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$14
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L110
	subl	$12, %esp
	pushl	$1
	call	ExpressionList
	addl	$16, %esp
	testl	%eax, %eax
	je	.L110
	movl	$1, -4(%ebp)
.L110:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	WriteStatement, .-WriteStatement
	.section	.rodata
.LC4:
	.string	"\t\t\t befire here %d\n"
	.align 4
.LC5:
	.string	"Mode = %d var level %d, func level %d\n"
	.text
.globl ExpressionList
	.type	ExpressionList, @function
ExpressionList:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	call	newItem
	movl	%eax, -8(%ebp)
	subl	$12, %esp
	pushl	-8(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L112
	cmpl	$0, 8(%ebp)
	jne	.L113
	cmpl	$0, list
	jne	.L114
	subl	$12, %esp
	pushl	$16
	call	ERR_HANDLER
	addl	$16, %esp
	jmp	.L118
.L114:
	movl	list, %eax
	movl	(%eax), %eax
	movl	-8(%ebp), %edx
	movl	4(%eax), %eax
	cmpl	4(%edx), %eax
	jne	.L116
	movl	list, %eax
	movl	%eax, -12(%ebp)
	movl	list, %eax
	movl	4(%eax), %eax
	movl	%eax, list
	subl	$8, %esp
	pushl	-8(%ebp)
	movl	-12(%ebp), %eax
	pushl	(%eax)
	call	printItems
	addl	$16, %esp
	subl	$8, %esp
	movl	-8(%ebp), %eax
	pushl	24(%eax)
	pushl	$.LC4
	call	printf
	addl	$16, %esp
	subl	$4, %esp
	pushl	-8(%ebp)
	movl	-12(%ebp), %eax
	pushl	(%eax)
	pushl	$14
	call	op2
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	movl	-12(%ebp), %eax
	pushl	(%eax)
	call	closeItem
	addl	$16, %esp
	subl	$12, %esp
	pushl	-12(%ebp)
	call	free
	addl	$16, %esp
	jmp	.L118
.L116:
	subl	$12, %esp
	pushl	$17
	call	ERR_HANDLER
	addl	$16, %esp
	jmp	.L118
.L113:
	movl	-8(%ebp), %eax
	cmpl	$1, (%eax)
	jne	.L119
	pushl	level
	movl	-8(%ebp), %eax
	pushl	28(%eax)
	movl	-8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC5
	call	printf
	addl	$16, %esp
	movl	-8(%ebp), %eax
	movl	28(%eax), %eax
	cmpl	level, %eax
	jne	.L120
	movl	-8(%ebp), %ebx
	call	resetLevel
	movl	%eax, 32(%ebx)
	jmp	.L119
.L120:
	movl	-8(%ebp), %eax
	cmpl	$1, 28(%eax)
	jne	.L122
	movl	-8(%ebp), %ebx
	call	rootLevel
	movl	%eax, 32(%ebx)
	jmp	.L119
.L122:
	movl	-8(%ebp), %ebx
	call	changeLevel
	movl	%eax, 32(%ebx)
.L119:
	movl	-8(%ebp), %eax
	cmpl	$0, 24(%eax)
	je	.L124
	pushl	$0
	movl	-8(%ebp), %eax
	pushl	12(%eax)
	movl	-8(%ebp), %eax
	pushl	12(%eax)
	pushl	$12
	call	Put
	addl	$16, %esp
.L124:
	subl	$8, %esp
	pushl	-8(%ebp)
	pushl	$40
	call	op1
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	-8(%ebp)
	call	closeItem
	addl	$16, %esp
	call	clear_list
.L118:
	subl	$8, %esp
	pushl	$0
	pushl	$18
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L125
	subl	$12, %esp
	pushl	8(%ebp)
	call	ExpressionList
	addl	$16, %esp
	movl	%eax, -16(%ebp)
	jmp	.L111
.L125:
	cmpl	$0, list
	je	.L127
	subl	$12, %esp
	pushl	$18
	call	ERR_HANDLER
	addl	$16, %esp
.L127:
	movl	$1, -16(%ebp)
	jmp	.L111
.L112:
	movl	$0, -16(%ebp)
.L111:
	movl	-16(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	ExpressionList, .-ExpressionList
.globl AssignmentStatement
	.type	AssignmentStatement, @function
AssignmentStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	clear_list
	movl	$0, -4(%ebp)
	subl	$12, %esp
	pushl	8(%ebp)
	call	VariableAccessList
	addl	$16, %esp
	testl	%eax, %eax
	je	.L130
	subl	$8, %esp
	pushl	$1
	pushl	$28
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L130
	subl	$12, %esp
	pushl	$0
	call	ExpressionList
	addl	$16, %esp
	testl	%eax, %eax
	je	.L130
	movl	$1, -4(%ebp)
.L130:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	AssignmentStatement, .-AssignmentStatement
.globl ProcedureStatement
	.type	ProcedureStatement, @function
ProcedureStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$0
	call	ProcedureName
	addl	$16, %esp
	testl	%eax, %eax
	je	.L132
	subl	$8, %esp
	pushl	$1
	pushl	$def_name
	call	lookup_proc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	cmpl	$0, -4(%ebp)
	jne	.L133
	movl	8(%ebp), %eax
	movl	$1, (%eax)
	movl	$0, -12(%ebp)
	jmp	.L131
.L133:
	movl	-4(%ebp), %eax
	movl	%eax, -8(%ebp)
	subl	$12, %esp
	pushl	-8(%ebp)
	call	procCall
	addl	$16, %esp
	subl	$4, %esp
	pushl	$def_name
	pushl	$.LC0
	pushl	$call_name
	call	sprintf
	addl	$16, %esp
	call	ActualParameters
	movl	%eax, -12(%ebp)
	jmp	.L131
.L132:
	movl	$0, -12(%ebp)
.L131:
	movl	-12(%ebp), %eax
	leave
	ret
	.size	ProcedureStatement, .-ProcedureStatement
.globl newParam
	.type	newParam, @function
newParam:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$60
	call	malloc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	$0, 56(%eax)
	movl	-4(%ebp), %edx
	movl	8(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, 36(%edx)
	movl	-4(%ebp), %edx
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	movl	%eax, 40(%edx)
	movl	-4(%ebp), %eax
	leave
	ret
	.size	newParam, .-newParam
.globl ActualParameters
	.type	ActualParameters, @function
ActualParameters:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	movl	$0, -8(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$37
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L137
	call	newItem
	movl	%eax, -16(%ebp)
	subl	$12, %esp
	pushl	-16(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L138
	subl	$12, %esp
	pushl	-16(%ebp)
	call	newParam
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	movl	-8(%ebp), %eax
	movl	%eax, -12(%ebp)
.L139:
	subl	$8, %esp
	pushl	$0
	pushl	$18
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L138
	subl	$12, %esp
	pushl	-16(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L141
	subl	$12, %esp
	pushl	$31
	call	ERR_HANDLER
	addl	$16, %esp
.L141:
	movl	-12(%ebp), %ebx
	subl	$12, %esp
	pushl	-16(%ebp)
	call	newParam
	addl	$16, %esp
	movl	%eax, 56(%ebx)
	movl	-12(%ebp), %eax
	movl	56(%eax), %eax
	movl	%eax, -12(%ebp)
	jmp	.L139
.L138:
	subl	$8, %esp
	pushl	-8(%ebp)
	pushl	$call_name
	call	checkParams
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$38
	call	expect
	addl	$16, %esp
	movl	%eax, -20(%ebp)
	jmp	.L136
.L137:
	subl	$8, %esp
	pushl	-8(%ebp)
	pushl	$call_name
	call	checkParams
	addl	$16, %esp
	movl	$1, -20(%ebp)
.L136:
	movl	-20(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	ActualParameters, .-ActualParameters
.globl IfStatement
	.type	IfStatement, @function
IfStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$8
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L143
	call	GuardedCommandList
	testl	%eax, %eax
	je	.L143
	subl	$8, %esp
	pushl	$1
	pushl	$7
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L143
	movl	$1, -4(%ebp)
.L143:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	IfStatement, .-IfStatement
.globl DoStatement
	.type	DoStatement, @function
DoStatement:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	subl	$8, %esp
	pushl	$0
	pushl	$5
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L145
	call	GuardedCommandList
	testl	%eax, %eax
	je	.L145
	subl	$8, %esp
	pushl	$1
	pushl	$10
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L145
	movl	$1, -4(%ebp)
.L145:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	DoStatement, .-DoStatement
.globl GuardedCommandList
	.type	GuardedCommandList, @function
GuardedCommandList:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	call	GuardedCommand
	testl	%eax, %eax
	je	.L147
	subl	$8, %esp
	pushl	$0
	pushl	$32
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L148
	movl	$0, -8(%ebp)
	call	GuardedCommandList
	testl	%eax, %eax
	jne	.L150
	jmp	.L149
.L150:
	movl	$1, -8(%ebp)
.L149:
	movl	-8(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L146
.L148:
	movl	$1, -4(%ebp)
	jmp	.L146
.L147:
	movl	$0, -4(%ebp)
.L146:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	GuardedCommandList, .-GuardedCommandList
.globl GuardedCommand
	.type	GuardedCommand, @function
GuardedCommand:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	call	newItem
	movl	%eax, -4(%ebp)
	subl	$12, %esp
	pushl	-4(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L154
	movl	-4(%ebp), %eax
	cmpl	$2, 4(%eax)
	je	.L155
	subl	$12, %esp
	pushl	$27
	call	ERR_HANDLER
	addl	$16, %esp
.L155:
	movl	$0, -12(%ebp)
	subl	$8, %esp
	pushl	$1
	pushl	$24
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L156
	call	StatementPart
	testl	%eax, %eax
	je	.L156
	movl	$1, -12(%ebp)
.L156:
	movl	-12(%ebp), %eax
	movl	%eax, -8(%ebp)
	jmp	.L153
.L154:
	movl	$0, -8(%ebp)
.L153:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	GuardedCommand, .-GuardedCommand
.globl Expression
	.type	Expression, @function
Expression:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	8(%ebp)
	call	PrimaryExpression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L158
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	subl	$4, %esp
	pushl	$36
	pushl	8(%ebp)
	pushl	-4(%ebp)
	call	memcpy
	addl	$16, %esp
	call	PrimaryOperator
	testl	%eax, %eax
	je	.L159
	subl	$12, %esp
	pushl	-4(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L160
	movl	-4(%ebp), %eax
	cmpl	$1, 4(%eax)
	je	.L162
	movl	8(%ebp), %eax
	cmpl	$1, 4(%eax)
	je	.L162
	jmp	.L161
.L162:
	subl	$12, %esp
	pushl	$26
	call	ERR_HANDLER
	addl	$16, %esp
.L161:
	movl	-4(%ebp), %eax
	movl	8(%ebp), %edx
	movl	4(%eax), %eax
	cmpl	4(%edx), %eax
	je	.L163
	subl	$12, %esp
	pushl	$25
	call	ERR_HANDLER
	addl	$16, %esp
.L163:
	subl	$4, %esp
	pushl	-4(%ebp)
	pushl	8(%ebp)
	pushl	op
	call	op2
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	$2, 4(%eax)
	movl	$1, -8(%ebp)
	jmp	.L157
.L160:
	movl	$0, -8(%ebp)
	jmp	.L157
.L159:
	movl	$1, -8(%ebp)
	jmp	.L157
.L158:
	movl	$0, -8(%ebp)
.L157:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	Expression, .-Expression
.globl PrimaryOperator
	.type	PrimaryOperator, @function
PrimaryOperator:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$25
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L168
	movl	$8, op
	movl	$1, -4(%ebp)
	jmp	.L167
.L168:
	subl	$8, %esp
	pushl	$0
	pushl	$26
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L169
	movl	$9, op
	movl	$1, -4(%ebp)
	jmp	.L167
.L169:
	movl	$0, -4(%ebp)
.L167:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	PrimaryOperator, .-PrimaryOperator
.globl PrimaryExpression
	.type	PrimaryExpression, @function
PrimaryExpression:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	8(%ebp)
	call	SimpleExpression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L171
	call	RelationalOperator
	testl	%eax, %eax
	je	.L172
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	subl	$4, %esp
	pushl	$36
	pushl	8(%ebp)
	pushl	-4(%ebp)
	call	memcpy
	addl	$16, %esp
	subl	$12, %esp
	pushl	-4(%ebp)
	call	SimpleExpression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L173
	movl	-4(%ebp), %eax
	movl	8(%ebp), %edx
	movl	4(%eax), %eax
	cmpl	4(%edx), %eax
	je	.L174
	subl	$12, %esp
	pushl	$24
	call	ERR_HANDLER
	addl	$16, %esp
	movl	$0, -8(%ebp)
	jmp	.L170
.L174:
	movl	8(%ebp), %eax
	movl	$2, 4(%eax)
	subl	$4, %esp
	pushl	-4(%ebp)
	pushl	8(%ebp)
	pushl	$6
	call	op2
	addl	$16, %esp
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	op
	call	op1
	addl	$16, %esp
	movl	$1, -8(%ebp)
	jmp	.L170
.L173:
	movl	$0, -8(%ebp)
	jmp	.L170
.L172:
	movl	$1, -8(%ebp)
	jmp	.L170
.L171:
	movl	$0, -8(%ebp)
.L170:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	PrimaryExpression, .-PrimaryExpression
.globl RelationalOperator
	.type	RelationalOperator, @function
RelationalOperator:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$27
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L179
	movl	$20, op
	movl	$1, -4(%ebp)
	jmp	.L178
.L179:
	subl	$8, %esp
	pushl	$0
	pushl	$21
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L180
	movl	$18, op
	movl	$1, -4(%ebp)
	jmp	.L178
.L180:
	subl	$8, %esp
	pushl	$0
	pushl	$29
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L181
	movl	$22, op
	movl	$1, -4(%ebp)
	jmp	.L178
.L181:
	movl	$0, -4(%ebp)
.L178:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	RelationalOperator, .-RelationalOperator
	.section	.rodata
.LC6:
	.string	"ERROR ?\n"
.LC7:
	.string	"\t\t\t\t\tAdd\n"
.LC8:
	.string	"\t\t\tis Array %d\n"
.LC9:
	.string	"voor x = %d, y = %d\n"
.LC10:
	.string	"na x = %d, y = %d\n"
.LC11:
	.string	"isInt = %d, mode = %d\n"
.LC12:
	.string	"want to  minus array\n"
	.text
.globl SimpleExpression
	.type	SimpleExpression, @function
SimpleExpression:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$31
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L183
	subl	$12, %esp
	pushl	8(%ebp)
	call	Term
	addl	$16, %esp
	testl	%eax, %eax
	je	.L184
	movl	8(%ebp), %eax
	cmpl	$1, 4(%eax)
	je	.L185
	subl	$12, %esp
	pushl	$20
	call	ERR_HANDLER
	addl	$16, %esp
.L185:
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$39
	call	op1
	addl	$16, %esp
	jmp	.L188
.L184:
	subl	$12, %esp
	pushl	$.LC6
	call	printf
	addl	$16, %esp
	movl	$0, -16(%ebp)
	jmp	.L182
.L183:
	subl	$12, %esp
	pushl	8(%ebp)
	call	Term
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L188
	movl	$0, -16(%ebp)
	jmp	.L182
.L188:
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	subl	$4, %esp
	pushl	$36
	pushl	8(%ebp)
	pushl	-8(%ebp)
	call	memcpy
	addl	$16, %esp
	movl	-8(%ebp), %eax
	movl	$0, 24(%eax)
.L190:
	subl	$12, %esp
	leal	-12(%ebp), %eax
	pushl	%eax
	call	AddingOperator
	addl	$16, %esp
	testl	%eax, %eax
	je	.L191
	subl	$12, %esp
	pushl	$.LC7
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	-8(%ebp)
	call	Term
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L192
	movl	$0, -16(%ebp)
	jmp	.L182
.L192:
	subl	$8, %esp
	movl	-8(%ebp), %eax
	pushl	24(%eax)
	pushl	$.LC8
	call	printf
	addl	$16, %esp
	movl	8(%ebp), %eax
	cmpl	$1, 4(%eax)
	jne	.L194
	movl	-8(%ebp), %eax
	cmpl	$1, 4(%eax)
	jne	.L194
	jmp	.L193
.L194:
	subl	$12, %esp
	pushl	$21
	call	ERR_HANDLER
	addl	$16, %esp
.L193:
	subl	$8, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	call	printItems
	addl	$16, %esp
	movl	8(%ebp), %eax
	cmpl	$0, 20(%eax)
	je	.L196
	movl	-8(%ebp), %eax
	cmpl	$0, 20(%eax)
	jne	.L195
.L196:
	movl	8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L197
	movl	-8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L197
	cmpl	$0, -12(%ebp)
	je	.L198
	subl	$4, %esp
	movl	-8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC9
	call	printf
	addl	$16, %esp
	call	printReg
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$1
	call	op2
	addl	$16, %esp
	subl	$4, %esp
	movl	-8(%ebp), %eax
	pushl	12(%eax)
	movl	8(%ebp), %eax
	pushl	12(%eax)
	pushl	$.LC10
	call	printf
	addl	$16, %esp
	call	printReg
	jmp	.L200
.L198:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$2
	call	op2
	addl	$16, %esp
	jmp	.L200
.L197:
	cmpl	$0, -12(%ebp)
	je	.L201
	subl	$4, %esp
	movl	8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	20(%eax)
	pushl	$.LC11
	call	printf
	addl	$16, %esp
	movl	8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L202
	subl	$8, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	call	printItems
	addl	$16, %esp
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$26
	call	op2
	addl	$16, %esp
	jmp	.L200
.L202:
	subl	$4, %esp
	pushl	8(%ebp)
	pushl	-8(%ebp)
	pushl	$26
	call	op2
	addl	$16, %esp
	subl	$4, %esp
	pushl	$36
	pushl	-8(%ebp)
	pushl	8(%ebp)
	call	memcpy
	addl	$16, %esp
	jmp	.L200
.L201:
	movl	8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L205
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$27
	call	op2
	addl	$16, %esp
	jmp	.L200
.L205:
	movl	8(%ebp), %eax
	cmpl	$0, 24(%eax)
	je	.L207
	subl	$12, %esp
	pushl	$.LC12
	call	printf
	addl	$16, %esp
.L207:
	subl	$4, %esp
	pushl	8(%ebp)
	pushl	-8(%ebp)
	pushl	$27
	call	op2
	addl	$16, %esp
	subl	$4, %esp
	pushl	$36
	pushl	-8(%ebp)
	pushl	8(%ebp)
	call	memcpy
	addl	$16, %esp
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$39
	call	op1
	addl	$16, %esp
.L200:
	movl	8(%ebp), %eax
	movl	$0, 20(%eax)
	movl	-8(%ebp), %eax
	movl	$0, 20(%eax)
	movl	-8(%ebp), %eax
	movl	$0, 16(%eax)
	jmp	.L190
.L195:
	cmpl	$0, -12(%ebp)
	je	.L209
	movl	8(%ebp), %ecx
	movl	-8(%ebp), %edx
	movl	8(%ebp), %eax
	movl	16(%eax), %eax
	addl	16(%edx), %eax
	movl	%eax, 16(%ecx)
	jmp	.L190
.L209:
	movl	8(%ebp), %ebx
	movl	8(%ebp), %ecx
	movl	-8(%ebp), %eax
	movl	16(%eax), %edx
	movl	16(%ecx), %eax
	subl	%edx, %eax
	movl	%eax, 16(%ebx)
	jmp	.L190
.L191:
	movl	-8(%ebp), %eax
	movl	8(%ebp), %edx
	movl	16(%edx), %edx
	movl	%edx, 16(%eax)
	movl	$1, -16(%ebp)
.L182:
	movl	-16(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	SimpleExpression, .-SimpleExpression
.globl AddingOperator
	.type	AddingOperator, @function
AddingOperator:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$30
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L212
	movl	8(%ebp), %eax
	movl	$1, (%eax)
	movl	$1, -4(%ebp)
	jmp	.L211
.L212:
	subl	$8, %esp
	pushl	$0
	pushl	$31
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L213
	movl	8(%ebp), %eax
	movl	$0, (%eax)
	movl	$1, -4(%ebp)
	jmp	.L211
.L213:
	movl	$0, -4(%ebp)
.L211:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	AddingOperator, .-AddingOperator
	.section	.rodata
.LC13:
	.string	"voor x = %d r=%d, y = %d\n"
	.text
.globl Term
	.type	Term, @function
Term:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	subl	$12, %esp
	pushl	8(%ebp)
	call	Factor
	addl	$16, %esp
	testl	%eax, %eax
	je	.L215
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	subl	$4, %esp
	pushl	$36
	pushl	8(%ebp)
	pushl	-8(%ebp)
	call	memcpy
	addl	$16, %esp
	subl	$8, %esp
	movl	8(%ebp), %eax
	pushl	24(%eax)
	pushl	$.LC8
	call	printf
	addl	$16, %esp
.L216:
	subl	$12, %esp
	leal	-12(%ebp), %eax
	pushl	%eax
	call	MultiplyingOperator
	addl	$16, %esp
	testl	%eax, %eax
	je	.L217
	subl	$12, %esp
	pushl	-8(%ebp)
	call	Factor
	addl	$16, %esp
	testl	%eax, %eax
	je	.L218
	movl	8(%ebp), %eax
	cmpl	$1, 4(%eax)
	jne	.L220
	movl	-8(%ebp), %eax
	cmpl	$1, 4(%eax)
	jne	.L220
	jmp	.L219
.L220:
	subl	$12, %esp
	pushl	$22
	call	ERR_HANDLER
	addl	$16, %esp
.L219:
	movl	8(%ebp), %eax
	cmpl	$0, 20(%eax)
	je	.L222
	movl	-8(%ebp), %eax
	cmpl	$0, 20(%eax)
	jne	.L221
.L222:
	movl	8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L223
	movl	-8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L223
	movl	-12(%ebp), %eax
	movl	%eax, -20(%ebp)
	cmpl	$1, -20(%ebp)
	je	.L226
	cmpl	$1, -20(%ebp)
	jg	.L228
	cmpl	$0, -20(%ebp)
	je	.L225
	jmp	.L229
.L228:
	cmpl	$2, -20(%ebp)
	je	.L227
	jmp	.L229
.L225:
	subl	$4, %esp
	movl	-8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC9
	call	printf
	addl	$16, %esp
	call	printReg
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$3
	call	op2
	addl	$16, %esp
	call	printReg
	subl	$4, %esp
	movl	-8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC10
	call	printf
	addl	$16, %esp
	jmp	.L229
.L226:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$4
	call	op2
	addl	$16, %esp
	jmp	.L229
.L227:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$5
	call	op2
	addl	$16, %esp
	jmp	.L229
.L223:
	movl	8(%ebp), %eax
	cmpl	$2, (%eax)
	je	.L230
	movl	-12(%ebp), %ebx
	movl	%ebx, -24(%ebp)
	cmpl	$1, -24(%ebp)
	je	.L233
	cmpl	$1, -24(%ebp)
	jg	.L235
	cmpl	$0, -24(%ebp)
	je	.L232
	jmp	.L229
.L235:
	cmpl	$2, -24(%ebp)
	je	.L234
	jmp	.L229
.L232:
	movl	-8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	12(%eax)
	movl	8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC13
	call	printf
	addl	$16, %esp
	call	printReg
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$28
	call	op2
	addl	$16, %esp
	subl	$4, %esp
	movl	-8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC9
	call	printf
	addl	$16, %esp
	call	printReg
	jmp	.L229
.L233:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$29
	call	op2
	addl	$16, %esp
	jmp	.L229
.L234:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$30
	call	op2
	addl	$16, %esp
	jmp	.L229
.L230:
	movl	-12(%ebp), %eax
	movl	%eax, -28(%ebp)
	cmpl	$1, -28(%ebp)
	je	.L239
	cmpl	$1, -28(%ebp)
	jg	.L241
	cmpl	$0, -28(%ebp)
	je	.L238
	jmp	.L229
.L241:
	cmpl	$2, -28(%ebp)
	je	.L240
	jmp	.L229
.L238:
	subl	$4, %esp
	movl	-8(%ebp), %eax
	pushl	(%eax)
	movl	8(%ebp), %eax
	pushl	(%eax)
	pushl	$.LC9
	call	printf
	addl	$16, %esp
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$3
	call	op2
	addl	$16, %esp
	jmp	.L229
.L239:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$4
	call	op2
	addl	$16, %esp
	jmp	.L229
.L240:
	subl	$4, %esp
	pushl	-8(%ebp)
	pushl	8(%ebp)
	pushl	$5
	call	op2
	addl	$16, %esp
.L229:
	movl	8(%ebp), %eax
	movl	$0, 20(%eax)
	movl	-8(%ebp), %eax
	movl	$0, 20(%eax)
	movl	8(%ebp), %eax
	movl	$0, 16(%eax)
	jmp	.L242
.L221:
	cmpl	$0, -12(%ebp)
	jne	.L243
	movl	8(%ebp), %ecx
	movl	-8(%ebp), %eax
	movl	8(%ebp), %edx
	movl	16(%eax), %eax
	imull	16(%edx), %eax
	movl	%eax, 16(%ecx)
.L243:
	cmpl	$1, -12(%ebp)
	jne	.L244
	movl	-8(%ebp), %eax
	cmpl	$0, 16(%eax)
	jne	.L245
	subl	$12, %esp
	pushl	$29
	call	ERR_HANDLER
	addl	$16, %esp
.L245:
	movl	8(%ebp), %ecx
	movl	8(%ebp), %edx
	movl	-8(%ebp), %ebx
	movl	%ebx, -32(%ebp)
	movl	16(%edx), %edx
	movl	-32(%ebp), %ebx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	16(%ebx)
	movl	%eax, -32(%ebp)
	movl	-32(%ebp), %eax
	movl	%eax, 16(%ecx)
.L244:
	cmpl	$2, -12(%ebp)
	jne	.L242
	movl	8(%ebp), %eax
	cmpl	$0, 16(%eax)
	jne	.L247
	subl	$12, %esp
	pushl	$29
	call	ERR_HANDLER
	addl	$16, %esp
.L247:
	movl	8(%ebp), %ecx
	movl	8(%ebp), %edx
	movl	-8(%ebp), %ebx
	movl	%ebx, -32(%ebp)
	movl	16(%edx), %edx
	movl	-32(%ebp), %ebx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	16(%ebx)
	movl	%edx, 16(%ecx)
.L242:
	movl	-8(%ebp), %eax
	movl	8(%ebp), %edx
	movl	16(%edx), %edx
	movl	%edx, 16(%eax)
	jmp	.L216
.L218:
	movl	$0, -16(%ebp)
	jmp	.L214
.L217:
	movl	$1, -16(%ebp)
	jmp	.L214
.L215:
	movl	$0, -16(%ebp)
.L214:
	movl	-16(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	Term, .-Term
.globl MultiplyingOperator
	.type	MultiplyingOperator, @function
MultiplyingOperator:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	$0
	pushl	$33
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L251
	movl	8(%ebp), %eax
	movl	$0, (%eax)
	movl	$1, -4(%ebp)
	jmp	.L250
.L251:
	subl	$8, %esp
	pushl	$0
	pushl	$34
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L252
	movl	8(%ebp), %eax
	movl	$1, (%eax)
	movl	$1, -4(%ebp)
	jmp	.L250
.L252:
	subl	$8, %esp
	pushl	$0
	pushl	$35
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L253
	movl	8(%ebp), %eax
	movl	$2, (%eax)
	movl	$1, -4(%ebp)
	jmp	.L250
.L253:
	movl	$0, -4(%ebp)
.L250:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	MultiplyingOperator, .-MultiplyingOperator
.globl Factor
	.type	Factor, @function
Factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$1, -4(%ebp)
	subl	$8, %esp
	leal	-4(%ebp), %eax
	pushl	%eax
	pushl	8(%ebp)
	call	Constant
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L256
	subl	$8, %esp
	pushl	-4(%ebp)
	pushl	8(%ebp)
	call	VariableAccess
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L256
	subl	$8, %esp
	pushl	$0
	pushl	$37
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L255
	subl	$12, %esp
	pushl	8(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L255
	subl	$8, %esp
	pushl	$1
	pushl	$38
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L256
	jmp	.L255
.L256:
	movl	$1, -8(%ebp)
	jmp	.L254
.L255:
	subl	$8, %esp
	pushl	$0
	pushl	$36
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L257
	subl	$12, %esp
	pushl	8(%ebp)
	call	Factor
	addl	$16, %esp
	testl	%eax, %eax
	je	.L257
	movl	8(%ebp), %eax
	cmpl	$2, 4(%eax)
	je	.L258
	subl	$12, %esp
	pushl	$23
	call	ERR_HANDLER
	addl	$16, %esp
.L258:
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$44
	call	op1
	addl	$16, %esp
	movl	$1, -8(%ebp)
	jmp	.L254
.L257:
	movl	$0, -8(%ebp)
.L254:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	Factor, .-Factor
.globl VariableAccess
	.type	VariableAccess, @function
VariableAccess:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	cmpl	$0, 12(%ebp)
	jne	.L261
	subl	$8, %esp
	pushl	$0
	pushl	8(%ebp)
	call	VariableName
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L261
	jmp	.L260
.L261:
	subl	$8, %esp
	pushl	$def_name
	pushl	$proc
	call	lookup_var
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	cmpl	$0, -8(%ebp)
	jne	.L262
	subl	$12, %esp
	pushl	$12
	call	ERR_HANDLER
	addl	$16, %esp
	movl	$0, -12(%ebp)
	jmp	.L259
.L262:
	movl	8(%ebp), %eax
	movl	$0, 16(%eax)
	movl	8(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	44(%eax), %eax
	movl	%eax, 8(%edx)
	movl	-8(%ebp), %eax
	movl	36(%eax), %eax
	movl	%eax, type
	movl	8(%ebp), %eax
	movl	$1, (%eax)
	movl	-8(%ebp), %eax
	cmpl	$0, 40(%eax)
	je	.L264
	movl	-8(%ebp), %eax
	cmpl	$6, 36(%eax)
	je	.L266
	movl	-8(%ebp), %eax
	cmpl	$5, 36(%eax)
	je	.L266
	jmp	.L265
.L266:
	subl	$12, %esp
	pushl	$15
	call	ERR_HANDLER
	addl	$16, %esp
	movl	$1, -12(%ebp)
	jmp	.L259
.L265:
	movl	-8(%ebp), %eax
	movl	48(%eax), %eax
	cmpl	level, %eax
	jne	.L268
	movl	8(%ebp), %ebx
	call	resetLevel
	movl	%eax, 32(%ebx)
	jmp	.L269
.L268:
	movl	-8(%ebp), %eax
	cmpl	$1, 48(%eax)
	jne	.L270
	movl	8(%ebp), %ebx
	call	rootLevel
	movl	%eax, 32(%ebx)
	jmp	.L269
.L270:
	movl	8(%ebp), %ebx
	call	changeLevel
	movl	%eax, 32(%ebx)
.L269:
	movl	8(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	48(%eax), %eax
	movl	%eax, 28(%edx)
	subl	$12, %esp
	pushl	8(%ebp)
	call	IndexedSelector
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	jmp	.L259
.L264:
	movl	8(%ebp), %edx
	movl	type, %eax
	movl	%eax, 4(%edx)
	movl	-8(%ebp), %eax
	movl	48(%eax), %eax
	cmpl	level, %eax
	jne	.L273
	movl	8(%ebp), %ebx
	call	resetLevel
	movl	%eax, 32(%ebx)
	jmp	.L274
.L273:
	movl	-8(%ebp), %eax
	cmpl	$1, 48(%eax)
	jne	.L275
	movl	8(%ebp), %ebx
	call	rootLevel
	movl	%eax, 32(%ebx)
	jmp	.L274
.L275:
	movl	8(%ebp), %ebx
	call	changeLevel
	movl	%eax, 32(%ebx)
.L274:
	movl	8(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	48(%edx), %edx
	movl	%edx, 28(%eax)
	movl	$1, -12(%ebp)
	jmp	.L259
.L260:
	movl	$0, -12(%ebp)
.L259:
	movl	-12(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	VariableAccess, .-VariableAccess
	.section	.rodata
.LC14:
	.string	"alloc new regs\n"
	.text
.globl IndexedSelector
	.type	IndexedSelector, @function
IndexedSelector:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$36, %esp
	subl	$8, %esp
	pushl	$1
	pushl	$22
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L279
	subl	$8, %esp
	pushl	$def_name
	pushl	$proc
	call	lookup_var
	addl	$16, %esp
	movl	%eax, -8(%ebp)
	cmpl	$0, -8(%ebp)
	je	.L281
	movl	-8(%ebp), %eax
	cmpl	$3, 36(%eax)
	je	.L282
	movl	-8(%ebp), %eax
	cmpl	$4, 36(%eax)
	jne	.L281
.L282:
	movl	-8(%ebp), %eax
	cmpl	$0, 40(%eax)
	jne	.L280
.L281:
	subl	$12, %esp
	pushl	$13
	call	ERR_HANDLER
	addl	$16, %esp
.L280:
	call	newItem
	movl	%eax, -12(%ebp)
	subl	$12, %esp
	pushl	-12(%ebp)
	call	Expression
	addl	$16, %esp
	testl	%eax, %eax
	je	.L283
	subl	$8, %esp
	pushl	$1
	pushl	$23
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L283
	movl	-12(%ebp), %eax
	cmpl	$1, 4(%eax)
	je	.L284
	subl	$12, %esp
	pushl	$19
	call	ERR_HANDLER
	addl	$16, %esp
.L284:
	movl	-12(%ebp), %eax
	cmpl	$0, 16(%eax)
	js	.L286
	movl	-12(%ebp), %eax
	movl	-8(%ebp), %edx
	movl	16(%eax), %eax
	cmpl	40(%edx), %eax
	jge	.L286
	jmp	.L285
.L286:
	subl	$12, %esp
	pushl	$28
	call	ERR_HANDLER
	addl	$16, %esp
.L285:
	movl	8(%ebp), %eax
	movl	$1, 24(%eax)
	movl	-8(%ebp), %eax
	cmpl	$3, 36(%eax)
	jne	.L287
	movl	8(%ebp), %eax
	movl	$1, 4(%eax)
	jmp	.L288
.L287:
	movl	8(%ebp), %eax
	movl	$2, 4(%eax)
.L288:
	call	newItem
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
	movl	$2, (%eax)
	movl	-16(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	40(%eax), %eax
	movl	%eax, 16(%edx)
	call	newItem
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	$2, (%eax)
	movl	-20(%ebp), %edx
	movl	8(%ebp), %eax
	movl	8(%eax), %eax
	movl	%eax, 16(%edx)
	movl	-20(%ebp), %edx
	movl	8(%ebp), %eax
	movl	32(%eax), %eax
	movl	%eax, 32(%edx)
	call	newItem
	movl	%eax, -24(%ebp)
	movl	-24(%ebp), %eax
	movl	$2, (%eax)
	movl	-24(%ebp), %eax
	movl	$4, 16(%eax)
	subl	$12, %esp
	pushl	$36
	call	malloc
	addl	$16, %esp
	movl	%eax, -28(%ebp)
	subl	$4, %esp
	pushl	$36
	pushl	-12(%ebp)
	pushl	-28(%ebp)
	call	memcpy
	addl	$16, %esp
	movl	-28(%ebp), %eax
	cmpl	$0, 24(%eax)
	jne	.L289
	subl	$12, %esp
	pushl	-28(%ebp)
	call	load
	addl	$16, %esp
	jmp	.L290
.L289:
	subl	$12, %esp
	pushl	$.LC14
	call	printf
	addl	$16, %esp
	subl	$12, %esp
	pushl	-28(%ebp)
	call	printItem
	addl	$16, %esp
	movl	-28(%ebp), %ebx
	call	getReg
	movl	%eax, 12(%ebx)
	movl	-12(%ebp), %eax
	pushl	12(%eax)
	pushl	$0
	movl	-28(%ebp), %eax
	pushl	12(%eax)
	pushl	$1
	call	Put
	addl	$16, %esp
.L290:
	subl	$4, %esp
	pushl	-16(%ebp)
	pushl	-28(%ebp)
	pushl	$32
	call	op2
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	-28(%ebp)
	call	closeItem
	addl	$16, %esp
	movl	-12(%ebp), %eax
	cmpl	$0, 24(%eax)
	je	.L291
	pushl	$0
	movl	-12(%ebp), %eax
	pushl	12(%eax)
	movl	-12(%ebp), %eax
	pushl	12(%eax)
	pushl	$12
	call	Put
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	$0, 24(%eax)
	jmp	.L292
.L291:
	subl	$12, %esp
	pushl	-12(%ebp)
	call	load
	addl	$16, %esp
.L292:
	subl	$12, %esp
	pushl	-20(%ebp)
	call	load
	addl	$16, %esp
	subl	$4, %esp
	pushl	-24(%ebp)
	pushl	-12(%ebp)
	pushl	$28
	call	op2
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	-24(%ebp)
	call	closeItem
	addl	$16, %esp
	movl	-12(%ebp), %eax
	pushl	12(%eax)
	movl	-20(%ebp), %eax
	pushl	12(%eax)
	movl	-20(%ebp), %eax
	pushl	12(%eax)
	pushl	$1
	call	Put
	addl	$16, %esp
	subl	$12, %esp
	pushl	8(%ebp)
	call	load
	addl	$16, %esp
	movl	8(%ebp), %eax
	pushl	32(%eax)
	movl	-20(%ebp), %eax
	pushl	12(%eax)
	movl	8(%ebp), %eax
	pushl	12(%eax)
	pushl	$1
	call	Put
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	-20(%ebp)
	call	closeItem
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	-12(%ebp)
	call	closeItem
	addl	$16, %esp
	subl	$8, %esp
	pushl	$1
	pushl	-16(%ebp)
	call	closeItem
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	$3, (%eax)
	movl	$1, -32(%ebp)
	jmp	.L278
.L283:
	movl	$0, -32(%ebp)
	jmp	.L278
.L279:
	movl	$0, -32(%ebp)
.L278:
	movl	-32(%ebp), %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	IndexedSelector, .-IndexedSelector
	.section	.rodata
.LC15:
	.string	"in constant. isA %d\n"
	.text
.globl Constant
	.type	Constant, @function
Constant:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	8(%ebp)
	call	Numeral
	addl	$16, %esp
	testl	%eax, %eax
	je	.L295
	movl	8(%ebp), %eax
	movl	$1, 4(%eax)
	movl	8(%ebp), %edx
	movl	val, %eax
	movl	%eax, 16(%edx)
	movl	$5, type
	movl	8(%ebp), %eax
	movl	$2, (%eax)
	subl	$8, %esp
	movl	8(%ebp), %eax
	pushl	24(%eax)
	pushl	$.LC15
	call	printf
	addl	$16, %esp
	movl	$1, -8(%ebp)
	jmp	.L294
.L295:
	subl	$8, %esp
	pushl	$0
	pushl	8(%ebp)
	call	ConstantName
	addl	$16, %esp
	testl	%eax, %eax
	je	.L296
	subl	$8, %esp
	pushl	$const_name
	pushl	$proc
	call	lookup_var
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	cmpl	$0, -4(%ebp)
	je	.L299
	movl	-4(%ebp), %eax
	cmpl	$6, 36(%eax)
	je	.L298
	movl	-4(%ebp), %eax
	cmpl	$5, 36(%eax)
	jne	.L299
	jmp	.L298
.L299:
	movl	12(%ebp), %eax
	cmpl	$0, (%eax)
	jne	.L300
	subl	$12, %esp
	pushl	$14
	call	ERR_HANDLER
	addl	$16, %esp
.L300:
	subl	$4, %esp
	pushl	$const_name
	pushl	$.LC0
	pushl	$def_name
	call	sprintf
	addl	$16, %esp
	movl	$0, -8(%ebp)
	jmp	.L294
.L298:
	movl	8(%ebp), %eax
	movl	$2, (%eax)
	movl	8(%ebp), %edx
	movl	-4(%ebp), %eax
	movl	40(%eax), %eax
	movl	%eax, 16(%edx)
	movl	8(%ebp), %edx
	movl	-4(%ebp), %eax
	movl	44(%eax), %eax
	movl	%eax, 8(%edx)
	movl	-4(%ebp), %eax
	cmpl	$5, 36(%eax)
	jne	.L302
	movl	$5, type
	movl	8(%ebp), %eax
	movl	$1, 4(%eax)
	jmp	.L303
.L302:
	movl	$6, type
	movl	8(%ebp), %eax
	movl	$2, 4(%eax)
.L303:
	movl	-4(%ebp), %eax
	movl	40(%eax), %eax
	movl	%eax, val
	movl	$1, -8(%ebp)
	jmp	.L294
.L296:
	movl	12(%ebp), %eax
	movl	$0, (%eax)
	movl	$0, -8(%ebp)
.L294:
	movl	-8(%ebp), %eax
	leave
	ret
	.size	Constant, .-Constant
.globl Numeral
	.type	Numeral, @function
Numeral:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	token, %eax
	movl	36(%eax), %eax
	movl	%eax, val
	subl	$8, %esp
	pushl	$0
	pushl	$17
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L305
	movl	8(%ebp), %eax
	movl	$1, 20(%eax)
	movl	8(%ebp), %eax
	movl	$2, (%eax)
	movl	$1, -4(%ebp)
	jmp	.L304
.L305:
	movl	$0, -4(%ebp)
.L304:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	Numeral, .-Numeral
.globl Name
	.type	Name, @function
Name:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	$16
	call	expect
	addl	$16, %esp
	testl	%eax, %eax
	je	.L307
	cmpl	$0, 8(%ebp)
	je	.L308
	movl	8(%ebp), %eax
	movl	$0, 20(%eax)
	movl	8(%ebp), %eax
	movl	$1, (%eax)
.L308:
	movl	$1, -4(%ebp)
	jmp	.L306
.L307:
	movl	$0, -4(%ebp)
.L306:
	movl	-4(%ebp), %eax
	leave
	ret
	.size	Name, .-Name
.globl ConstantName
	.type	ConstantName, @function
ConstantName:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$4, %esp
	pushl	token
	pushl	$.LC0
	pushl	$const_name
	call	sprintf
	addl	$16, %esp
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	Name
	addl	$16, %esp
	leave
	ret
	.size	ConstantName, .-ConstantName
.globl VariableName
	.type	VariableName, @function
VariableName:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$4, %esp
	pushl	token
	pushl	$.LC0
	pushl	$def_name
	call	sprintf
	addl	$16, %esp
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	Name
	addl	$16, %esp
	leave
	ret
	.size	VariableName, .-VariableName
.globl ProcedureName
	.type	ProcedureName, @function
ProcedureName:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$4, %esp
	pushl	token
	pushl	$.LC0
	pushl	$def_name
	call	sprintf
	addl	$16, %esp
	subl	$8, %esp
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	Name
	addl	$16, %esp
	leave
	ret
	.size	ProcedureName, .-ProcedureName
	.local	token
	.comm	token,4,4
	.local	last_expected
	.comm	last_expected,33,32
	.local	last_found
	.comm	last_found,33,32
	.local	proc
	.comm	proc,32,32
	.local	def_name
	.comm	def_name,33,32
	.local	const_name
	.comm	const_name,33,32
	.local	call_name
	.comm	call_name,33,32
	.local	need_priority
	.comm	need_priority,4,4
	.local	level
	.comm	level,4,4
	.local	val
	.comm	val,4,4
	.local	type
	.comm	type,4,4
	.comm	sym_type,4,4
	.comm	op,4,4
	.comm	list,4,4
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.1 (Mandrakelinux 10.1 3.4.1-4mdk)"
