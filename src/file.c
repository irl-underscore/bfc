#include "file.h"

#include <stdlib.h>
#include <stdio.h>

file_buf *file_buf_load(char *path)
{
    if (!path) return NULL;

    file_buf *buf = malloc(sizeof(file_buf));
    if (!buf) return NULL;

    FILE *file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    buf->size = ftell(file);
    fseek(file, 0, SEEK_SET);
    buf->data = malloc(buf->size + 1);
    if (!buf->data)
    {
        fclose(file);
        return NULL;
    }

    fread(buf->data, 1, buf->size, file);
    buf->data[buf->size] = '\0';
    fclose(file);
    return buf;
}

void file_buf_destroy(file_buf *buf)
{
    if (!buf) return;

    if (buf->data) free(buf->data);

    free(buf);
}
