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

#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

file_buf *file_buf_load(const char *in_path)
{
    if (!in_path) return NULL;

    FILE *in = fopen(in_path, "rb");
    if (!in)
    {
        fprintf(stderr, "Error: Couldn't open '%s'\n", in_path);
        return NULL;
    }

    fseek(in, 0, SEEK_END);
    long length = ftell(in);
    fseek(in, 0, SEEK_SET);

    if (length < 0) {
        fclose(in);
        return NULL;
    }

    file_buf *buf = malloc(sizeof(file_buf));
    if (!buf) {
        fclose(in);
        return NULL;
    }

    buf->size = (size_t)length;
    buf->data = malloc(buf->size + 1);
    if (!buf->data) {
        fclose(in);
        free(buf);
        return NULL;
    }

    size_t read_count = fread(buf->data, 1, buf->size, in);
    buf->data[read_count] = '\0';

    fclose(in);
    return buf;
}

void file_buf_destroy(file_buf *buf)
{
    if (!buf) return;

    free(buf->data);
    free(buf);
}

void pitch_code(const char *code, const char *out_path)
{
    if (!code || !out_path) return;

    FILE *out = fopen(out_path, "w");
    if (!out) {
        fprintf(stderr, "Error: Couldn't open '%s'\n", out_path);
        return;
    }

    fputs(code, out);
    fclose(out);
}
