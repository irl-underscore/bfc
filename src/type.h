#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>

typedef enum ir_type
{
    OP_INC,
    OP_DEC,
    OP_RSHIFT,
    OP_LSHIFT,
    OP_OUT,
    OP_IN,
    OP_LLOOP,
    OP_RLOOP
} ir_type;

typedef struct operation
{
    ir_type type;
    uint16_t count;
} operation;

typedef enum
{
    BF_CALL_EXIT = 0,
    BF_CALL_READ = 1,
    BF_CALL_WRITE = 2
} bf_call;

typedef enum
{
    ARC_X86_LINUX,
    ARC_X86_64_LINUX
} ArcType;

#endif
