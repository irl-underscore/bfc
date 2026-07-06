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

#include "../args.h"

#include "args_map.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * ===================================================
 * Custom flag fucntions
 * ==================================================
 */

void process_target_arc(char *arg, void *mem)
{
    if (strncmp(arg, "x86_64_linux", 30) == 0)
    {
        write_data(mem, (arc_type*)ARC_X86_64_LINUX, sizeof(arc_type));
    } else if (strncmp(arg, "x86_linux", 30) == 0)
    {
        write_data(mem, (arc_type*)ARC_X86_LINUX, sizeof(arc_type));
    } else
    {
        fprintf(stderr, "Error: Unkown arcitecture: '%s'\n", arg);
    }
}

static uint8_t is_num(char a)
{
    return (a <= '9' && a >= '0');
}

void process_num(char *arg, void *mem)
{
    uint32_t res = 0;
    uint8_t arg_len = (uint8_t)strnlen(arg, 5);
    for (uint8_t i = 0; i < arg_len; i++)
    {
        if (is_num(arg[i]))
        {
            res = (res * 10) + (arg[i] - '0');
        } else
        {
            fprintf(stderr, "Error: non number in '%s'\n", arg);
            return;
        }
    }

    write_data(mem, &res, sizeof(uint32_t));
}

/*
 * ==================================================
 * Flag functions
 * ==================================================
 */

void print_help(void)
{
    printf("brainfuck [flags] <source_file>\n");
    printf("-h --help\tprint help message\n");
    printf("-v --version\tprint version info\n");
    printf("-p --tape <size>\tchange tape size\n");
    printf("-t --target <arcitecture>\tchange target arcitecture (see below)\n");
    printf("-o --output <file_name>\tdeclare output file (default to a.s)\n");
    printf("supported arcitectures\n");
    printf("  linux x86_64 (x86_64_linux)\n");
}

void print_version(void)
{
    printf("barinfuck v%i.%i\n", VERSION_MAJOR, VERSION_MINOR);
    printf("Copyright 2026 irl-underscore\n");
    printf("Licensed under the Apache License Version 2.0\n");
}

flag map[] = {
    {'h', "help", FLAG_TYPE_FUNCTION, FLAG_POS_THIS, offsetof(compiler_options, help), NULL, print_help},
    {'v', "version", FLAG_TYPE_FUNCTION, FLAG_POS_THIS, offsetof(compiler_options, version), NULL, print_version},
    {'p', "tape", FLAG_TYPE_CUSTOM, FLAG_POS_NEXT, offsetof(compiler_options, tape_size), process_num, NULL},
    {'t', "target", FLAG_TYPE_CUSTOM, FLAG_POS_NEXT, offsetof(compiler_options, target), process_target_arc, NULL},
    {'o', "output", FLAG_TYPE_STRING, FLAG_POS_NEXT, offsetof(compiler_options, output), NULL, NULL},
};

size_t map_len = sizeof(map);
