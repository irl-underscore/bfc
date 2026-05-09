#ifndef LEXER_H
#define LEXER_H
#include "file.h"
#include "dyn_array.h"

#include <stdint.h>

#define INC_OPERATION "\tincb (%rbx)\n"
#define DEC_OPERATION "\tdecb (%rbx)\n"
#define RSHIFT_OPERATION "\tincq %rbx\n"
#define LSHIFT_OPERATION "\tdecq %rbx\n"

#define ADD_OPERATION_FIRST "\taddb $"
#define ADD_OPERATION_SECOND ", (%rbx)\n"
#define SUB_OPERATION_FIRST "\tsubb $"
#define SUB_OPERATION_SECOND ", (%rbx)\n"
#define LSHIFT_OPERATION_FIRST "\tsubq $"
#define LSHIFT_OPERATION_SECOND ", %rbx\n"
#define RSHIFT_OPERATION_FIRST "\taddq $"
#define RSHIFT_OPERATION_SECOND ", %rbx\n"

#define IN_OPERATION "\tmovq $0, %rax\n\tmovq $0, %rdi\n\tmovq %rbx, %rsi\n\tmovq $1, %rdi\n\tsyscall\n "
#define OUT_OPERATION "\tmovq $1, %rax\n\tmovq $1, %rdi\n\tmovq %rbx, %rsi\n\tmovq $1, %rdx\n\tsyscall\n"

typedef enum op_type
{
    OP_INC,
    OP_DEC,
    OP_RSHIFT,
    OP_LSHIFT,
    OP_OUT,
    OP_IN
} op_type;

typedef struct operation
{
    op_type type;
    uint16_t count;
} operation;

dyn_array *parse_file(file_buf *buf);
dyn_array *apply_o1_optimization(dyn_array *operations);
char *assemble(dyn_array *operations);
void pitch_template(char *assembled_code, char *template_file, char *output_file);

#endif /* LEXER_H */
