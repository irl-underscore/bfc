#include <stdlib.h>

#include "lexer.h"
#include "assembler.h"
#include "file.h"
#include "args.h"

#include <stdio.h>

void print_help(void)
{
    printf("Still working on that one :)");
}

compiler_options opt = {
    .help = 0,
    .output = "a.exe",
    .input = "main.bf"
};

int main(int argc, char *argv[])
{
    process_args(argv, argc, &opt);
    if (opt.help == 1)
    {
        print_help();
        return 0;
    }

    file_buf *buf = file_buf_load(opt.input);
    dyn_array *operations = parse_file(buf);
    file_buf_destroy(buf);
    dyn_array *optimized = post_process(operations);
    assemble_ctx *ctx = ctx_init(1050);
    ctx_process(ctx, optimized, ARC_X86_64_LINUX);
    dyn_array_destroy(optimized);
    char *code = ctx_assemble(ctx);
    ctx_destroy(ctx);
    pitch_code(code, opt.output);
    free(code);
    return 0;
}
