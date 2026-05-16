.section .bss
    .align 64
    .lcomm data, 1024

.section .text
.global _start
_start:
    lea  data(%rip), %rbx
	movq $0, %rax
	movq $0, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
_L1_start:
	decb (%rbx)
	cmpb $0, (%rbx)
	jne _L1_start
_L1_end:
	addb $4, (%rbx)
	movq $1, %rax
	movq $1, %rdi
	movq %rbx, %rsi
	movq $1, %rdx
	syscall
    movq $60, %rax
    xorq %rdi, %rdi
    syscall

