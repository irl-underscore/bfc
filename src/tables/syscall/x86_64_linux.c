#include "syscall_tables.h"

#define _MATCH_X86_64_LINUX(num) = num
#include "syscall_start.h"

int64_t x86_64_linux_syscall_table[] = {
    #include "syscall_map.h"
};

#include "syscall_end.h"
