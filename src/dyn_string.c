#include "dyn_string.h"
#include "arena.h"
#include "base_types.h"
#include "error.h"
#include "test.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static Bool _dynStringResize(DynString *pDynString, Size size);

Bool dynStringCreate(const Size intialCapacity, Arena *pArena, DynString *pDynString)
{
    pDynString->pData = arenaAllocate(pArena, intialCapacity);
    if (pDynString->pData == NULL)
    {
        emitStdError();
        return FALSE;
    }

    pDynString->capacity = intialCapacity;
    pDynString->used = 0;
    pDynString->pArena = pArena;
    return TRUE;
}

Bool dynStringAppend(DynString *pDynString, const Char *pStr)
{
    Size strLen = strnlen(pStr, 128);
    Size neededCapacity = pDynString->used + strLen;
    if (neededCapacity >= pDynString->capacity)
        if (_dynStringResize(pDynString, strLen) == FALSE) return FALSE;

    strncpy(pDynString->pData + pDynString->used, pStr, strLen);
    pDynString->used += strLen;
    return TRUE;
}

Bool dynStringAppendFmt(DynString *pDynString, const Char *pFmt, ...)
{
    va_list args;
    va_start(args, pFmt);
    Size strLen = vsnprintf(NULL, 0, pFmt, args);
    strLen++;
    TEST_LOG_FMT("strlen: %zu", strLen);
    va_end(args);
    Size neededCapacity = pDynString->used + strLen;
    if (neededCapacity >= pDynString->capacity)
        if (_dynStringResize(pDynString, strLen) == FALSE) return FALSE;

    va_start(args, pFmt);
    vsnprintf(pDynString->pData + pDynString->used, strLen, pFmt, args);
    va_end(args);
    pDynString->used += strLen;
    return TRUE;
}

void dynStringDestroy(DynString *pDynString)
{
    if (pDynString->pData != NULL) free(pDynString->pData);

    pDynString->capacity = 0;
    pDynString->used = 0;
}

static Bool _dynStringResize(DynString *pDynString, Size size)
{
    Size newCapacity = (pDynString->capacity >> 1) + pDynString->capacity;
    if (newCapacity < size) newCapacity = size;

    arenaRealloc(pDynString->pArena, (void**)&pDynString->pData, pDynString->capacity, newCapacity); // error handling comes later
    pDynString->capacity = newCapacity;
    return TRUE;
}
