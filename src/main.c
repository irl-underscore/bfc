#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "assembler.h"
#include "file.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "The feature that u want to use is not done yet :) ig");
        return 1;
    }

    file_buf *buf = file_buf_load(argv[1]);
    dyn_array *operations = parse_file(buf);
    file_buf_destroy(buf);
    dyn_array *opt = post_process(operations);
    assemble_ctx *ctx = ctx_init(1050);
    ctx_process(ctx, opt, ARC_X86_64_LINUX);
    dyn_array_destroy(opt);
    char *code = ctx_assemble(ctx);
    ctx_destroy(ctx);
    pitch_code(code, "out.s");
    free(code);
    return 0;
}
