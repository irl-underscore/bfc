#include "../args.h"

#include "args_map.h"

#include <stddef.h>

args_map map[] = {
    {'o', NULL, FLAG_TYPE_STRING, .contents = {.offset = offsetof(compiler_options, output)}},
    {'h', "help", FLAG_TYPE_BOOL, .contents = {.offset = offsetof(compiler_options, help)}}
};

size_t map_len = sizeof(map);
