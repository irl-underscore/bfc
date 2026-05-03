#ifndef FILE_H
#define FILE_H
#include "error.h"

#include <stdint.h>

typedef struct file_buf
{
    char *data;
    size_t size;
} file_buf;

file_buf *load_file_buf(char *path);
void destroy_file_buf(file_buf *buf);

#endif
