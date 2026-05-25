#include "stack.h"

#include <stdlib.h>

struct stack
{
    uint32_t vals[MAX_STACK_SIZE];
    size_t top;
};

stack *stack_create(void)
{
    stack *stk = malloc(sizeof(stack));
    stk->top = 0;
    if (stk) return stk;

    return NULL;
}

void push(stack *stk, uint32_t val)
{
    if (!stk || stk->top >= MAX_STACK_SIZE) return;

    stk->vals[stk->top++] = val;
}

uint32_t pop(stack *stk)
{
    if (!stk ||stk->top == 0) return 0;

    return stk->vals[--stk->top];
}
