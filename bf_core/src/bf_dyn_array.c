#include "bf_dyn_array.h"

#include "bf_arena.h"
#include "bf_defs.h"

#include <string.h>
#include <stdlib.h>

static inline Bool _dynArrayResize(BfDynArray *pDynArray, Size newSize);

Bool bfDynArrayCreate(Size initialSize, Size elemSize, BfDynArray *pDst, BfArena *pArena)
{
    pDst->data = bfArenaAllocate(pArena, initialSize * elemSize);
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

Bool bfDynArrayAppend(BfDynArray *__restrict pDynArray, void *__restrict elem)
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

void *bfDynArrayGet(BfDynArray *pDynArray, Size idx)
{
    if (UNLIKELY(idx >= pDynArray->used))
    {
        return NULL;
    }

    return pDynArray->data + (idx * pDynArray->elemSize);
}

void bfDynArrayDestroy(BfDynArray *pDynArray)
{
    if (!pDynArray) return;

    if (pDynArray->data) free(pDynArray->data);

    free(pDynArray);
}

static inline Bool _dynArrayResize(BfDynArray *pDynArray, Size newSize)
{
    if (bfArenaRealloc(pDynArray->pArena, (void**)&pDynArray->data, pDynArray->capacity, pDynArray->capacity + (pDynArray->capacity >> 1)) == TRUE)
    {
        pDynArray->capacity = newSize;
        return TRUE;
    }

    return FALSE;
}
