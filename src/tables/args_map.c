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
    printf("Still working on that one :)\n");
}

void print_version(void)
{
    printf("Not done yet :)\n");
}

flag map[] = {
    {'h', "help", FLAG_TYPE_FUNCTION, FLAG_POS_THIS, offsetof(compiler_options, help), NULL, print_help},
    {'v', "version", FLAG_TYPE_FUNCTION, FLAG_POS_THIS, offsetof(compiler_options, version), NULL, print_version},
    {'p', "tape", FLAG_TYPE_CUSTOM, FLAG_POS_NEXT, offsetof(compiler_options, tape_size), process_num, NULL},
    {'t', "target", FLAG_TYPE_CUSTOM, FLAG_POS_NEXT, offsetof(compiler_options, target), process_target_arc, NULL},
    {'o', NULL, FLAG_TYPE_STRING, FLAG_POS_NEXT, offsetof(compiler_options, output), NULL, NULL},
};

size_t map_len = sizeof(map);
