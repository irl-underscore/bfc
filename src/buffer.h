#ifndef FILE_H
#define FILE_H
#include "error.h"

#include <stdint.h>

typedef struct file_buf
{
    char *data;
    size_t size;
} file_buf;

file_buf *file_buf_load(char *path);
void file_buf_destroy(file_buf *buf);

#endif
