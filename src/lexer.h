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
