/*
 * Copyright 2016 irl-underscore
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dyn_string.h"
#include "assembler.h"
#include "stack.h"
#include "lexer.h"

#include "tables/syscall/syscall_tables.h"

#include <string.h>
#include <stdio.h>

static void emit_x86_linux(string *dst, bf_call call, char *arg0, char *arg1, char *arg2)
{
    uint8_t syscall = x86_linux_syscall_table[call] - 1;
    if (syscall == 0)
    {
        string_append_string(dst, "\txorl %eax, %eax\n");
    } else
    {
        string_append_format(dst, "\tmovl $%d, %%eax\n", syscall);
    }

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
    uint8_t syscall = x86_64_linux_syscall_table[call] - 1;
    if (syscall == 0)
    {
        string_append_string(dst, "\txorq %rax, %rax\n");
    } else
    {
        string_append_format(dst, "\tmovq $%d, %%rax\n", syscall);
    }

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

static void emit_inc_op(arc_type type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 1) string_append_string(dst, "\tincb (%rbx)\n");
            else if (count > 1) string_append_format(dst, "\taddb $%u, (%%rbx)\n", count);

            break;
        }

        case ARC_X86_LINUX: {
            if (count == 1) string_append_string(dst, "\tincb (%edi)\n");
            else if (count > 1) string_append_format(dst, "\taddb $%u, (%%edi)\n", count);

            break;
        }
    }
}

static void emit_dec_op(arc_type type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 1) string_append_string(dst, "\tdecb (%rbx)\n");
            else if (count > 1) string_append_format(dst, "\tsubb $%u, (%%rbx)\n", count);

            break;
        }

        case ARC_X86_LINUX: {
            if (count == 1) string_append_string(dst, "\tdecb (%edi)\n");
            else if (count > 1) string_append_format(dst, "\tsubb $%u, (%%edi)\n", count);

            break;
        }
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
            string_append_string(dst, "\txorl %edi, %edi\n");
            break;
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
            else if (count > 1) string_append_format(dst, "\tsubq $%u, %%rbx\n", count);

            break;
        }

        case ARC_X86_LINUX: {
            if (count == 1) string_append_string(dst, "decl %edi\n");
            else if (count > 1) string_append_format(dst, "\tsubl $%u, %%edi\n", count);

            break;
        }
    }
}

static void emit_rshift_op(arc_type type, string *dst, uint16_t count)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (count == 1) string_append_string(dst, "\tincq %rbx\n");
            else if (count > 1) string_append_format(dst, "\taddq $%u, %%rbx\n", count);

            break;
        }

        case ARC_X86_LINUX: {
            if (count == 1) string_append_string(dst, "\tincl %edi\n");
            else if (count > 1) string_append_format(dst, "\taddl $%u, %%edi\n", count);

            break;
        }
    }
}

static void emit_loop_start(arc_type type, string *dst, uint16_t *loop_count, stack *loop_stack)
{
    if (!dst) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            string_append_format(dst, ".L%u_start:\n", ++(*loop_count));
            push(loop_stack, *loop_count);
            string_append_string(dst, "\tcmpb $0, (%rbx)\n");
            string_append_format(dst, "\tje .L%u_end\n", *loop_count);
            break;
        }

        case ARC_X86_LINUX: {
            string_append_format(dst, ".L%u_start:\n", ++(*loop_count));
            push(loop_stack, *loop_count);
            string_append_string(dst, "\tcmpb $0, (%edi)\n");
            string_append_format(dst, "\tje .L%u_end\n", *loop_count);
            break;
        }
    }
}

static void emit_loop_end(string *dst, stack *loop_stack)
{
    if (!dst) return;

    uint16_t loop_count = pop(loop_stack);
    string_append_format(dst, "\tjmp .L%u_start\n", loop_count);
    string_append_format(dst, ".L%u_end:\n", loop_count);
}

static void emit_flush(arc_type type, string *dst, uint16_t *outs)
{
    if (*outs == 0) return;

    switch (type)
    {
        case ARC_X86_64_LINUX: {
            string *count_str = string_create(3);
            string_append_format(count_str, "$%u", *outs);
            emit_syscall(type, dst, BF_CALL_WRITE, "$1", "%rdx", string_get_raw(count_str));
            *outs = 0;
            string_append_string(dst, "\txorq %rcx, %rcx\n");
            string_destroy(count_str);
            break;
        }

        case ARC_X86_LINUX: {
            string *count_str = string_create(3);
            string_append_format(count_str, "$%u", *outs);
            emit_syscall(type, dst, BF_CALL_WRITE, "$1", NULL, string_get_raw(count_str)); // Still figuring out what register to use for that
            *outs = 0;
            string_append_string(dst, "\txorl %ecx, %ecx\n");
            string_destroy(count_str);
            break;
        }
    }
}

static void emit_out_op(arc_type type, string *dst, uint16_t *outs)
{
    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (*outs == 20)
            {
                emit_flush(type, dst, outs);
            } else
            {
                string_append_string(dst, "\tmovb (%rbx), %al\n");
                string_append_string(dst, "\tmovb %al, (%rdx, %rcx, 1)\n");
                string_append_string(dst, "\tincq %rcx\n");
                (*outs)++;
            }

            break;
        }

        case ARC_X86_LINUX: {
            if (*outs == 20)
            {
                emit_flush(type, dst, outs);
            } else
            {
                string_append_string(dst, "\tmovb (%edi), %al\n");
                string_append_string(dst, "\tmovb %al, (%rdx, %ecx, 1)\n");
                string_append_string(dst, "\tincl %ecx\n");
                (*outs)++;
            }

            break;
        }
    }
}

static void emit_in_op(arc_type type, string *dst, uint16_t *outs)
{
    switch (type)
    {
        case ARC_X86_64_LINUX: {
            if (*outs >= 1)
            {
                emit_flush(type, dst, outs);
            }

            emit_syscall(type, dst, BF_CALL_READ, "$0", "%rbx", "$1");
            break;
        }

        case ARC_X86_LINUX: {
            if (*outs >= 1)
            {
                emit_flush(type, dst, outs);
            }

            emit_syscall(type, dst, BF_CALL_READ, "$0", "%edi", "$1");
            break;
        }
    }
}

static void process_intructions(arc_type type, string *dst, dyn_array *operations)
{
    if (!dst) return;

    uint16_t loops = 0;
    uint16_t outs = 0;
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
            case IR_RLOOP: emit_loop_end(dst, loop_stack); break;
            case IR_IN: emit_in_op(type, dst, &outs); break;
            case IR_OUT: emit_out_op(type, dst, &outs); break;
            case IR_END: emit_flush(type, dst, &outs); emit_syscall(type, dst, BF_CALL_EXIT, "$0", NULL, NULL); break;
        }
    }
}

static void emit_tape_reg_init(string *text, arc_type type)
{
    switch (type)
    {
        case ARC_X86_64_LINUX: string_append_string(text, "\tlea tape(%rip), %rbx\n"); string_append_string(text, "\tlea print_buf(%rip), %rdx\n"); break;
        case ARC_X86_LINUX: string_append_string(text, "\tlea tape(%rip), %edi\n"); break;
    }
}
