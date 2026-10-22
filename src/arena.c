#include "arena.h"
#include "base_types.h"
#include "defs.h"
#include "error.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

static ArenaChunk *_getFreeChunk(Arena *pArena, Size neededSize);
static Bool _appendArenaChunk(Arena *pArena);

Bool arenaCreate(Size capacity, Arena *pArena)
{
    pArena->pRootChunk = mmap(NULL, sizeof(ArenaChunk) + capacity, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (UNLIKELY(pArena->pRootChunk == MAP_FAILED))
    {
        emitStdError();
        pArena->pRootChunk = NULL;
        return FALSE;
    }

    pArena->pRootChunk->used = 0;
    pArena->capacity = capacity;
    pArena->pRootChunk->pNext = NULL;
    return TRUE;
}

void *arenaAllocate(Arena *pArena, Size size)
{
    ArenaChunk *pTargetChunk = _getFreeChunk(pArena, size);
    if (UNLIKELY(pTargetChunk == NULL))
    {
        if (_appendArenaChunk(pArena) == FALSE) return NULL;
    }

    UPtr absAlignedAddr = ALIGN_UP((UPtr)(&pTargetChunk->data[pTargetChunk->used]), NATURAL_ALIGN(size));
    Size align = absAlignedAddr - (UPtr)&pTargetChunk->data[0];
    void *pRes = pTargetChunk->data + pTargetChunk->used + align;
    pTargetChunk->used += align + size;
    return pRes;
}

Bool arenaRealloc(Arena *pArena, void **ppData, Size oldSize, Size newSize)
{
    ArenaChunk *pCurrent = pArena->pRootChunk;
    while (pCurrent != NULL)
    {
        if ((&pCurrent->data[0]) - oldSize == *ppData && pArena->capacity - pCurrent->used > newSize - oldSize)
        {
            pCurrent->used += newSize - oldSize;
            return TRUE;
        }
    }

    void *pTemp = arenaAllocate(pArena, newSize);
    if (!pTemp) return FALSE;

    memcpy(pTemp, *ppData, oldSize);
    *ppData = pTemp;
    return TRUE;
}

void arenaReset(Arena *pArena)
{
    // TODO: resetting logic for all chunks
}

void arenaDestroy(Arena *pArena)
{
    // TODO: write reverse linked list thingy
}

static ArenaChunk *_getFreeChunk(Arena *pArena, Size neededSize)
{
    ArenaChunk *pCurrent = pArena->pRootChunk;
    while (pCurrent != NULL)
    {
        UPtr abdAlignedAddr = ALIGN_UP((UPtr)&pCurrent->data[pCurrent->used], NATURAL_ALIGN(neededSize));
        Size align = abdAlignedAddr - (UPtr)&pCurrent->data[pCurrent->used];
        Size alignedNeededSize = neededSize + align;
        if (pArena->capacity - pCurrent->used > alignedNeededSize) goto _success;

        pCurrent = pCurrent->pNext;
    }

    return NULL;

_success:
    return pCurrent;
}

static Bool _appendArenaChunk(Arena *pArena)
{
    ArenaChunk *pCurrent = pArena->pRootChunk;
    ArenaChunk *pLast = NULL;
    while (pCurrent != NULL)
    {
        pLast = pCurrent;
        pCurrent = pCurrent->pNext;
    }

    if (pLast == NULL) return FALSE;

    ArenaChunk *pNewChunk = mmap(NULL, sizeof(ArenaChunk) + pArena->capacity, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (pNewChunk == MAP_FAILED)
    {
        emitStdError();
        return FALSE;
    }

    pNewChunk->used = 0;
    pNewChunk->pNext = NULL;
    pLast->pNext = pNewChunk;
    return TRUE;
}
