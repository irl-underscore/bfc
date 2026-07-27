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

#include "ir_map.h"

#include <stddef.h>

#define OP(c, type, match) [c] = {c, type, match},

const ir_def_op ir_operation_map[] = {
    OP('+', IR_INC, 0)
    OP('-', IR_DEC, 0)
    OP('>', IR_RSHIFT, 0)
    OP('<', IR_LSHIFT, 0)
    OP('[', IR_LLOOP, IR_RLOOP)
    OP(']', IR_RLOOP, 0)
    OP('.', IR_OUT, 0)
    OP(',', IR_IN, 0)
};

#undef OP
