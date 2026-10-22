#include "arena.h"
#include "base_types.h"
#include "defs.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

Bool arenaCreate(Size initialSize, Arena *pArena)
{
    pArena->pData = mmap(NULL, initialSize, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (UNLIKELY(pArena->pData == MAP_FAILED))
    {
        emitStdError();
        pArena->pData = NULL;
        return FALSE;
    }

    pArena->capacity = initialSize;
    pArena->used = 0;
    return TRUE;
}

void *arenaAllocate(Arena *pArena, Size size)
{
    UPtr alignedData = ALIGN_UP((UPtr)(pArena->pData + pArena->used), NATURAL_ALIGN(size));
    Size alignedDiff = alignedData - (UPtr)(pArena->pData + pArena->used);
    Size neededCapacity = pArena->used + alignedDiff + size;
    if (UNLIKELY(neededCapacity >= pArena->capacity))
    {
        return NULL; // TODO: Add something like resizing
    }

    void *pRes = pArena->pData + pArena->used + alignedDiff;
    pArena->used += alignedDiff + size;
    return pRes;
}

Bool arenaRealloc(Arena *pArena, void **ppData, Size oldSize, Size newSize)
{
    if (pArena->pData - oldSize == *ppData) // ppData was the last allocated element
    {
        pArena->pData -= oldSize;
    }

    void *pTemp = arenaAllocate(pArena, newSize);
    if (!pTemp) return FALSE;

    *ppData = pTemp;
    return TRUE;
}

void arenaReset(Arena *pArena)
{
    pArena->used = 0;
}

void arenaDestroy(Arena *pArena)
{
    if (pArena->pData) munmap(pArena->pData, pArena->capacity);
}
