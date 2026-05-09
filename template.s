.section .bss
    .align 64
    .lcomm data, 1024

.section .text
.global _start
_start:
    lea  data(%rip), %rbx
$(code)
    movq $60, %rax
    xorq %rdi, %rdi
    syscall

# Funfact: If you compile thís code with the GAS assembler and remove the brainfuck compilers $() calls this programme is ~5KB in size :)
