#ifndef ARGS_H
#define ARGS_H
#include <stdint.h>

typedef struct compiler_options
{
    uint8_t debug;
    char *output;
    char *input;
} compiler_options;

void process_args(char *argv[], int32_t argc, compiler_options *options);

#endif
