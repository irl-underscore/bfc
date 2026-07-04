#include "args.h"

#include "tables/args_map.h"
#include "type.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CUSTOM_FLAG_MEMORY 8

static uint8_t execute_flag(flag f, compiler_options *options, char *next, void *mem)
{
    switch (f.type)
    {
        case FLAG_TYPE_BOOL: {
            *((byte*)options + f.offset) = 1;
            return 1;
        }

        case FLAG_TYPE_STRING: {
            if (next)
            {
                *(char**)((byte*)options + f.offset) = next;

            } else
            {
                fprintf(stderr, "Error: Flag is missing parameter\n");
            }

            return 2;
        }
        case FLAG_TYPE_CUSTOM: {
            f.process(next, mem, MAX_CUSTOM_FLAG_MEMORY);
            *((byte*)options + f.offset) = *(byte*)mem;
            return 2;
        }
    }
}

static uint8_t check_short(char *argv[], int32_t argc, compiler_options *options, size_t *i, int32_t current_idx, void *mem)
{
    char *arg = argv[current_idx] + 1;
    uint8_t res = 1;
    if (arg[0] == map[*i].short_form && arg[1] == '\0')
    {
        res = execute_flag(map[*i], options, argv[current_idx + 1], mem);
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
    size_t flag_len = strnlen(map[*i].long_form, 6);
    uint8_t res = 1;
    if (map[*i].long_form && strncmp(arg, map[*i].long_form, flag_len) == 0)
    {
        res = execute_flag(map[*i], options, argv[current_idx + 1], mem);
        *i = 0;
    } else
    {
        (*i)++;
        if (*i < map_len) return check_long(argv, argc, options, i, current_idx, mem);
    }

    return res;
}

void process_args(char *argv[], int32_t argc, compiler_options *options)
{
    size_t i = 0;
    int32_t current_idx = 1;
    void *mem = malloc(MAX_CUSTOM_FLAG_MEMORY);
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
            size_t len = strnlen(arg, 15);
            if (strncmp(arg + len - 3, ".bf", 3) == 0)
            {
                options->input = arg;
            }
        }

        if (i >= map_len)
        {
            fprintf(stderr, "Error: Unkown flag: %s\n", arg);
            break;
        }

        current_idx += increment;
        if (current_idx >= argc) break;
    }

    free(mem);
}
