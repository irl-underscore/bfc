#include "lexer.h"

#include "string.h"
#include "type.h"

#include <stdlib.h>
#include <string.h>

static ir_type check_loop_start(char **root)
{
    if (strncmp(*root, "[-]", 3) == 0)
    {
        (*root) += 3;
        return IR_CLEAR;
    }

    return IR_LLOOP;
}

static void iterate_arythmic(dyn_array *operations, char **root, char *end)
{
    int64_t count = 0;
    while (*root < end)
    {
        char curr = **root;
        if (curr == '+')
        {
            count++;
            (*root)++;
        } else if (curr == '-')
        {
            count--;
            (*root)++;
        } else if (curr == '\n') (*root)++;
        else if (check_loop_start(root) == IR_CLEAR) count = 0;
        else
        {
            (*root)--;
            break;
        }
    }

    if (count != 0)
    {
        ir_operation res = {
            .count = _abs64(count),
            .type = (count < 0) ? IR_DEC : IR_INC
        };

        dyn_array_restrict_insert_end(operations, &res);
    }
}

static void iterate_shift(dyn_array *operations, char **root, char *end)
{
    int64_t count = 0;
    while (*root < end) {
        char curr = **root;
        if (curr == '<')
        {
            count--;
            (*root)++;
        }
        else if (curr == '>')
        {
            count++;
            (*root)++;
        } else if (curr == '\n') (*root)++;
        else
        {
            (*root)--;
            break;
        }
    }

    if (count != 0)
    {
        ir_operation res = {
            .count = _abs64(count),
            .type = (count < 0) ? IR_LSHIFT : IR_RSHIFT
        };

        dyn_array_restrict_insert_end(operations, &res);
    }
}

static ir_operation translate_sign(char sign)
{
    ir_operation res = {
        .count = 1
    };

    switch (sign)
    {
        case ',': res.type = IR_IN; break;
        case '.': res.type = IR_OUT; break;
        case '[': res.type = IR_LLOOP; break;
        case ']': res.type = IR_RLOOP; break;
    }

    return res;
}

static void insert_loop_start(dyn_array *operations, char **root)
{
    ir_operation op = {
        .type = check_loop_start(root),
        .count = 1
    };

    dyn_array_restrict_insert_end(operations, &op);
}

dyn_array *parse_file(file_buf *buf)
{
    if (!buf) return NULL;

    dyn_array *operations = dyn_array_create(100, sizeof(ir_operation));
    char *root = buf->data;
    char *end = buf->data + buf->size;
    for (char curr = *root; curr != '\0' && root <= end; ++root)
    {
        curr = *root;
        ir_operation sign = translate_sign(curr);
        switch (curr) {
            case '+':
            case '-': iterate_arythmic(operations, &root, end); break;
            case '>':
            case '<': iterate_shift(operations, &root, end); break;
            case ',': dyn_array_restrict_insert_end(operations, &sign); break;
            case '.': dyn_array_restrict_insert_end(operations, &sign); break;
            case '[': insert_loop_start(operations, &root); break;
            case ']': dyn_array_restrict_insert_end(operations, &sign); break;
        }
    }

    return operations;
}
