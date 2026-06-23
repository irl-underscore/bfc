#include "dyn_string.h"
#include "assembler.h"
#include "stack.h"
#include "lexer.h"

#include "syscall/x86_linux.h"
#include "syscall/x86_64_linux.h"

#include <string.h>

/*
 * Syscalls
 */

static void emit_x86_linux(string *dst, bf_call call, char *arg0, char *arg1, char *arg2)
{
    int syscall = x86_linux_syscall_table[call] - 1;
    if (syscall < 0)
    {
        // error handling
        return;
    }

    string_append_format(dst, "\tmovl $%d, %%eax\n", syscall);
    if (arg0)
    {
        if (strncmp(arg0, "$0", 2) == 0) string_append_string(dst, "\txorl %ebx, %ebx\n");
        else string_append_format(dst, "\tmovl %s, %%ebx\n", arg0);
    }

    if (arg1)
    {
        if (strncmp(arg1, "$0", 2) == 0) string_append_string(dst, "\txorl %ecx, %ecx");
        else string_append_format(dst, "\tmovl %s, %%ecx\n", arg1);
    }

    if (arg2)
    {
        if (strncmp(arg2, "$0", 2) == 0) string_append_string(dst, "\txorl %edx, %edx");
        else string_append_format(dst, "\tmovl %s, %%edx\n", arg2);
    }

    string_append_string(dst, "\tint $0x80\n");
}

static void emit_x86_64_linux(string *dst, bf_call call, char *arg0, char *arg1, char *arg2)
{
    int syscall = x86_64_linux_syscall_table[call] - 1;
    if (syscall < 0)
    {
        // error handling
        return;
    }

    string_append_format(dst, "\tmovq $%d, %%rax\n", syscall);
    if (arg0)
    {
        if (strncmp(arg0, "$0", 2) == 0) string_append_string(dst, "\txorq %rdi, %rdi\n");
        else string_append_format(dst, "\tmovq %s, %%rdi\n", arg0);
    }

    if (arg1)
    {
        if (strncmp(arg1, "$0", 2) == 0) string_append_string(dst, "\txorq %rsi, %rsi\n");
        else string_append_format(dst, "\tmovq %s, %%rsi\n", arg1);
    }

    if (arg2)
    {
        if (strncmp(arg2, "$0", 2) == 0) string_append_string(dst, "\txorq %rdx, %rdx\n");
        else string_append_format(dst, "\tmovq %s, %%rdx\n", arg2);
    }

    string_append_string(dst, "\tsyscall\n");
}

static void emit_syscall(arc_type target, string *dst, bf_call call, char *arg0, char *arg1, char *arg2)
{
    if (!dst) return;

    switch (target)
    {
        case ARC_X86_LINUX: emit_x86_linux(dst, call, arg0, arg1, arg2); break;
        case ARC_X86_64_LINUX: emit_x86_64_linux(dst, call, arg0, arg1, arg2); break;
    }
}

/*
 * Assemblage
 */

static void emit_inc_op(arc_type type, string *dst, uint16_t count)
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

static void emit_dec_op(arc_type type, string *dst, uint16_t count)
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

static void emit_clear_op(arc_type type, string *dst)
{
    if (!dst) return;

    switch (type) {
        case ARC_X86_64_LINUX: {
            string_append_string(dst, "\txorq %rbx, %rbx\n");
            break;
        }
        case ARC_X86_LINUX: {
            break; // ...
        }
    }
}

static void emit_lshift_op(arc_type type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 1) string_append_string(dst, "\tdecq %rbx\n");
            else if (count > 1) string_append_format(dst, "\tsubq $%d, %%rbx\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_rshift_op(arc_type type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 1) string_append_string(dst, "\tincq %rbx\n");
            else if (count > 1) string_append_format(dst, "\taddq $%d, %%rbx\n", count);

            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_loop_start(arc_type type, string *dst, uint16_t *loop_count, stack *loop_stack)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX:
        {
            string_append_format(dst, ".L%d_start:\n", ++(*loop_count));
            push(loop_stack, *loop_count);
            string_append_string(dst, "\tcmpb %rbx, $0\n");
            string_append_format(dst, "\tje .L%d_end\n", *loop_count);
            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void emit_loop_end(arc_type type, string *dst, stack *loop_stack)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            uint16_t loop_count = pop(loop_stack);
            string_append_format(dst, "\tjmp .L%d_start\n", loop_count);
            string_append_format(dst, ".L%d_end:\n", loop_count);
            break;
        }

        case ARC_X86_LINUX: break; // ...
    }
}

static void process_intructions(arc_type type, string *dst, dyn_array *operations)
{
    if (!dst) return;

    uint16_t loops = 0;
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

static void emit_tape_reg_init(string *text, arc_type type)
{
    switch (type)
    {
        case ARC_X86_64_LINUX: string_append_string(text, "\tlea tape(%rip), %rbx\n"); break;
        case ARC_X86_LINUX: break;
    }
}
