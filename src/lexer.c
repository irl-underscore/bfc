#include "lexer.h"

#include "string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dyn_array *parse_file(file_buf *buf)
{
    if (!buf) return NULL;

    dyn_array *operations = dyn_array_create(50, sizeof(operation_o0));
    if (!operations) return NULL;

    char *root = buf->data;
    for (char curr = *root; curr != '\0'; curr = *++root)
    {
        uint8_t valide = 1;
        operation_o0 op;
        switch (curr) {
            case '+': op.type = OP_INC;    break;
            case '-': op.type = OP_DEC;    break;
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

char *assemble(dyn_array *operations, uint8_t optimization)
{
    if (!operations) return NULL;

    string *code = string_create(100);
    if (!code) return NULL;

    for (int i = 0; i < dyn_array_get_size(operations); ++i)
    {
        if (optimization == 0)
        {
            operation_o0 op = *(operation_o0*)dyn_array_get(operations, i);
            switch (op.type) {
                case OP_INC: string_append_string(code, O0_INC_OPERATION); break;
                case OP_DEC: string_append_string(code, O0_DEC_OPERATION); break;
                case OP_LSHIFT: string_append_string(code, O0_LSHIFT_OPERATION); break;
                case OP_RSHIFT: string_append_string(code, O0_RSHIFT_OPERATION); break;
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
