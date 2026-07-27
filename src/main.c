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

#include "assembler.h"
#include "args.h"
#include "error.h"
#include "brainfuck.h"

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
    init_diags();
    res r = process(opt, argv, argc);
    if (r != OK)
    {
        throw_errs();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
