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
    }

    dyn_array *operations = parse_file(buf);
    char *code = assemble(operations, 0);
    pitch_template(code, "template.s", "out.s");
    return 0;
}
