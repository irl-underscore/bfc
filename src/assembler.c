#include "assembler.h"

#include "type.h"
#include "dyn_string.h"
#include "stack.h"

#include <stdlib.h>
#include <string.h>

static void emit_inc_op(ArcType type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 1) string_append_string(dst, "\tincb (%rbx)\n");
            else if (count > 1) string_append_format(dst, "\taddb $%i, (%%rbx)\n", count);

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
            if (count == 1) string_append_string(dst, "\tdecb (%rbx)\n");
            else if (count > 1) string_append_format(dst, "\tsubb $%i, (%%rbx)\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_clear_op(ArcType type, string *dst)
{
    if (!dst) return;

    switch (type) {
        case ARC_X86_64_LINUX: {
            string_append_string(dst, "\txorq %rbx, %rbx");
            break;
        }
        case ARC_X86_LINUX: {
            break; // ...
        }
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

static void emit_loop_start(ArcType type, string *dst, uint16_t *loop_count, stack *loop_stack)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: string_append_format(dst, ".L%d_:\n", ++(*loop_count)); push(loop_stack, *loop_count);  break;
        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_loop_end(ArcType type, string *dst, stack *loop_stack)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            string_append_string(dst, "\tcmpb (%rbx), $0\n");
            string_append_format(dst, "\tjne .L%d_\n", pop(loop_stack));
            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

#include "syscall.c"

static void process_intructions(ArcType type, string *dst, dyn_array *operations)
{
    if (!dst) return;

    uint16_t loops;
    stack *loop_stack = stack_create();
    size_t arr_size = dyn_array_get_size(operations);
    for (size_t i = 0; i < arr_size; i++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(operations, i);
        switch (op.type)
        {
            case IR_INC: emit_inc_op(type, dst, op.count); break;
            case IR_DEC: emit_dec_op(type, dst, op.count); break;
            case IR_CLEAR: emit_clear_op(type, dst); break;
            case IR_LSHIFT: emit_lshift_op(type, dst, op.count); break;
            case IR_RSHIFT: emit_rshift_op(type, dst, op.count); break;
            case IR_LLOOP: emit_loop_start(type, dst, &loops, loop_stack); break;
            case IR_RLOOP: emit_loop_end(type, dst, loop_stack); break;
            case IR_IN: emit_syscall(type, dst, BF_CALL_READ, "$0", "%rbx", "$1"); break;
            case IR_OUT: emit_syscall(type, dst, BF_CALL_WRITE, "$1", "%rbx", "$1"); break;
        }
    }
}

char *assemble(dyn_array *operations, ArcType type)
{
    if (!operations) return NULL;

    string *code = string_create(100);
    if (!code) return NULL;

    process_intructions(type, code, operations);
    emit_syscall(type, code, BF_CALL_EXIT, "$0", NULL, NULL);
    return string_get_raw(code);
}
