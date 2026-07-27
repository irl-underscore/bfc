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

#include "args.h"

#include "tables/args_map.h"
#include "type.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>

#define MAX_CUSTOM_FLAG_MEMORY 32
#define MAX_SRC_FILE_LEN 64
#define MAX_LONG_FLAG_LEN 10

#define ARGS_FAILURE 255

#define ALIGN_UP(align, addr) ((addr + (align - 1)) & ~(align - 1))

static uint8_t get_offset(void *mem)
{
    uint8_t offset;
    memcpy(&offset, (byte*)mem + 1, 1);
    return offset;
}

static uint8_t get_size(void *mem)
{
    uint8_t size;
    memcpy(&size, mem, 1);
    return size;
}

static uint8_t get_err(void *mem)
{
    uint8_t err;
    memcpy(&err, (byte*)mem + 2, 1);
    return err;
}

void write_data(void *mem, void *data, uint8_t obj_size)
{
    uintptr_t mem_addr = (uintptr_t)mem;
    uintptr_t aligned_mem = ALIGN_UP(8, mem_addr + 3);
    uint8_t offset = aligned_mem - mem_addr;
    memset(mem, obj_size, 1);
    memset((byte*)mem + 1, offset, 1);
    uint8_t ok = 0;
    memset((byte*)mem + 2, ok, 1);
    mem = (byte*)mem + 3 + offset;
    memcpy(mem, data, MAX_CUSTOM_FLAG_MEMORY - offset);
}

void write_err(void *mem)
{
    uint8_t err = 0xFF;
    memset((byte*)mem + 2, err, 1);
}

static uint8_t execute_flag(flag f, compiler_options *options, char *this, char *next, void *mem)
{
    uint8_t res = 1;
    switch (f.type)
    {
        case FLAG_TYPE_BOOL: {
            *((byte*)options + f.offset) = 1;
            break;
        }

        case FLAG_TYPE_STRING: {
            if (f.pos == FLAG_POS_NEXT && next)
            {
                *(char**)((byte*)options + f.offset) = next;
                res = 2;

            } else if (f.pos == FLAG_POS_THIS && this)
            {
                *(char**)((byte*)options + f.offset) = this;
            } else
            {
                emit_err(NULL, NULL, 0, 0, G2001, ERROR);
                return ARGS_FAILURE;
            }

            break;
        }

        case FLAG_TYPE_CUSTOM: {
            if (f.pos == FLAG_POS_NEXT && next)
            {
                f.custom_func(next, mem);
                res = 2;
            } else if (f.pos == FLAG_POS_THIS && this)
            {
                f.custom_func(this, mem);
            }

            uint8_t err = get_err(mem);
            if (err == 0xFF)
            {
                return ARGS_FAILURE;
            }

            uint8_t size = get_size(mem);
            uint8_t offset = get_offset(mem);
            mem = (byte*)mem + 3 + offset;
            memcpy((byte*)options + f.offset, mem, size); break;
            memset(mem, 0, 3 + MAX_CUSTOM_FLAG_MEMORY);
            break;
        }

        case FLAG_TYPE_FUNCTION: {
            if (f.func)
            {
                f.func();
            }

            break;
        }
    }

    return res;
}

static uint8_t check_short(char *argv[], int32_t argc, compiler_options *options, size_t *i, int32_t current_idx, void *mem)
{
    char *arg = argv[current_idx] + 1;
    uint8_t res = 1;
    if (arg[0] == map[*i].short_form && arg[1] == '\0')
    {
        res = execute_flag(map[*i], options, arg + 1, argv[current_idx + 1], mem);
        *i = 0;
    } else
    {
        (*i)++;
        if (*i < map_len) return check_short(argv, argc, options, i, current_idx, mem);
    }

    return res;
}

static uint8_t check_long(char *argv[], int32_t argc, compiler_options *options, size_t *i, int32_t current_idx, void *mem)
{
    char *arg = argv[current_idx] + 2;
    uint8_t res = 1;
    if (map[*i].long_form)
    {
        size_t flag_len = strnlen(map[*i].long_form, MAX_LONG_FLAG_LEN);
        if (map[*i].long_form && strncmp(arg, map[*i].long_form, flag_len) == 0)
        {
            res = execute_flag(map[*i], options, arg + flag_len, argv[current_idx + 1], mem);
            *i = 0;
            goto ret;
        } else
        {
            goto call_self;
        }
    } else
    {
        goto call_self;
    }

    ret:
    return res;

    call_self:
    (*i)++;
    if (*i < map_len) return check_long(argv, argc, options, i, current_idx, mem);
    else goto ret;
}

res process_args(char *argv[], int32_t argc, compiler_options *options)
{
    size_t i = 0;
    int32_t current_idx = 1;
    void *mem = malloc(3 + MAX_CUSTOM_FLAG_MEMORY);
    if (!mem)
    {
        emit_err(NULL, NULL, 0, 0, G3001, ERROR);
        return ERR;
    }

    memset(mem, 0, 3 + MAX_CUSTOM_FLAG_MEMORY);
    while (1)
    {
        uint8_t increment = 1;
        char *arg = argv[current_idx];
        if (arg[1] == '-')
        {
            increment = check_long(argv, argc, options, &i, current_idx, mem);
        } else if (arg[0] == '-')
        {
            increment = check_short(argv, argc, options, &i, current_idx, mem);
        } else
        {
            size_t len = strnlen(arg, MAX_SRC_FILE_LEN);
            if (strncmp(arg + (len - 3), ".bf", 4) == 0 ||strncmp(arg +(len - 2), ".b", 3) == 0)
            {
                options->input = arg;
            }
        }

        if (i >= map_len)
        {
            emit_err(NULL, NULL, 0, 0, G2001, ERROR);
            free(mem);
            return ERR;
        }

        if (increment == ARGS_FAILURE)
        {
            free(mem);
            return ERR;
        }

        current_idx += increment;
        if (current_idx >= argc) break;
    }

    free(mem);
    return OK;
}
