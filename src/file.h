#ifndef FILE_H
#define FILE_H
#include "dyn_array.h"

#include <stdint.h>

typedef struct file_buf
{
    char *data;
    size_t size;
} file_buf;

file_buf *file_buf_load(const char *path);
void file_buf_destroy(file_buf *buf);
dyn_array *parse_file(file_buf *buf);
void pitch_template(const char *assembled_code, const char *template_file, const char *output_file);

#endif
