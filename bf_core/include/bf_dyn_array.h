#ifndef BF_DYN_ARRAY_H
#define BF_DYN_ARRAY_H
#include "bf_base_types.h"
#include "bf_arena.h"

typedef struct BfDynArray_s
{
    BfArena *pArena;
    Size capacity; // per elements
    Size used; // also
    Size elemSize;
    Byte *data;
} BfDynArray;

Bool bfDynArrayCreate(Size initalSize, Size elemSize, BfDynArray *pDst, BfArena *pArena);
Bool bfDynArrayAppend(BfDynArray *__restrict pDynArray, void *__restrict elem);
void *bfDynArrayGet(BfDynArray *pDynArray, Size idx);
void bfDynArrayDestroy(BfDynArray *pDynArray);

#endif /* BF_DYN_ARRAY_H */
