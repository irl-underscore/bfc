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
    assemble_ctx *ctx = ctx_init(1050);
    ctx_process(ctx, operations, ARC_X86_64_LINUX);
    char *code = ctx_assemble(ctx);
    printf("%s\n", code);
    file_buf_destroy(buf);
    dyn_array_destroy(operations);
    ctx_destroy(ctx);
    free(code);
    return 0;
}
