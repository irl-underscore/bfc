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
            case ',': op.type = OP_IN; break;
            case '.': op.type = OP_OUT; break;
            case '[': op.type = OP_LLOOP; break;
            case ']': op.type = OP_RLOOP; break;
            default: valide = 0;
        }

        if (valide)
        {
            dyn_array_restrict_insert_end(operations, &op);
        }
    }

    return operations;
}

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

    operation res = {
        .count = abs(count),
        .type = (count < 0) ? OP_DEC : OP_INC
    };

    dyn_array_restrict_insert_end(optimized, &res);
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

    operation res = {
        .count = abs(count),
        .type = (count < 0) ? OP_LSHIFT : OP_RSHIFT
    };

    dyn_array_restrict_insert_end(optimized, &res);
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

char *assemble(dyn_array *operations)
{
    if (!operations) return NULL;

    uint16_t loops = 0;
    uint8_t open = 0;
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
                case OP_OUT: string_append_string(code, OUT_OPERATION); break;
                case OP_IN: string_append_string(code, IN_OPERATION); break;
                case OP_LLOOP:
                {
                    char instruction[40];
                    snprintf(instruction, sizeof(instruction), "%s%i%s", LOOP_OPERATION, ++loops, LLOOP_OPERATION);
                    string_append_string(code, instruction);
                    open++;
                    break;
                }
                case OP_RLOOP:
                {
                    char instruction[50];
                    snprintf(instruction, sizeof(instruction), "%s%i%s%i%s", RLOOP_OPERATION_FIRST, loops, RLOOP_OPERATION_SECOND, loops, RLOOP_OPERATON_THIRD);
                    string_append_string(code, instruction);
                    open--;
                    break;
                }
            }
        } else if (op.count != 0)
        {
             switch (op.type)
             {
                 case OP_INC:
                 {
                     char instruction[20];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", ADD_OPERATION_FIRST, op.count, ADD_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_DEC:
                 {
                     char instruction[20];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", SUB_OPERATION_FIRST, op.count, SUB_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_LSHIFT:
                 {
                     char instruction[20];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", LSHIFT_OPERATION_FIRST, op.count, LSHIFT_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_RSHIFT:
                 {
                     char instruction[20];
                     snprintf(instruction, sizeof(instruction), "%s%d%s", RSHIFT_OPERATION_FIRST, op.count, RSHIFT_OPERATION_SECOND);
                     string_append_string(code, instruction);
                     break;
                 }
                 case OP_IN: break;
                 case OP_OUT: break;
                 case OP_LLOOP: break;
                 case OP_RLOOP: break;
             }
        }
    }

    if (open > 0)
    {
        printf("Error: Expected %i ']' at the end\n", open);
        return NULL;
    }

    return string_get_raw(code);
}

void pitch_template(const char *assembled_code, const char *template_file, const char *output_file)
{
    if (!assembled_code || !template_file || !output_file) return;

    FILE *in = fopen(template_file, "r");
    if (!in) {
        perror("Error opening input");
        return;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Error opening output");
        fclose(in);
        return;
    }

    char line[1024];
    const char *tape_tag = "$(code)";
    size_t tape_len = strlen(tape_tag) + 1;
    while (fgets(line, sizeof(line), in))
    {
        if (line[0] == '#') continue;

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
