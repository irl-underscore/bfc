#ifndef LEXER_H
#define LEXER_H
#include "file.h"
#include "dyn_array.h"

#include <stdint.h>

#define O0_INC_OPERATION "\tincb (%rbx)\n"
#define O0_DEC_OPERATION "\tdecb (%rbx)\n"
#define O0_RSHIFT_OPERATION "\tincq %rbx\n"
#define O0_LSHIFT_OPERATION "\tdecq %rbx\n"

#define O1_INC_OPERATION(count) "\tadd " #count ", (%rbx)\n"

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

dyn_array *parse_file(file_buf *buf);
char *assemble(dyn_array *operations, uint8_t optimization);
void pitch_template(char *assembled_code, char *template_file, char *output_file);

#endif /* LEXER_H */
