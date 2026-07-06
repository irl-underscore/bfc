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

#ifndef ARGS_H
#define ARGS_H
#include "assembler.h"

#include <stdint.h>

typedef struct compiler_options
{
    uint8_t help;
    uint8_t version;
    uint32_t tape_size;
    arc_type target;
    char *output;
    char *input;
} compiler_options;

void write_data(void *mem, void *inst, uint8_t obj_size);
void process_args(char *argv[], int32_t argc, compiler_options *options);

#endif
