/*
 * Copyright (c) 2026 irl-underscore
 * SPDX-License-Identifier: Apache-2.0
 * This file is part of brainfuck and is licensed under the Apache 2.0 License.
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
