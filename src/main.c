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

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#include "lexer.h"
#include "assembler.h"
#include "file.h"
#include "args.h"

#include <stdlib.h>

compiler_options opt = {
    .help = 0,
    .version = 0,
    .tape_size = 1050,
    .target = ARC_X86_64_LINUX,
    .output = "a.s",
    .input = NULL,
};

int main(int argc, char *argv[])
{
    process_args(argv, argc, &opt);
    if (opt.input)
    {
        file_buf *buf = file_buf_load(opt.input);
        if (!buf) return 1;

        dyn_array *operations = parse_file(buf);
        if (!operations) return 1;

        file_buf_destroy(buf);
        dyn_array *optimized = post_process(operations);
        assemble_ctx *ctx = ctx_init(opt.tape_size);
        ctx_process(ctx, optimized, opt.target);
        dyn_array_destroy(optimized);
        char *code = ctx_assemble(ctx);
        ctx_destroy(ctx);
        pitch_code(code, opt.output);
        free(code);
    }

    return 0;
}
