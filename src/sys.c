/*
 * Copyright 2026 irl-underscore
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
#include "error.h"

#include "tables/syscall/syscall_tables.h"
#include "tables/registers/register_tables.h"

#include <string.h>
#include <stdio.h>

static void emit_syscall(arc_type target, string *dst, bf_call call, char *arg0, char *arg1, char *arg2)
{
    const int64_t syscall = syscall_table[target][call] - 1;
    const char **registers = register_table[target];
    if (syscall == 0) string_append_format(dst, "\txorl %%%s, %%%s\n", registers[ASM_REG_CALL], registers[ASM_REG_CALL]);
    else string_append_format(dst, "\tmovl $%i, %%%s\n", syscall, registers[ASM_REG_CALL]);

    #define EMIT_ARG(arg_num) \
    if (!arg ## arg_num) return; \
    if (strncmp(arg ## arg_num, "$0", 3) == 0) string_append_format(dst, "\txorl %%%s, %%%s", registers[ASM_REG_PARAM ## arg_num], registers[ASM_REG_PARAM ## arg_num]); \
    else string_append_format(dst, "\tmovl %s, %%%s\n", arg ## arg_num, registers[ASM_REG_PARAM ## arg_num]); \

    EMIT_ARG(0)
    EMIT_ARG(1)
    EMIT_ARG(2)

    #undef EMIT_ARG
}

static void emit_inc_op(arc_type type, string *dst, uint16_t count)
{
    if (count == 1) string_append_format(dst, "\tincb (%%%s)\n", register_table[type][ASM_REG_TAPE_BASE]);
    else if (count > 1) string_append_format(dst, "\taddb $%u, (%%%s)\n", count, register_table[type][ASM_REG_TAPE_BASE]);
}

static void emit_dec_op(arc_type type, string *dst, uint16_t count)
{
    if (count == 1) string_append_format(dst, "\tdecb (%%%s)\n", register_table[type][ASM_REG_TAPE_BASE]);
    else if (count > 1) string_append_format(dst, "\tsubb $%u, (%%%s)\n", count, register_table[type][ASM_REG_TAPE_BASE]);
}

static void emit_clear_op(arc_type type, string *dst)
{
    string_append_format(dst, "\txorq %%%s, %%%s\n", register_table[type][ASM_REG_TAPE_BASE], register_table[type][ASM_REG_TAPE_BASE]);
}

static void emit_lshift_op(arc_type type, string *dst, uint16_t count)
{
    if (count == 1) string_append_format(dst, "\tdecl %%%s\n", register_table[type][ASM_REG_TAPE_BASE]);
    else if (count > 1) string_append_format(dst, "\tsubl $%u, %%%s\n", count, register_table[type][ASM_REG_TAPE_BASE]);
}

static void emit_rshift_op(arc_type type, string *dst, uint16_t count)
{
    if (count == 1) string_append_format(dst, "\tincq %%%s\n", register_table[type][ASM_REG_TAPE_BASE]);
    else if (count > 1) string_append_format(dst, "\taddq $%u, %%%s\n", count, register_table[type][ASM_REG_TAPE_BASE]);
}

static void emit_loop_start(arc_type type, string *dst, uint16_t *loop_count, stack *loop_stack)
{
    string_append_format(dst, ".L%u_start:\n", ++(*loop_count));
    push(loop_stack, *loop_count);
    string_append_format(dst, "\tcmpb $0, (%%%s)\n", register_table[type][ASM_REG_TAPE_BASE]);
    string_append_format(dst, "\tje .L%u_end\n", *loop_count);
}

static void emit_loop_end(string *dst, stack *loop_stack)
{
    uint16_t loop_count = pop(loop_stack);
    string_append_format(dst, "\tjmp .L%u_start\n", loop_count);
    string_append_format(dst, ".L%u_end:\n", loop_count);
}

static void emit_flush(arc_type type, string *dst, uint16_t *outs)
{
    if (*outs == 0) return;

    char count_str[7];
    sprintf(count_str, "$%u", *outs);
    char print_buf[5];
    sprintf(print_buf, "%%%s", register_table[type][ASM_REG_TAPE_BASE]);
    emit_syscall(type, dst, BF_CALL_WRITE, "$1", print_buf, count_str);
    *outs = 0;
    string_append_format(dst, "\txorq %%%s, %%%s\n", register_table[type][ASM_REG_COUNT], register_table[type][ASM_REG_COUNT]);
}

static void emit_out_op(arc_type type, string *dst, uint16_t *outs)
{
    string_append_format(dst, "\tmovb (%%%s), %%%s\n", register_table[type][ASM_REG_TAPE_BASE], register_table[type][ASM_REG_BYTE_BUFFER]);
    string_append_format(dst, "\tmovb %%%s, (%%%s, %%%s, 1)\n", register_table[type][ASM_REG_BYTE_BUFFER], register_table[type][ASM_REG_PRINT_BUF], register_table[type][ASM_REG_COUNT]);
    string_append_format(dst, "\tincq %%%s\n", register_table[type][ASM_REG_COUNT]);
    (*outs)++;
    if (*outs == 20)
    {
        emit_flush(type, dst, outs);
    }
}

static void emit_in_op(arc_type type, string *dst, uint16_t *outs)
{
    if (*outs >= 1)
    {
        emit_flush(type, dst, outs);
    }

    char base_reg[7];
    sprintf(base_reg, "(%%%s)", register_table[type][ASM_REG_TAPE_BASE]);
    emit_syscall(type, dst, BF_CALL_READ, "$0", base_reg, "$1");
}

static res process_intructions(arc_type type, string *dst, dyn_array *operations)
{
    uint16_t loops = 0;
    uint16_t outs = 0;
    stack *loop_stack = stack_create();
    if (!loop_stack)
    {
        emit_err("Unable to allocate memory for stack", NULL, 0, 0, G1001, ERROR);
        return ERR;
    }

    size_t arr_size = dyn_array_get_size(operations);
    string_append_format(dst, "\txorl %%%s, %%%s\n", register_table[type][ASM_REG_COUNT], register_table[type][ASM_REG_COUNT]);
    for (size_t i = 0; i < arr_size; i++)
    {
        ir_operation op = *(ir_operation*)dyn_array_get(operations, i);
        switch (op.type)
        {
            case IR_INC: emit_inc_op(type, dst, op.count);  break;
            case IR_DEC: emit_dec_op(type, dst, op.count); break;
            case IR_CLEAR: emit_clear_op(type, dst);  break;
            case IR_LSHIFT: emit_lshift_op(type, dst, op.count); break;
            case IR_RSHIFT: emit_rshift_op(type, dst, op.count); break;
            case IR_LLOOP: emit_loop_start(type, dst, &loops, loop_stack); break;
            case IR_RLOOP: emit_loop_end(dst, loop_stack); break;
            case IR_IN: emit_in_op(type, dst, &outs); break;
            case IR_OUT: emit_out_op(type, dst, &outs); break;
            case IR_END: emit_flush(type, dst, &outs); emit_syscall(type, dst, BF_CALL_EXIT, "$0", NULL, NULL); break;
            case IR_NOP: break;
        }
    }

    return OK;
}

static void emit_tape_reg_init(string *text, arc_type type)
{
    string_append_format(text, "\tlea tape(%%rip), %%%s\n", register_table[type][ASM_REG_TAPE_BASE]); string_append_format(text, "\tlea print_buf(%%rip), %%%s\n", register_table[type][ASM_REG_TAPE_BASE]);
}
