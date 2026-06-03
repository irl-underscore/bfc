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
    for (size_t i = 0; i < dyn_array_get_size(operations); i++);
    char *code = assemble(operations, ARC_X86_64_LINUX);
    pitch_template(code, "template.s", "out.s");
    file_buf_destroy(buf);
    dyn_array_destroy(operations);
    free(code);
    return 0;
}
