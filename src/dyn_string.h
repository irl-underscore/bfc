#ifndef DYN_STRING_H
#define DYN_STRING_H
#include "base_types.h"
#include "arena.h"

typedef struct DynString_s
{
    Size capacity;
    Size used;
    Arena *pArena;
    Char *pData;
} DynString;

Bool dynStringCreate(const Size intialCapacity, Arena *pArena, DynString *pDynString);
Bool dynStringAppend(DynString *pDynString, const Char *pStr);
Bool dynStringAppendFmt(DynString *pDynString, const Char *pFmt, ...);

#endif /* DYN_STRING_H */
