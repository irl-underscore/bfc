#include <stdio.h>
#include "file.h"
#include "dyn_array.h"
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    file_buf *buf = load_file_buf(argv[1]);
    if (!buf)
    {
        fprintf(stderr, "Error: Couldn't load file into buffer");
        return 1;
    }

    printf("File buffer: %s", buf->data);
    dyn_array *operations = parse_file(buf);
    if (!operations)
    {
        fprintf(stderr, "Error: failes to parse file.");
        return 1;
    }

    printf("Tokens: ");
    for (int i = 0; i < dyn_array_get_size(operations); i++)
    {
        operation op = *(operation*)dyn_array_get(operations, i);
        switch (op.type)
        {
            case OP_DEC: printf("'-' "); break;
            case OP_INC: printf("'+' "); break;
            case OP_LSHIFT: printf("'<' "); break;
            case OP_RSHIFT: printf("'>' "); break;
        }
    }

    printf("\n");
    dyn_array *optimized = apply_o1_optimization(operations);
    if (!optimized)
    {
        fprintf(stderr, "Error: couldn't apply optimization.");
        return 2;
    }

    printf("Tokens: ");
    for (int i = 0; i < dyn_array_get_size(optimized); i++)
    {
        operation op = *(operation*)dyn_array_get(optimized, i);
        switch (op.type)
        {
            case OP_DEC: printf("'-'(%i) ", op.count); break;
            case OP_INC: printf("'+'(%i) ", op.count); break;
            case OP_LSHIFT: printf("'<'(%i) ", op.count); break;
            case OP_RSHIFT: printf("'>'(%i) ", op.count); break;
        }
    }

    printf("\n");

    char *code = assemble(optimized);
    if (!code)
    {
        fprintf(stderr, "Error: Could't assemble instructions.");
    }

    pitch_template(code, "template.s", "out.s");
    dyn_array_destroy(optimized);
    dyn_array_destroy(operations);
    return 0;
}
