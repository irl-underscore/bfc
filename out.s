.section .bss
	.align 64
	.lcomm, tape 1050
.section .text
.global _start
_start:
	lea tape(%rip), %rbx
	movq $60, %rax
	xorq %rdi, %rdi
	syscall
