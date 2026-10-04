#ifndef BF_DEFS_H
#define BF_DEFS_H

#if defined (__GNUC__) || defined (__clang__)
    #define UNLIKELY(cond) __builtin_expect(!!(cond), 0)
    #define LIKELY(cond) __builtin_expect(!!(cond), 1)
#else
    #define UNUNLIKELY(cond) (cond)
    #define LIKELYLIKELY(cond) (cond)
#endif /* __GNUC__ || __clang__ */

#endif /* BF_DEFS_H */
