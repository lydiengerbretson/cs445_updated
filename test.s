	.file	"test.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movabsq	$4596373779694328218, %rax
	movq	%rax, -32(%rbp)
	movabsq	$4591870180066957722, %rax
	movq	%rax, -24(%rbp)
	movabsq	$4607182418800017408, %rax
	movq	%rax, -16(%rbp)
	movabsq	$4613937818241073152, %rax
	movq	%rax, -8(%rbp)
	movl	$5, -40(%rbp)
	movl	$10, -36(%rbp)
	movsd	-16(%rbp), %xmm0
	addsd	-8(%rbp), %xmm0
	movsd	%xmm0, -32(%rbp)
	movl	-36(%rbp), %eax
	movl	-40(%rbp), %edx
	addl	%edx, %eax
	cvtsi2sd	%eax, %xmm0
	movsd	%xmm0, -24(%rbp)
	movb	$97, -41(%rbp)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
