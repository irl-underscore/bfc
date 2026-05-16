#ifndef X86_64_LINUX_H
#define X86_64_LINUX_H

#define TAR(arc, num) _MATCH_##arc(num)
#define _MATCH_X86(num)
#define _MATCH_X86_64(num) = num
#define _MATCH_ARM(num)
#define _MATCH_ARM64(num)

#define X(bf_code, matches) [bf_code] matches + 1,

static int64_t x86_64_linux_syscall_table[] = {
    #include "syscall_map.h"
};

#undef X
#undef _MATCH_X86
#undef _MATCH_X86_64
#undef _MATCH_ARM
#undef _MATCH_ARM64

#endif
