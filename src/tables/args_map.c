#include "../args.h"

#include "args_map.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void process_target_arc(char *arg, void *mem, uint8_t size)
{
    if (strncmp(arg, "x86_64_linux", 30) == 0)
    {
        *(arc_type*)mem = (arc_type)ARC_X86_64_LINUX;
    } else if (strncmp(arg, "x86_linux", 30) == 0)
    {
        *(arc_type*)mem = (arc_type)ARC_X86_LINUX;
    } else
    {
        fprintf(stderr, "Error: Unkown arcitecture: '%s'\n", arg);
    }
}

flag map[] = {
    {'o', NULL, FLAG_TYPE_STRING, offsetof(compiler_options, output), NULL},
    {'h', "help", FLAG_TYPE_BOOL, offsetof(compiler_options, help), NULL},
    {'v', "version", FLAG_TYPE_BOOL, offsetof(compiler_options, version), NULL},
    {'t', "target", FLAG_TYPE_CUSTOM, offsetof(compiler_options, target), process_target_arc}
};

size_t map_len = sizeof(map);
