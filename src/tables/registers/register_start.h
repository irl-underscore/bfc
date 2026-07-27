#define TAR(arc, reg_str) _MATCH_##arc(reg_str)
#ifndef _MATCH_X86_LINUX
#define _MATCH_X86_LINUX(reg_str)
#endif /* _MATCH_X86_LINUX */
#ifndef _MATCH_X86_64_LINUX
#define _MATCH_X86_64_LINUX(reg_str)
#endif /* _MATCH_X86_64_LINUX */
#ifndef _MATCH_ARM_LINUX
#define _MATCH_ARM_LINUX(reg_str)
#endif /* _MATCH_ARM_LINUX */
#ifndef _MATCH_ARM64_LINUX
#define _MATCH_ARM64_LINUX(reg_str)
#endif /* _MATCH_ARM64_LINUX */
#define _MATCH_ALL(reg_str) = reg_str

#define X(reg, matches) [reg] matches,
