#include "bf_arena.h"
#include "bf_base_types.h"
#include "bf_defs.h"
#include "bf_error.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

Bool bfArenaCreate(Size initialSize, BfArena *pDst)
{
    pDst->pData = mmap(NULL, initialSize, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (UNLIKELY(pDst->pData == MAP_FAILED))
    {
        emitStdError();
        pDst->pData = NULL;
        return FALSE;
    }

    pDst->capacity = initialSize;
    pDst->used = 0;
    return TRUE;
}

void *bfArenaAllocate(BfArena *pArena, Size size)
{
    UPtr alignedData = BF_ALIGN_UP((UPtr)(pArena->pData + pArena->used), BF_NATURAL_ALIGN(size));
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

Bool bfArenaRealloc(BfArena *pArena, void **ppData, Size oldSize, Size newSize)
{
    if (pArena->pData - oldSize == *ppData) // ppData was the last allocated element
    {
        pArena->pData -= oldSize;
    }

    void *pTemp = bfArenaAllocate(pArena, newSize);
    if (!pTemp) return FALSE;

    *ppData = pTemp;
    return TRUE;
}

void bfArenaReset(BfArena *pArena)
{
    pArena->used = 0;
}

void bfArenaDestroy(BfArena *pArena)
{
    if (pArena->pData) munmap(pArena->pData, pArena->capacity);
}
