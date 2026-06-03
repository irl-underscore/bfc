#ifndef TYPE_H
#define TYPE_H
#include <stdint.h>

typedef unsigned char byte;

typedef enum ir_type
{
    IR_INC,
    IR_DEC,
    IR_CLEAR,
    IR_RSHIFT,
    IR_LSHIFT,
    IR_OUT,
    IR_IN,
    IR_LLOOP,
    IR_RLOOP
} ir_type;

typedef struct ir_operation
{
    ir_type type;
    uint16_t count;
} ir_operation;

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
