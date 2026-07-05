#include <stdlib.h>

#include "lexer.h"
#include "assembler.h"
#include "file.h"
#include "args.h"

#include <stdio.h>

void print_help(void)
{
    printf("Still working on that one :)\n");
}

void print_version(void)
{
    printf("Not done yet :)\n");
}

compiler_options opt = {
    .help = 0,
    .version = 0,
    .target = ARC_X86_64_LINUX,
    .output = "a.s",
    .input = NULL,
};

int main(int argc, char *argv[])
{
    process_args(argv, argc, &opt);
    if (opt.help == 1)
    {
        print_help();
        return 0;
    }

    if (opt.version == 1)
    {
        print_version();
    }

    if (opt.input)
    {
        file_buf *buf = file_buf_load(opt.input);
        dyn_array *operations = parse_file(buf);
        file_buf_destroy(buf);
        dyn_array *optimized = post_process(operations);
        assemble_ctx *ctx = ctx_init(1050);
        ctx_process(ctx, optimized, opt.target);
        dyn_array_destroy(optimized);
        char *code = ctx_assemble(ctx);
        ctx_destroy(ctx);
        pitch_code(code, opt.output);
        free(code);
    }

    return 0;
}
