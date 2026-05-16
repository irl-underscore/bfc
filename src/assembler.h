#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "syscall.h"

void emit_inc_op(ArcType type, string *dst, uint16_t count);
void emit_dec_op(ArcType type, string *dst, uint16_t count);
void emit_lshift_op(ArcType type, string *dst, uint16_t count);
void emit_rshift_op(ArcType type, string *dst, uint16_t count);
void emit_loop_start(ArcType type, string *dst, uint16_t *loop_count);
void emit_loop_end(ArcType type, string *dst, uint16_t *loop_count);

#endif /* ASSEMBLER_H  */
