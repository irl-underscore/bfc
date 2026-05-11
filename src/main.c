#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    file_buf *buf = file_buf_load(argv[1]);
    if (!buf)
    {
        fprintf(stderr, "Error: Couldn't load file into buffer\n");
        return 1;
    }

    dyn_array *operations = parse_file(buf);
    if (!operations)
    {
        file_buf_destroy(buf);
        return 1;
    }

    printf("Operations: ");
    for (int i = 0; i < dyn_array_get_size(operations); i++)
    {
        operation op = *(operation*)dyn_array_get(operations, i);
        switch (op.type)
        {
            case OP_INC: printf("'+' "); break;
            case OP_DEC: printf("'-' "); break;
            case OP_LSHIFT: printf("'<' "); break;
            case OP_RSHIFT: printf("'>' "); break;
            case OP_OUT: printf("'.' "); break;
            case OP_IN: printf("',' "); break;
        }
    }

    printf("\n");
    dyn_array *optimized = apply_o1_optimization(operations);
    if (!optimized)
    {
        fprintf(stderr, "Error: couldn't apply optimization\n");
        file_buf_destroy(buf);
        dyn_array_destroy(operations);
        return 1;
    }

    char *code = assemble(optimized);
    if (!code)
    {
        fprintf(stderr, "Error: Could't assemble instructions\n");
        file_buf_destroy(buf);
        dyn_array_destroy(operations);
        free(code);
        return 1;
    }

    pitch_template(code, "template.s", "out.s");
    file_buf_destroy(buf);
    dyn_array_destroy(optimized);
    dyn_array_destroy(operations);
    free(code);
    return 0;
}
