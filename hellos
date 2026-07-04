.section .bss
	.align 64
	.lcomm tape, 1050
	.lcomm print_buf, 20
.section .text
.global _start
_start:
	xorq %rcx, %rcx
	lea tape(%rip), %rbx
	lea print_buf(%rip), %rdx
	addb $10, (%rbx)
.L1_start:
	cmpb $0, (%rbx)
	je .L1_end
	incq %rbx
	addb $7, (%rbx)
	incq %rbx
	addb $10, (%rbx)
	incq %rbx
	addb $3, (%rbx)
	incq %rbx
	incb (%rbx)
	subq $4, %rbx
	decb (%rbx)
	jmp .L1_start
.L1_end:
	incq %rbx
	addb $2, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	incq %rbx
	incb (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	addb $7, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	addb $3, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	incq %rbx
	addb $2, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	subq $2, %rbx
	addb $15, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	incq %rbx
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	addb $3, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	subb $6, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	subb $8, (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	incq %rbx
	incb (%rbx)
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	incq %rbx
	movb (%rbx), %al
	movb %al, (%rdx, %rcx, 1)
	incq %rcx
	movq $1, %rax
	movq $1, %rdi
	movq %rdx, %rsi
	movq $13, %rdx
	syscall
	xorq %rcx, %rcx
	movq $60, %rax
	xorq %rdi, %rdi
	syscall
