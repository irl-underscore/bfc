#include "lexer.h"

#include "string.h"
#include "type.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void iterate_arythmic(dyn_array *optimized, dyn_array *operations, int *i)
{
    int count = 0;
    size_t size = dyn_array_get_size(operations);
    for (; (size_t)(*i) < size; (*i)++)
    {
        operation op = *(operation*)dyn_array_get(operations, *i);
        if (op.type == OP_INC) count++;
        else if (op.type == OP_DEC) count--;
        else
        {
            (*i)--;
            break;
        }
    }

    if (count != 0)
    {
        operation res = {
            .count = abs(count),
            .type = (count < 0) ? OP_DEC : OP_INC
        };

        dyn_array_restrict_insert_end(optimized, &res);
    }
}

static void iterate_shift(dyn_array *optimized, dyn_array *operations, int *i)
{
    int count = 0;
    size_t size = dyn_array_get_size(operations);
    for (; (size_t)(*i) < size; (*i)++)
    {
        operation op = *(operation*)dyn_array_get(operations, *i);
        if (op.type == OP_LSHIFT) count--;
        else if (op.type == OP_RSHIFT) count++;
        else
        {
            (*i)--;
            break;
        }
    }

    if (count != 0)
    {
        operation res = {
            .count = abs(count),
            .type = (count < 0) ? OP_LSHIFT : OP_RSHIFT
        };

        dyn_array_restrict_insert_end(optimized, &res);
    }
}

dyn_array *apply_o1_optimization(dyn_array *operations)
{
    if (!operations) return NULL;

    dyn_array *optimized = dyn_array_create(dyn_array_get_size(operations), sizeof(operation));
    if (!optimized) return NULL;

    int i = 0;
    for (; (size_t)i < dyn_array_get_size(operations); i++)
    {
        operation op = *(operation*)dyn_array_get(operations, i);
        if (op.type == OP_DEC || op.type == OP_INC)
        {
            iterate_arythmic(optimized, operations, &i);
        } else if (op.type == OP_LSHIFT || op.type == OP_RSHIFT)
        {
            iterate_shift(optimized, operations, &i);
        } else if (op.type == OP_OUT)
        {
            dyn_array_restrict_insert_end(optimized, &op);
        } else if (op.type == OP_IN)
        {
            dyn_array_restrict_insert_end(optimized, &op);
        } else if (op.type == OP_LLOOP)
        {
            dyn_array_restrict_insert_end(optimized, &op);
        } else if (op.type == OP_RLOOP)
        {
            dyn_array_restrict_insert_end(optimized, &op);
        }
    }

    return optimized;
}
