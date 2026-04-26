#ifndef LEXER_H
#define LEXER_H
#include <stdint.h>

typedef enum op_type
{
    OP_INC,
    OP_DEC,
    OP_RSHIFT,
    OP_LSHIFT
} op_type;

typedef struct operation_o0 // 0 optimization instructions
{
    op_type type;
} operation_o0;

typedef struct operation_01 // 1 optimization instructions
{
    op_type type;
    uint16_t count;
} operation_o1;



#endif /* LEXER_H */
