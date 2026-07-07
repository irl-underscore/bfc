/*
 * Copyright 2026 irl-underscore
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "lexer.h"

#include "string.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static ir_type get_type(char c)
{
    switch (c)
    {
        case '+': return IR_INC;
        case '-': return IR_DEC;
        case '<': return IR_LSHIFT;
        case '>': return IR_RSHIFT;
        case ',': return IR_IN;
        case '.': return IR_OUT;
        case '[': return IR_LLOOP;
        case ']': return IR_RLOOP;
        default: return 10;
    }
}

dyn_array *parse_file(file_buf *buf)
{
    if (!buf) return NULL;

    dyn_array *operations = dyn_array_create(200, sizeof(ir_operation));
    char *data = buf->data;
    char *end = buf->data + buf->size;
    while (*data != '\0' || data < end)
    {
        ir_type type = get_type(*data);
        if (type != 10)
        {
            ir_operation op = {
                .type = type,
                .count = 1
            };

            dyn_array_restrict_insert_end(operations, &op);
        }
        data++;
    }

    ir_operation end_op = {
        .type = IR_END,
        .count = 1
    };

    dyn_array_restrict_insert_end(operations, &end_op);
    return operations;
}

static void iterate_arythmic(size_t *i, dyn_array *operations, dyn_array *dst, size_t end)
{
    int64_t count = 0;
    for (; *i < end; (*i)++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(operations, *i);
        if (op.type == IR_INC) count++;
        else if (op.type == IR_DEC) count--;
        else break;
    }
    if (count != 0)
    {
        ir_operation op = {
            .type = (count < 0) ? IR_DEC : IR_INC,
            .count = llabs(count)
        };

        dyn_array_restrict_insert_end(dst, &op);
    }
}

static void iterate_shift(size_t *i, dyn_array *operations, dyn_array *dst, size_t end)
{
    int64_t count = 0;
    for (; *i < end; (*i)++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(operations, *i);
        if (op.type == IR_RSHIFT) count++;
        else if (op.type == IR_LSHIFT) count--;
        else break;

    }

    if (count != 0)
    {
        ir_operation op = {
            .type = (count < 0) ? IR_LSHIFT : IR_RSHIFT,
            .count = llabs(count)
        };

        dyn_array_restrict_insert_end(dst, &op);
    }
}

static void check_loop(size_t *i, dyn_array *operations, dyn_array *dst, dyn_array *temp);

static void iterate_instructions(size_t *i, dyn_array *operations, dyn_array *dst, ir_type end_cond)
{
    dyn_array *temp = dyn_array_create(50, sizeof(ir_operation));
    size_t end = dyn_array_get_size(operations);
    while (*i < end)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(operations, *i);
        if (op.type == end_cond) break;

        switch (op.type)
        {
            case IR_INC:
            case IR_DEC: iterate_arythmic(i, operations, dst, end); break;
            case IR_LSHIFT:
            case IR_RSHIFT: iterate_shift(i, operations, dst, end); break;
            case IR_LLOOP: check_loop(i, operations, dst, temp); break;
            default: {
                dyn_array_restrict_insert_end(dst, &op);
                (*i)++;
                break;
            }
        }
    }

    dyn_array_destroy(temp);
}

static void check_loop(size_t *i, dyn_array *operations, dyn_array *dst, dyn_array *temp)
{
    (*i)++;
    iterate_instructions(i, operations, temp, IR_RLOOP);
    size_t length = dyn_array_get_size(temp);
    (*i)++;
    if (length == 1)
    {
        ir_operation expec = *(ir_operation*)dyn_array_get(temp, 0);
        if ((expec.type == IR_INC || expec.type == IR_DEC) && expec.count == 1)
        {
            ir_operation res = {
                .type = IR_CLEAR,
                .count = 1
            };

            dyn_array_insert_end(dst, &res);
            return;
        }
    }

    ir_operation start = {
        .type = IR_LLOOP,
        .count = 1
    };
    dyn_array_restrict_insert_end(dst, &start);
    for (size_t i = 0; i < length; i++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(temp, i);
        dyn_array_restrict_insert_end(dst, &op);
    }
    ir_operation end = {
        .type = IR_RLOOP,
        .count = 1
    };
    dyn_array_restrict_insert_end(dst, &end);
}

dyn_array *post_process(dyn_array *operations)
{
    if (!operations) return NULL;

    dyn_array *res = dyn_array_create(200, sizeof(ir_operation));
    size_t i = 0;
    iterate_instructions(&i, operations, res, IR_END);
    ir_operation end = {
        .type = IR_END,
        .count = 1
    };

    dyn_array_restrict_insert_end(res, &end);
    return res;
}
