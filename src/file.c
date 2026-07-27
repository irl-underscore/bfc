/*
 * Copyright 2026 irl-underscore
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
    FILE *in = fopen(in_path, "rb");
    if (!in)
    {
        emit_err(NULL, in_path, 0, 0, G1001, ERROR);
        return NULL;
    }

    fseek(in, 0, SEEK_END);
    long length = ftell(in);
    fseek(in, 0, SEEK_SET);
    if (length < 0) {
        fclose(in);
        emit_err(NULL, in_path, 0, 0, G1002, ERROR);
        return NULL;
    }

    file_buf *buf = malloc(sizeof(file_buf));
    if (!buf) {
        fclose(in);
        emit_err(NULL, NULL, 0, 0, G2001, ERROR);
        return NULL;
    }

    buf->size = (size_t)length;
    buf->data = malloc(buf->size + 1);
    if (!buf->data) {
        fclose(in);
        free(buf);
        emit_err(NULL, NULL, 0, 0, G2001, ERROR);
        return NULL;
    }

    size_t read_count = fread(buf->data, 1, buf->size, in);
    buf->data[read_count] = '\0';
    fclose(in);
    return buf;
}

void file_buf_destroy(file_buf *buf)
{
    free(buf->data);
    free(buf);
}

res pitch_code(const char *code, const char *out_path)
{
    FILE *out = fopen(out_path, "w");
    if (!out) {
        emit_err(NULL, out_path, 0, 0, G1001, ERROR);
        return ERR;
    }

    fputs(code, out);
    fclose(out);
    return OK;
}
