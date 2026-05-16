#ifndef SYSCALL_H
#define SYSCALL_H
#include "bf_call.h"
#include "string.h"

typedef enum
{
    ARC_X86_LINUX,
    ARC_X86_64_LINUX
} SyscallArc;

void emit_syscall(SyscallArc target, string *dst, bf_call bf_call, char *arg0, char *arg1, char *arg2);

#endif
