#ifndef LEXER_H
#define LEXER_H
#include "buffer.h"
#include "dyn_array.h"

#include <stdint.h>

typedef enum op_type
{
    OP_INC,
    OP_DEC,
    OP_RSHIFT,
    OP_LSHIFT,
    OP_OUT,
    OP_IN,
    OP_LLOOP,
    OP_RLOOP
} op_type;

typedef struct operation
{
    op_type type;
    uint16_t count;
} operation;

dyn_array *parse_file(file_buf *buf);
dyn_array *apply_o1_optimization(dyn_array *operations);
void pitch_template(const char *assembled_code, const char *template_file, const char *output_file);

#endif /* LEXER_H */
