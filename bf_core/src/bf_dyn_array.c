#include "bf_dyn_array.h"

#include "bf_defs.h"

#include <string.h>
#include <stdlib.h>

struct BfDynArray_s
{
    Size capacity; // per elements
    Size used; // also
    Size elemSize;
    Byte *data;
};

static void _dynArrayResize(BfDynArray *pDynArray, Size newSize);

BfDynArray *bfDynArrayCreate(Size initialSize, Size elemSize)
{
    BfDynArray *pDynArray = malloc(sizeof(BfDynArray));
    pDynArray->data = malloc(initialSize * elemSize);
    pDynArray->capacity = initialSize;
    pDynArray->used = 0;
    pDynArray->elemSize = elemSize;
    return pDynArray;
}

void bfDynArrayAppend(BfDynArray *__restrict pDynArray, void *__restrict elem)
{
    Size requiredCapacity = pDynArray->used + 1;
    if (requiredCapacity >= pDynArray->capacity)
    {
        _dynArrayResize(pDynArray, (pDynArray->capacity + (pDynArray->capacity >> 1)) * pDynArray->elemSize);
    }

    memcpy(pDynArray->data + (pDynArray->used * pDynArray->elemSize), elem, pDynArray->elemSize);
    pDynArray->used++;
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

static void _dynArrayResize(BfDynArray *pDynArray, Size newSize)
{
    Byte *tempPtr = realloc(pDynArray->data, newSize);
    if (LIKELY(tempPtr))
    {
        pDynArray->data = tempPtr;
        pDynArray->capacity = newSize;
    }
}
