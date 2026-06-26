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
    size_t size1 = dyn_array_get_size(operations);
    printf("%zu\n", size1);
    for (size_t i = 0; i < size1; i++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(operations, i);
        switch (op.type) {
            case IR_DEC:
            case IR_INC: printf("+/-\n"); break;
            case IR_LLOOP:
            case IR_RLOOP: printf("[/]\n"); break;
            case IR_CLEAR: printf("--\n"); break;
            case IR_OUT:
            case IR_IN: printf("./,\n"); break;
            case IR_RSHIFT:
            case IR_LSHIFT: printf("</>\n"); break;
        }
    }
    dyn_array *opt = post_process(operations);
    size_t size = dyn_array_get_size(opt);
    printf("%zu\n", size);
    for (size_t i = 0; i < size; i++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(opt, i);
        switch (op.type) {
            case IR_DEC:
            case IR_INC: printf("+/-\n"); break;
            case IR_LLOOP:
            case IR_RLOOP: printf("[/]\n"); break;
            case IR_CLEAR: printf("--\n"); break;
            case IR_OUT:
            case IR_IN: printf("./,\n"); break;
            case IR_RSHIFT:
            case IR_LSHIFT: printf("</>\n"); break;
        }
    }

    assemble_ctx *ctx = ctx_init(1050);
    ctx_process(ctx, opt, ARC_X86_64_LINUX);
    dyn_array_destroy(opt);
    char *code = ctx_assemble(ctx);
    ctx_destroy(ctx);
    pitch_code(code, "out.s");
    free(code);
    return 0;
}
