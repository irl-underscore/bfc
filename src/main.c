#include <stdio.h>

#include "file.h"
#include "dyn_array.h"
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    file_buf *buf = load_file_buf(argv[1]);
    if (!buf)
    {
        fprintf(stderr, "Error: Couldn't load file into buffer");
        return 1;
    }

    dyn_array *operations = parse_file(buf);
    if (!operations)
    {
        fprintf(stderr, "Error: failes to parse file");
        return 1;
    }

    dyn_array *optimized = apply_o1_optimization(operations);
    if (!optimized)
    {
        fprintf(stderr, "Error: couldn't apply optimization");
        return 1;
    }

    char *code = assemble(optimized);
    if (!code)
    {
        fprintf(stderr, "Error: Could't assemble instructions");
        return 1;
    }

    pitch_template(code, "template.s", "out.s");
    dyn_array_destroy(optimized);
    dyn_array_destroy(operations);
    return 0;
}
