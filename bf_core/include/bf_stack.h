#ifndef BF_STACK_H
#define BF_STACK_H
#include "bf_base_types.h"

#define BF_STACK_MAX_SIZE 128

typedef struct BfStack_s
{
    U16 head;
    U32 elements[BF_STACK_MAX_SIZE];
} BfStack;

static inline void bfStackPush(BfStack *pStack, U32 elem)
{
    pStack->elements[pStack->head++] = elem;
}

static inline U32 bfStackPop(BfStack *pStack)
{
    if (pStack->head == 0) // unlikely
    {
        return U32_MAX;
    }

    return pStack->elements[--pStack->head];
}

#endif /* BF_STACK_H */
