#include <stdio.h>
#include "file.h"

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

    printf("%s", buf->data);

    return 0;
}
