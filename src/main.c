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

    printf("Buffer: %s", buf->data);
    dyn_array *operations = parse_file(buf);
    if (!operations)
    {
        fprintf(stderr, "Error: failes to parse file\n");
        file_buf_destroy(buf);
        return 1;
    }

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
