#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H
#include "base_types.h"
#include "arena.h"

typedef struct DynArray_s
{
    Arena *pArena;
    Size capacity; // per elements
    Size used; // also
    Size elemSize;
    Byte *data;
} DynArray;

Bool dynArrayCreate(Size initalSize, Size elemSize, DynArray *pDst, Arena *pArena);
Bool dynArrayAppend(DynArray *__restrict pDynArray, void *__restrict elem);
void *dynArrayGet(DynArray *pDynArray, Size idx);
void dynArrayDestroy(DynArray *pDynArray);

#endif /* BF_DYN_ARRAY_H */
