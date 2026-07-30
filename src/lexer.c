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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static ir_operation get_ir_op(char c)
{
    ir_operation res = {
        .type = IR_NOP,
        .count = 1,
        .offset = 0
    };

    switch (c)
    {
        case '+': res.type = IR_ADD; break;
        case '-': res.type = IR_ADD; res.count = -1; break;
        case '<': res.type = IR_SHIFT; res.count = -1; break;
        case '>': res.type = IR_SHIFT; break;
        case '[': res.type = IR_LLOOP; break;
        case ']': res.type = IR_RLOOP; break;
        case '.': res.type = IR_OUT; break;
        case ',': res.type = IR_IN; break;
    }

    return res;
}

dyn_array *parse_file(file_buf *buf)
{
    dyn_array *operations = dyn_array_create(200, sizeof(ir_operation));
    char *data = buf->data;
    char *end = buf->data + buf->size;
    while (*data != '\0' || data < end)
    {
        ir_operation op = get_ir_op(*data);
        if (op.type != IR_NOP) dyn_array_restrict_insert_end(operations, &op);
        data++;
    }

    ir_operation end_op = {
        .type = IR_END,
        .count = 1
    };

    dyn_array_restrict_insert_end(operations, &end_op);
    return operations;
}

static ir_operation peek(dyn_array * src, size_t read, uint8_t ammount)
{
    if (read + ammount >= dyn_array_get_size(src)) return (ir_operation){ .type = IR_NOP };

    return *(ir_operation*)dyn_array_get(src, read + ammount);
}

#define EMIT_SHIFT() ir_operation shift = { \
    .count = offset, \
    .type = IR_SHIFT \
}; \
dyn_array_restrict_insert_end(dst, &shift);

uint8_t pass_tape_mutation(dyn_array *__restrict__ dst, dyn_array *__restrict__ src, size_t *__restrict__ read)
{
    uint8_t change = 0;
    int32_t offset = 0;
    ir_operation add_ops[50];
    for (uint8_t i = 0; i < 50; i++)
    {
        add_ops[i].count = 0;
        add_ops[i].type = IR_ADD;
        add_ops[i].offset = i - 25;
    }

    size_t len = dyn_array_get_size(src);
    for (; *read < len; (*read)++)
    {
        ir_operation current = *(ir_operation*)dyn_array_get(src, *read);
        if (*read + 1 < len)
        {
            ir_operation next = *(ir_operation*)dyn_array_get(src, *read + 1);
            if (current.type == IR_ADD && next.type == IR_SHIFT)
            {
                add_ops[offset + 25].count += current.count;
                continue;
            } else if (current.type == IR_SHIFT &&  next.type == IR_ADD)
            {
                offset += current.count;
                continue;
            }
        }

        if (current.type == IR_ADD) add_ops[offset + 25].count += current.count;
        else if (current.type == IR_SHIFT) offset += current.count;
        else break;
    }

    for (uint8_t i = 0; i < 50; i++)
    {
        if (abs(add_ops[i].count) > 0)
        {
            dyn_array_restrict_insert_end(dst, &add_ops[i]);
            change = 1;
        }
    }

    if (abs(offset) > 0)
    {
        EMIT_SHIFT()
        offset = 0;
        change = 1;
    }

    return change;
}

#undef EMIT_ADD
#undef EMIT_SHIFT

uint8_t pass_loop(dyn_array *__restrict__ dst, dyn_array *__restrict__ src, size_t *__restrict__ read)
{
    uint8_t change = 0;
    ir_operation op = *(ir_operation*)dyn_array_get(src, *read);
    if (op.type == IR_LLOOP && (*read) + 1 < dyn_array_get_size(src))
    {
        change = 1;
    }
}

void post_process(dyn_array **src)
{
    dyn_array *res = dyn_array_create(200, sizeof(ir_operation));
    size_t read = 0;
    uint8_t change = 0;
    do
    {
        change = 0;
        change |= pass_tape_mutation(res, *src, &read);
    } while(change == 1);

    ir_operation end = {
        .type = IR_END
    };

    dyn_array_restrict_insert_end(res, &end);
    dyn_array_destroy(*src);
    *src = res;
}
