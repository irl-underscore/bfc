#include "syscall.h"

#include "syscall/x86_linux.h"
#include "syscall/x86_64_linux.h"

#include <stdio.h>

void emit_syscall(SyscallArc target, string *dst, bf_call bf_call, char *arg0, char *arg1, char *arg2)
{
    if (!dst) return;

    switch (target)
    {
        case ARC_X86_LINUX: {
            int syscall = x86_linux_syscall_table[bf_call] - 1;
            if (syscall < 0)
            {
                // error handling
                return;
            }

            string_append_format(dst, "\tmovl $%d, %%eax\n", syscall);
            if (arg0) string_append_format(dst, "\tmovl %s, %%ebx\n", arg0);
            if (arg1) string_append_format(dst, "\tmovl %s, %%ecx\n", arg1);
            if (arg2) string_append_format(dst, "\tmovl %s, %%edx\n", arg2);
            string_append_string(dst, "\tint $0x80\n");
            printf("Data: %s\n", string_get_raw(dst));
            break;
        }
        case ARC_X86_64_LINUX: {
            int syscall = x86_64_linux_syscall_table[bf_call] - 1;
            printf("%d\n", syscall);
            if (syscall < 0)
            {
                // error handling
                return;
            }

            string_append_format(dst, "\tmovq $%d, %%rax\n", syscall);
            if (arg0) string_append_format(dst, "\tmovq %s, %%rdi\n", arg0);
            if (arg1) string_append_format(dst, "\tmovq %s, %%rsi\n", arg1);
            if (arg2) string_append_format(dst, "\tmovq %s, %%rdx\n", arg2);
            string_append_string(dst, "\tsyscall\n");
            printf("Data: %s\n", string_get_raw(dst));
            break;
        }
    }
}
