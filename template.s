.section .bss
    .align 64
    .lcomm data, 1024
.section .text
.global _start
_start:
    lea  data(%rip), %rbx
$(code)
