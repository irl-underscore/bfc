/*
 * Copyright 2016 irl-underscore
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
