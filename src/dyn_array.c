#include "dyn_array.h"

#include "arena.h"
#include "defs.h"

#include <string.h>
#include <stdlib.h>

static inline Bool _dynArrayResize(DynArray *pDynArray, Size newSize);

Bool dynArrayCreate(Size initialSize, Size elemSize, DynArray *pDst, Arena *pArena)
{
    pDst->data = arenaAllocate(pArena, initialSize * elemSize);
    if (!pDst->data)
    {
        return FALSE;
    }

    pDst->capacity = initialSize;
    pDst->used = 0;
    pDst->elemSize = elemSize;
    pDst->pArena = pArena;
    return TRUE;
}

Bool dynArrayAppend(DynArray *__restrict pDynArray, void *__restrict elem)
{
    Size requiredCapacity = pDynArray->used + 1;
    if (requiredCapacity >= pDynArray->capacity)
    {
        if (_dynArrayResize(pDynArray, (pDynArray->capacity + (pDynArray->capacity >> 1)) * pDynArray->elemSize) == FALSE) return FALSE;
    }

    memcpy(pDynArray->data + (pDynArray->used * pDynArray->elemSize), elem, pDynArray->elemSize);
    pDynArray->used++;
    return TRUE;
}

void *dynArrayGet(DynArray *pDynArray, Size idx)
{
    if (UNLIKELY(idx >= pDynArray->used))
    {
        return NULL;
    }

    return pDynArray->data + (idx * pDynArray->elemSize);
}

static inline Bool _dynArrayResize(DynArray *pDynArray, Size newSize)
{
    if (arenaRealloc(pDynArray->pArena, (void**)&pDynArray->data, pDynArray->capacity, pDynArray->capacity + (pDynArray->capacity >> 1)) == TRUE)
    {
        pDynArray->capacity = newSize;
        return TRUE;
    }

    return FALSE;
}
