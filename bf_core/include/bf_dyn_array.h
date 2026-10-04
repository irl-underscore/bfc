#ifndef BF_DYN_ARRAY_H
#define BF_DYN_ARRAY_H
#include "bf_base_types.h"

typedef struct BfDynArray_s BfDynArray;

BfDynArray *bfDynArrayCreate(Size initalSize, Size elemSize);
void bfDynArrayAppend(BfDynArray *__restrict pDynArray, void *__restrict elem);
void *bfDynArrayGet(BfDynArray *pDynArray, Size idx);
void bfDynArrayDestroy(BfDynArray *pDynArray);

#endif /* BF_DYN_ARRAY_H */
