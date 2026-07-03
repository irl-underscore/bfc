#include "args.h"

#include "tables/args_map.h"
#include "type.h"

#include <string.h>
#include <stdio.h>

static void execute_flag(args_map flag, compiler_options *options, char *next)
{
    switch (flag.type)
    {
        case FLAG_TYPE_BOOL: {
            *((byte*)options + flag.contents.offset) = 1;
            break;
        }

        case FLAG_TYPE_STRING: {
            if (next)
            {
                *(char**)((byte*)options + flag.contents.offset) = next;
            } else
            {
                fprintf(stderr, "Error: Flag is missing parameter");
            }

            break;
        }
        case FLAG_TYPE_FUNCTION: {
            flag.contents.func();
            break;
        }
    }
}

static void check_short(char *argv[], int32_t argc, compiler_options *options, size_t *i, const int32_t current_idx)
{
    char *arg = argv[current_idx] + 1;
    if (arg[0] == map[*i].short_form && arg[1] == '\0')
    {
        execute_flag(map[*i], options, argv[current_idx + 1]);
        *i = 0;
    } else
    {
        (*i)++;
        if (*i < map_len) check_short(argv, argc, options, i, current_idx);
    }
}

static void check_long(char *argv[], int32_t argc, compiler_options *options, size_t *i, const int32_t current_idx)
{
    char *arg = argv[current_idx] + 2;
    size_t flag_len = strnlen(map[*i].long_form, 6);
    if (map[*i].long_form && strncmp(arg, map[*i].long_form, flag_len) == 0)
    {
        execute_flag(map[*i], options, argv[current_idx + 1]);
        *i = 0;
    } else
    {
        (*i)++;
        if (*i < map_len) check_long(argv, argc, options, i, current_idx);
    }
}

void process_args(char *argv[], int32_t argc, compiler_options *options)
{
    size_t i = 0;
    int32_t current_idx = 1;
    while (1)
    {
        char *arg = argv[current_idx];
        if (arg[1] == '-')
        {
            check_long(argv, argc, options, &i, current_idx);
        } else if (arg[0] == '-')
        {
            check_short(argv, argc, options, &i, current_idx);
        } else
        {
            current_idx++; // handeled later
            continue;
        }

        if (i >= map_len)
        {
            fprintf(stderr, "Error: Unkown flag: %s", arg);
            break;
        }

        current_idx++;
        if (current_idx >= argc) break;
    }
}
