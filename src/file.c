#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

file_buf *file_buf_load(const char *path)
{
    if (!path) return NULL;

    FILE *file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (length < 0) {
        fclose(file);
        return NULL;
    }

    file_buf *buf = malloc(sizeof(file_buf));
    if (!buf) {
        fclose(file);
        return NULL;
    }

    buf->size = (size_t)length;
    buf->data = malloc(buf->size + 1);
    if (!buf->data) {
        fclose(file);
        free(buf);
        return NULL;
    }

    size_t read_count = fread(buf->data, 1, buf->size, file);
    buf->data[read_count] = '\0';

    fclose(file);
    return buf;
}

void file_buf_destroy(file_buf *buf)
{
    if (!buf) return;

    free(buf->data);
    free(buf);
}

void pitch_code(const char *code, const char *output_file)
{
    if (!code || !output_file) return;

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Error opening output");
        return;
    }

    fputs(code, out);
    fclose(out);
}
