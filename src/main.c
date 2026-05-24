#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "assembler.h"
#include "file.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "The feature that u want to use is not done yet :)");
        return 1;
    }

    file_buf *buf = file_buf_load(argv[1]);
    dyn_array *operations = parse_file(buf);
    dyn_array *optimized = apply_o1_optimization(operations);
    char *code = assemble(optimized, ARC_X86_64_LINUX);;
    pitch_template(code, "template.s", "out.s");
    file_buf_destroy(buf);
    dyn_array_destroy(optimized);
    dyn_array_destroy(operations);
    free(code);
    return 0;
}
