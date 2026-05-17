#include "assembler.h"

void emit_inc_op(ArcType type, string *dst, uint16_t count)
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

void emit_dec_op(ArcType type, string *dst, uint16_t count)
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

void emit_lshift_op(ArcType type, string *dst, uint16_t count)
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

void emit_rshift_op(ArcType type, string *dst, uint16_t count)
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

void emit_loop_start(ArcType type, string *dst, uint16_t *loop_count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: string_append_format(dst, "_L%d_start:\n", ++(*loop_count)); break;
        case ARC_X86_LINUX: break; // ...
    }
}

void emit_loop_end(ArcType type, string *dst, uint16_t *loop_count)
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
