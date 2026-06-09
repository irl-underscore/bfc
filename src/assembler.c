#include "assembler.h"

#include "dyn_string.h"

#include <stdlib.h>
#include <string.h>

struct assemble_ctx
{
    uint8_t use_data;
    string *bss;
    string *text;
    string *data;
};

assemble_ctx *ctx_init(size_t tape_size)
{
    tape_size = (tape_size == 0) ? 1 : tape_size;
    assemble_ctx *ctx = malloc(sizeof(assemble_ctx));
    if (!ctx) return NULL;

    ctx->bss = string_create(50);
    string_append_string(ctx->bss, ".section .bss\n");
    string_append_string(ctx->bss, "\t.align 64\n");
    string_append_format(ctx->bss, "\t.lcomm, tape %zu\n", tape_size);

    ctx->text = string_create(100);
    string_append_string(ctx->text, ".section .text\n");
    string_append_string(ctx->text, ".global _start\n");
    string_append_string(ctx->text, "_start:\n");

    ctx->data = string_create(50);
    string_append_string(ctx->data, ".section .data\n");
    ctx->use_data = 0;
    return ctx;
}

#include "sys.c"

void ctx_process(assemble_ctx *ctx, dyn_array *operations, arc_type type)
{
    if (!ctx || !operations) return;

    emit_tape_reg_init(ctx->text, type);
    process_intructions(type, ctx->text, operations);
    emit_syscall(type, ctx->text, BF_CALL_EXIT, "$0", NULL, NULL);
}

char *ctx_assemble(assemble_ctx *ctx)
{
    string *code = string_create(250);
    if (!code) return NULL;

    if (ctx->use_data) string_append_string(code, string_get_raw(ctx->data));

    string_append_string(code, string_get_raw(ctx->bss));
    string_append_string(code, string_get_raw(ctx->text));
    return string_get_raw(code);
}

void ctx_destroy(assemble_ctx *ctx)
{
    if (!ctx) return;

    string_destroy(ctx->bss);
    string_destroy(ctx->text);
    string_destroy(ctx->data);
}
