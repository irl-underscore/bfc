#ifndef SYSCALL_TABLES_H
#define SYSCALL_TABLES_H
#include <stdint.h>

typedef enum
{
    BF_CALL_EXIT = 0,
    BF_CALL_READ = 1,
    BF_CALL_WRITE = 2
} bf_call;

extern int64_t x86_64_linux_syscall_table[];
extern int64_t x86_linux_syscall_table[];

#endif /* SYSCALL_TABLES_H */
