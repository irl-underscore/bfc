.section .bss
    .align 64
    .lcomm data, 1024
.section .text
.global _start
_start:
    lea  data(%rip), %rbx
.L1_:
	cmpb (%rbx), $0
	jne .L1_
	addb $10, (%rbx)
.L2_:
	addq $1, %rbx
	addb $7, (%rbx)
	addq $1, %rbx
	addb $10, (%rbx)
	addq $1, %rbx
	addb $3, (%rbx)
	addq $1, %rbx
	addb $7, (%rbx)
	subq $1, %rbx
	subq $1, %rbx
	subq $1, %rbx
	subq $1, %rbx
	decb (%rbx)
	cmpb (%rbx), $0
	jne .L2_
	addq $1, %rbx
	addb $2, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addq $1, %rbx
	incb (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addb $7, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addb $3, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addq $1, %rbx
	addb $2, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	subq $1, %rbx
	subq $1, %rbx
	addb $8, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addq $1, %rbx
	addq $1, %rbx
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addb $3, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	subb $6, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	subb $8, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	addq $1, %rbx
	incb (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
	movq $60, %rax
	xorq %rdi, %rdi
	syscall
