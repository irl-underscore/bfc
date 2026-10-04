#ifndef BF_BASE_TYPES_H
#define BF_BASE_TYPES_H

/* Basic types */
typedef __UINT8_TYPE__ U8;
typedef __INT8_TYPE__ I8;
typedef __UINT16_TYPE__ U16;
typedef __INT16_TYPE__ I16;
typedef __UINT32_TYPE__ U32;
typedef __INT32_TYPE__ I32;
typedef __UINT64_TYPE__ U64;
typedef __INT64_TYPE__ I64;
typedef unsigned __int128 U128;
typedef __int128 I128;

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) // 64 bit
    typedef __UINT64_TYPE__ UPtr;
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) // 32 bit
    typedef __UINT32_TYPE__ UPtr;
#else // default
    typedef __UINT64_TYPE__ UPtr;
#endif /* arc */

typedef unsigned long Size;
typedef long SSize;

typedef float F32;
typedef double F64;

typedef unsigned char Byte;
typedef char Char;
typedef unsigned char UChar;

typedef enum Bool_e : U8
{
    FALSE = 0,
    TRUE = 1
} Bool;

#define U8_MAX 0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFF

#endif /* BF_BASE_TYPES_H */
