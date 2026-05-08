#include "lexer.h"

#include "string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dyn_array *parse_file(file_buf *buf)
{
    if (!buf) return NULL;

    dyn_array *operations = dyn_array_create(50, sizeof(operation));
    if (!operations) return NULL;

    char *root = buf->data;
    for (char curr = *root; curr != '\0'; curr = *++root)
    {
        uint8_t valide = 1;
        operation op;
        op.count = 1;
        switch (curr) {
            case '+': op.type = OP_INC; break;
            case '-': op.type = OP_DEC; break;
            case '>': op.type = OP_RSHIFT; break;
            case '<': op.type = OP_LSHIFT; break;
            default: valide = 0;
        }

        if (valide)
        {
            dyn_array_emplace_back(operations, &op);
        }
    }

    return operations;
}

static void iterate_arythmic(dyn_array *optimized, dyn_array *operations, int *i)
{
    int count = 0;
    size_t size = dyn_array_get_size(operations);
    printf("Size: %llu\n", size);
    for (; (size_t)(*i) < size; (*i)++)
    {
        operation op = *(operation*)dyn_array_get(operations, *i);
        if (op.type == OP_INC)
        {
            count++;
            printf("\tIncreasing count to %i\n", count);
        } else if (op.type == OP_DEC)
        {
            count--;
            printf("\tDecreasing count to %i\n", count);
        } else
        {
            (*i)--;
            break;
        }
    }

    operation res = {
        .count = abs(count),
        .type = (count < 0) ? OP_DEC : OP_INC
    };

    dyn_array_emplace_back(optimized, &res);
}

static void iterate_shift(dyn_array *optimized, dyn_array *operations, int *i)
{
    int count = 0;
    size_t size = dyn_array_get_size(operations);
    printf("Size: %llu\n", size);
    for (; (size_t)(*i) < size; (*i)++)
    {
        operation op = *(operation*)dyn_array_get(operations, *i);
        if (op.type == OP_LSHIFT)
        {
            count--;
            printf("\tDecreasing count to %i\n", count);
        } else if (op.type == OP_RSHIFT)
        {
            count++;
            printf("\tIncreasing count to %i\n", count);
        } else
        {
            (*i)--;
            break;
        }
    }

    operation res = {
        .count = abs(count),
        .type = (count < 0) ? OP_LSHIFT : OP_RSHIFT
    };

    dyn_array_emplace_back(optimized, &res);
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
            printf("Calling 'iterate_arythmic'(%i):\n", i);
            iterate_arythmic(optimized, operations, &i);
        } else if (op.type == OP_LSHIFT || op.type == OP_RSHIFT)
        {
            printf("Calling 'iterate_shift'(%i):\n", i);
            iterate_shift(optimized, operations, &i);
        }
    }

    printf("Ended at %i\n", i);

    return optimized;
}

char *assemble(dyn_array *operations)
{
    if (!operations) return NULL;

    string *code = string_create(100);
    if (!code) return NULL;

    for (int i = 0; (size_t)i < dyn_array_get_size(operations); ++i)
    {
        operation op = *(operation*)dyn_array_get(operations, i);
        if (op.count == 1)
        {
            switch (op.type)
            {
                case OP_INC: string_append_string(code, INC_OPERATION); break;
                case OP_DEC: string_append_string(code, DEC_OPERATION); break;
                case OP_LSHIFT: string_append_string(code, LSHIFT_OPERATION); break;
                case OP_RSHIFT: string_append_string(code, RSHIFT_OPERATION); break;
            }
        } else if (op.count != 0)
        {
             switch (op.type)
             {
                 case OP_INC:
                 {
                     char instruction[18];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", ADD_OPERATION_FIRST, op.count, ADD_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_DEC:
                 {
                     char instruction[18];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", SUB_OPERATION_FIRST, op.count, SUB_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_LSHIFT:
                 {
                     char instruction[16];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", LSHIFT_OPERATION_FIRST, op.count, LSHIFT_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_RSHIFT:
                 {
                     char instruction[16];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", RSHIFT_OPERATION_FIRST, op.count, RSHIFT_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
             }
        }
    }

    return string_get_raw(code);
}

void pitch_template(char *assembled_code, char *template_file, char *output_file)
{
    if (!assembled_code || !template_file || !output_file) return;

    FILE *in = fopen(template_file, "r");
    FILE *out = fopen(output_file, "w");
    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
        printf("Couldn't open some file!");
        return;
    }

    char line[1024];
    const char *tape_tag = "$(code)";
    size_t tape_len = strlen(tape_tag);
    while (fgets(line, sizeof(line), in))
    {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *tag_pos = strstr(line, tape_tag);
        if (tag_pos)
        {
            fwrite(line, 1, tag_pos - line, out);
            fputs(assembled_code, out);
            fputs(tag_pos + tape_len, out);
        }
        else
        {
            fputs(line, out);
        }
    }

    fclose(out);
    fclose(in);
}
