#ifndef DYN_STRING_H
#define DYN_STRING_H
#include "base_types.h"

typedef struct DynString_s
{
    Size capacity;
    Size used;
    Char *pData;
} DynString;

Bool dynStringCreate(const Size intialCapacity, DynString *pDynString);
Bool dynStringAppend(DynString *pDynString, const Char *pStr, const Size strLen);
Bool dynStringAppendFmt(DynString *pDynString, const Char *pFmt, ...);
void dynStringDestroy(DynString *pDynString);

#endif /* DYN_STRING_H */
