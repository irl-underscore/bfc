/*
 * Copyright (c) 2026 irl-underscore
 * SPDX-License-Identifier: Apache-2.0
 * This file is part of brainfuck and is licensed under the Apache 2.0 License.
 */

#ifndef LEXER_H
#define LEXER_H
#include "dyn_array.h"
#include "file.h"

#include <stdint.h>

typedef enum ir_type
{
    IR_INC,
    IR_DEC,
    IR_CLEAR,
    IR_RSHIFT,
    IR_LSHIFT,
    IR_OUT,
    IR_IN,
    IR_LLOOP,
    IR_RLOOP,
    IR_END
} ir_type;

typedef struct ir_operation
{
    ir_type type;
    uint16_t count;
} ir_operation;

dyn_array *parse_file(file_buf *buf);
dyn_array *post_process(dyn_array *operations);

#endif /* LEXER_H */
