#include "dyn_string.h"
#include "base_types.h"
#include "error.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static Bool _dynStringResize(DynString *pDynString);

Bool dynStringCreate(const Size intialCapacity, DynString *pDynString)
{
    pDynString->pData = malloc(intialCapacity);
    if (pDynString->pData == NULL)
    {
        emitStdError();
        return FALSE;
    }

    pDynString->capacity = intialCapacity;
    pDynString->used = 0;
    return TRUE;
}

Bool dynStringAppend(DynString *pDynString, const Char *pStr, Size strLen)
{
    Size neededCapacity = pDynString->used + strLen;
    if (neededCapacity >= pDynString->capacity)
        if (_dynStringResize(pDynString) == FALSE) return FALSE;

    strncpy(pDynString->pData + pDynString->used, pStr, strLen);
    pDynString->used += strLen;
    return TRUE;
}

Bool dynStringAppendFmt(DynString *pDynString, const Char *pFmt, ...)
{
    va_list args;
    va_start(args, pFmt);
    Size strLen = vsnprintf(NULL, 0, pFmt, args);
    va_end(args);
    Size neededCapacity = pDynString->used + strLen;
    if (neededCapacity >= pDynString->capacity)
        if (_dynStringResize(pDynString) == FALSE) return FALSE;

    vsnprintf(pDynString->pData + pDynString->used, strLen, pFmt, args);
    pDynString->used += strLen;
    return TRUE;
}

void dynStringDestroy(DynString *pDynString)
{
    if (pDynString->pData != NULL) free(pDynString->pData);

    pDynString->capacity = 0;
    pDynString->used = 0;
}

static Bool _dynStringResize(DynString *pDynString)
{
    Size newCapacity = (pDynString->capacity >> 1) + pDynString->capacity;
    Char *pTemp = realloc(pDynString->pData, newCapacity);
    if (!pTemp)
    {
        emitStdError();
        return FALSE;
    }

    pDynString->pData = pTemp;
    pDynString->capacity = newCapacity;
    return TRUE;
}
