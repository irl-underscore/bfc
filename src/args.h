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
