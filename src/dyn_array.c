#include "dyn_array.h"

#include "defs.h"

#include <string.h>
#include <stdlib.h>

static inline Bool _dynArrayResize(DynArray *pDynArray, Size newSize);

Bool dynArrayCreate(Size initialSize, Size elemSize, DynArray *pDst)
{
    pDst->data = malloc(initialSize * elemSize);
    if (!pDst->data)
    {
        return FALSE;
    }

    pDst->capacity = initialSize;
    pDst->used = 0;
    pDst->elemSize = elemSize;
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

void dynArrayDestroy(DynArray *pDynArray)
{
    if (pDynArray->data) free(pDynArray->data);

    pDynArray->capacity = 0;
    pDynArray->elemSize = 0;
    pDynArray->used = 0;
}

static inline Bool _dynArrayResize(DynArray *pDynArray, Size newSize)
{
    Byte *pTemp = realloc(pDynArray->data, newSize);
    if (pTemp)
    {
        pDynArray->data = pTemp;
        pDynArray->capacity = newSize;
        return TRUE;
    }

    return FALSE;
}
