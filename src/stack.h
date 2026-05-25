/*
 * Note: That is a verry small stck implementation and just for loop nesting :)
 */

#ifndef STACK_H
#define STACK_H
#include <stdint.h>

#define MAX_STACK_SIZE 100

typedef struct stack stack;

stack *stack_create(void);
void push(stack *stk, uint32_t val);
uint32_t pop(stack *stk);

#endif
