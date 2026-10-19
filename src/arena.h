#ifndef BF_ARENA_H
#define BF_ARENA_H
#include "base_types.h"

typedef struct Arena_s
{
    Size capacity;
    Size used;
    Byte *pData;
} Arena;

Bool arenaCreate(Size initialSize, Arena *pDst);
void *arenaAllocate(Arena *pArena, Size size);
Bool arenaRealloc(Arena *pArena, void **ppData, Size oldSize, Size newSize);
void arenaReset(Arena *pArena);
void arenaDestroy(Arena *pArena);

#endif /* BF_ARENA_H */
