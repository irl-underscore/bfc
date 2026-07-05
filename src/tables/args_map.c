#include "../args.h"

#include "args_map.h"
#include "../type.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void process_target_arc(char *arg, void *mem)
{
    if (strncmp(arg, "x86_64_linux", 30) == 0)
    {
        *(arc_type*)((byte*)mem + sizeof(flag_mem_instruction)) = (arc_type)ARC_X86_64_LINUX;
    } else if (strncmp(arg, "x86_linux", 30) == 0)
    {
        *(arc_type*)((byte*)mem + sizeof(flag_mem_instruction)) = (arc_type)ARC_X86_LINUX;
    } else
    {
        fprintf(stderr, "Error: Unkown arcitecture: '%s'\n", arg);
    }
}

flag map[] = {
    {'o', NULL, FLAG_TYPE_STRING, FLAG_POS_NEXT, offsetof(compiler_options, output), NULL},
    {'h', "help", FLAG_TYPE_BOOL, FLAG_POS_THIS, offsetof(compiler_options, help), NULL},
    {'v', "version", FLAG_TYPE_BOOL, FLAG_POS_THIS, offsetof(compiler_options, version), NULL},
    {'t', "target", FLAG_TYPE_CUSTOM, FLAG_POS_NEXT, offsetof(compiler_options, target), process_target_arc}
};

size_t map_len = sizeof(map);
