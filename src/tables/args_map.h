#ifndef ARGS_MAP_H
#define ARGS_MAP_H
#include <stdint.h>

typedef enum flag_type {
    FLAG_TYPE_BOOL,
    FLAG_TYPE_STRING,
    FLAG_TYPE_FUNCTION
} flag_type;

typedef struct args_map
{
    char short_form;
    char *long_form;
    flag_type type;
    union
    {
        void (*func)();
        uint8_t offset;
    } contents;
} args_map;

extern args_map map[];
extern size_t map_len;

#endif /* ARGS_MAP_H */
