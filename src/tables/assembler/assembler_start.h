#define TAR(arc, str) _MATCH_##arc(str)
#ifndef _MATCH_X86_LINUX
#define _MATCH_X86_LINUX(str)
#endif /* _MATCH_X86_LINUX */
#ifndef _MATCH_X86_64_LINUX
#define _MATCH_X86_64_LINUX(str)
#endif /* _MATCH_X86_64_LINUX */
#ifndef _MATCH_ARM_LINUX
#define _MATCH_ARM_LINUX(str)
#endif /* _MATCH_ARM_LINUX */
#ifndef _MATCH_ARM64_LINUX
#define _MATCH_ARM64_LINUX(str)
#endif /* _MATCH_ARM64_LINUX */
#define _MATCH_ALL(str) = str

#define X(type, matches) [type] matches,
