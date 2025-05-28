	.file	"strerror_tls.c"
	.text
	.section	.tbss,"awT",@nobits
	.type	buf, @object
	.size	buf, 256
buf:
	.zero	256
	.section	.rodata
.LC0:
	.string	"Unknown error %d"
	.text
	.globl	strerror
	.type	strerror, @function
strerror:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$0, -4(%rbp)
	js	.L2
	movl	_sys_nerr(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jge	.L2
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	leaq	_sys_errlist(%rip), %rax
	movq	(%rdx,%rax), %rax
	testq	%rax, %rax
	jne	.L3
.L2:
	movl	-4(%rbp), %eax
	movq	%fs:0, %rdx
	leaq	buf@tpoff(%rdx), %rdi
	movl	%eax, %ecx
	leaq	.LC0(%rip), %rdx
	movl	$256, %esi
	movl	$0, %eax
	call	snprintf@PLT
	jmp	.L4
.L3:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	leaq	_sys_errlist(%rip), %rax
	movq	(%rdx,%rax), %rax
	movq	%fs:0, %rdx
	leaq	buf@tpoff(%rdx), %rcx
	movl	$255, %edx
	movq	%rax, %rsi
	movq	%rcx, %rdi
	call	strncpy@PLT
	movb	$0, %fs:255+buf@tpoff
.L4:
	movq	%fs:0, %rax
	addq	$buf@tpoff, %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	strerror, .-strerror
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
