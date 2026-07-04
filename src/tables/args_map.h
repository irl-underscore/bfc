#ifndef ARGS_MAP_H
#define ARGS_MAP_H
#include <stdint.h>

typedef enum flag_type {
    FLAG_TYPE_BOOL,
    FLAG_TYPE_STRING,
    FLAG_TYPE_CUSTOM
} flag_type;

typedef struct flag
{
    char short_form;
    char *long_form;
    flag_type type;
    uint8_t offset;
    void (*process)(char *, void *, uint8_t);
} flag;

extern flag map[];
extern size_t map_len;

#endif /* ARGS_MAP_H */
