#ifndef LEXER_H
#define LEXER_H
#include "dyn_array.h"
#include "file.h"

#include <stdint.h>

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

dyn_array *apply_o1_optimization(dyn_array *operations);
dyn_array *parse_file(file_buf *buf);

#endif /* LEXER_H */
