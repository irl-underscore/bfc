#ifndef BF_ARENA_H
#define BF_ARENA_H
#include "base_types.h"

typedef struct ArenaChunk_s
{
    Size used;
    Size capacity;
    struct ArenaChunk_s *pNext;
    Byte data[];
} ArenaChunk;

typedef struct Arena_s
{
    Size baseCapacity;
    ArenaChunk *pRootChunk;
} Arena;

Bool arenaCreate(Size initialSize, Arena *pArena);
void *arenaAllocate(Arena *pArena, Size size);
Bool arenaRealloc(Arena *pArena, void **ppData, Size oldSize, Size newSize);
void arenaReset(Arena *pArena);
void arenaDestroy(Arena *pArena);

#endif /* BF_ARENA_H */
