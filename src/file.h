/*
 * Copyright (c) 2026 irl-underscore
 * SPDX-License-Identifier: Apache-2.0
 * This file is part of brainfuck and is licensed under the Apache 2.0 License.
 */

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
void pitch_code(const char *code, const char *output_file);

#endif
