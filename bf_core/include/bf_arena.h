#ifndef BF_ARENA_H
#define BF_ARENA_H
#include "bf_base_types.h"

typedef struct BfArena_s
{
    Size capacity;
    Size used;
    Byte *pData;
} BfArena;

Bool bfArenaCreate(Size initialSize, BfArena *pDst);
void *bfArenaAllocate(BfArena *pArena, Size size);
Bool bfArenaRealloc(BfArena *pArena, void **ppData, Size oldSize, Size newSize);
void bfArenaReset(BfArena *pArena);
void bfArenaDestroy(BfArena *pArena);

#endif /* BF_ARENA_H */
