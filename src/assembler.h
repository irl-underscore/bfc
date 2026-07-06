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

#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "dyn_array.h"

typedef enum
{
    ARC_X86_LINUX,
    ARC_X86_64_LINUX
} arc_type;

typedef struct assemble_ctx assemble_ctx;

assemble_ctx *ctx_init(uint32_t tape_size);
void ctx_process(assemble_ctx *ctx, dyn_array *instructions, arc_type type);
char *ctx_assemble(assemble_ctx *ctx);
void ctx_destroy(assemble_ctx *ctx);

#endif /* ASSEMBLER_H  */
