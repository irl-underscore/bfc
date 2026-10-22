#ifndef DEFS_H
#define DEFS_H

#if defined (__GNUC__) || defined (__clang__)
    #define UNLIKELY(cond) __builtin_expect(!!(cond), 0)
    #define LIKELY(cond) __builtin_expect(!!(cond), 1)
#else
    #define UNUNLIKELY(cond) (cond)
    #define LIKELYLIKELY(cond) (cond)
#endif /* __GNUC__ || __clang__ */

#define NATURAL_ALIGN(num) (num & (-num))
#define ALIGN_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

#define MB (1024 * 1024)

#if defined (__linux__)
    #define OS_LINUX 1
#elif defined (__APPLE__)
    #define OS_APPLE 1
#elif defined (_WIN32)
    #define OS_WINDOWS 1
#endif /* os */

#if !defined (_TEST)
    #define main main
#else
    #define main mainFunc
#endif /* _TEST */

#endif /* DEFS_H */
