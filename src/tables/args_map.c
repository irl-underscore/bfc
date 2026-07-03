#include "../args.h"

#include "args_map.h"

#include <stddef.h>

args_map map[] = {
    {'d', "debug", FLAG_TYPE_BOOL, .contents = {.offset =  offsetof(compiler_options, debug)}},
    {'o', NULL, FLAG_TYPE_STRING, .contents = {.offset = offsetof(compiler_options, output)}}
};

size_t map_len = sizeof(map);
