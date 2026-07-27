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

#include "brainfuck.h"

#include "file.h"
#include "lexer.h"

#include <stdlib.h>

#define ASSERT_TO(cond, to) if (!(cond)) \
{ \
    r = ERR; \
    goto to; \
} \

res process(compiler_options defaults, char *argv[], uint32_t argc)
{
    compiler_options opts = defaults;
    res r = OK;
    res args = process_args(argv, argc, &opts);
    ASSERT_TO(args == OK, end)

    if (!opts.input) return OK;

    file_buf *buf = file_buf_load(opts.input);
    ASSERT_TO(buf, end)

    dyn_array *operations = parse_file(buf);
    ASSERT_TO(operations, destroy_file_buf)

    dyn_array *temp = post_process(operations);
    ASSERT_TO(temp, destroy_operations)

    operations = temp;
    assemble_ctx *ctx = ctx_init(opts.tape_size);
    ASSERT_TO(ctx, destroy_operations)

    res process = ctx_process(ctx, operations, opts.target);
    ASSERT_TO(process == OK, destroy_ctx)

    char *code = ctx_assemble(ctx);
    ASSERT_TO(code, destroy_ctx)

    res pitch = pitch_code(code, opts.output);
    ASSERT_TO(pitch == OK, destroy_code)

    destroy_code:
    free(code);

    destroy_ctx:
    ctx_destroy(ctx);

    destroy_operations:
    dyn_array_destroy(operations);

    destroy_file_buf:
    file_buf_destroy(buf);

    end:
    return r;
}

#undef ASSERT_TO
