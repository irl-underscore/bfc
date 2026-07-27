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

#ifndef IR_MAP_H
#define IR_MAP_H

#include <stdint.h>

typedef enum ir_type
{
    IR_NOP,
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

typedef struct ir_def_op
{
    char sign;
    ir_type type;
    ir_type match;
} ir_def_op;

extern const ir_def_op ir_operation_map[];

#endif /* IR_MAP_H */
