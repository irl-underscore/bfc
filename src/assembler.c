#include "assembler.h"

#include "lexer.h"

#include <stdlib.h>

static void emit_inc_op(ArcType type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 0) string_append_string(dst, "\tincb (%rbx)\n");
            else if (count > 0) string_append_format(dst, "\taddb $%i, (%%rbx)\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_dec_op(ArcType type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 0) string_append_string(dst, "\tdecb (%rbx)\n");
            else if (count > 0) string_append_format(dst, "\tsubb $%i, (%%rbx)\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_lshift_op(ArcType type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 0) string_append_string(dst, "\tdecq %rbx\n");
            else if (count > 0) string_append_format(dst, "\tsubq $%d, %%rbx\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_rshift_op(ArcType type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 0) string_append_string(dst, "\tincq %rbx\n");
            else if (count > 0) string_append_format(dst, "\taddq $%d, %%rbx\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_loop_start(ArcType type, string *dst, uint16_t *loop_count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: string_append_format(dst, "_L%d_start:\n", ++(*loop_count)); break;
        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_loop_end(ArcType type, string *dst, uint16_t *loop_count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            string_append_string(dst, "\tcmpb (%rbx), $0\n");
            string_append_format(dst, "\tjne _L%d_start\n", *loop_count);
            string_append_format(dst, "_L%d_end:\n", *loop_count);
            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

char *assemble(dyn_array *operations, ArcType type)
{
    if (!operations) return NULL;

    uint16_t loops = 0;
    string *code = string_create(100);
    if (!code) return NULL;

    for (int i = 0; (size_t)i < dyn_array_get_size(operations); ++i)
    {
        operation op = *(operation*)dyn_array_get(operations, i);
        switch (op.type)
        {
            case OP_INC: emit_inc_op(type, code, op.count); break;
            case OP_DEC: emit_dec_op(type, code, op.count); break;
            case OP_LSHIFT: emit_lshift_op(type, code, op.count); break;
            case OP_RSHIFT: emit_rshift_op(type, code, op.count); break;
            case OP_LLOOP: emit_loop_start(type, code, &loops); break;
            case OP_RLOOP: emit_loop_end(type, code, &loops); break;
            case OP_IN: emit_syscall(type, code, BF_CALL_READ, "$0", "%rbx", "$1"); break;
            case OP_OUT: emit_syscall(type, code, BF_CALL_WRITE, "$1", "%rbx", "$1"); break;
        }
    }

    emit_syscall(type, code, BF_CALL_EXIT, "$0", NULL, NULL);
    return string_get_raw(code);
}
