#ifndef STACK_H
#define STACK_H
#include "base_types.h"
#include "defs.h"

#define STACK_MAX_SIZE 128

typedef struct Stack_s
{
    U16 head;
    U32 elements[STACK_MAX_SIZE];
} Stack;

static inline void stackPush(Stack *pStack, U32 elem)
{
    pStack->elements[pStack->head++] = elem;
}

static inline U32 stackPop(Stack *pStack)
{
    if (UNLIKELY(pStack->head == 0))
    {
        return U32_MAX;
    }

    return pStack->elements[--pStack->head];
}

#endif /* STACK_H */
