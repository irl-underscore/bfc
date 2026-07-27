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

#include "error.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_DIAGNOSIS_ERRS 5
#define MAX_ERR_MSG_LEN 64

static err_diagonsis *diags;
static uint8_t err_count = 0;

void emit_err(const char *msg, const char *file, const uint16_t row, const uint16_t col, const err_id_type id, const err_type type)
{
    if (err_count >= MAX_DIAGNOSIS_ERRS) return;

    err_diagonsis diag = {
        .id = 0,
        .msg = NULL,
        .pos.col = 0,
        .pos.row = 0,
        .pos.file = NULL,
        .type = 0
    };

    if (file)
    {
        size_t len = strnlen(file, MAX_ERR_MSG_LEN);
        diag.pos.file = malloc(len);
        strncpy(diag.pos.file, file, len);
    }

    diag.id = id;
    diag.pos.row = row;
    diag.pos.col = col;
    diag.type = type;
    if (!msg)
    {
        size_t len = strnlen(err_id_table[id].name, MAX_ERR_MSG_LEN);
        diag.msg = malloc(len);
        strncpy(diag.msg, err_id_table[id].name, len);
    } else
    {
        size_t len = strnlen(msg, MAX_ERR_MSG_LEN);
        diag.msg = malloc(len);
        strncpy(diag.msg, msg, len);
    }

    diags[err_count] = diag;
    err_count++;
}

err_diagonsis *get_all_errs(void)
{
    return diags;
}

void init_diags(void)
{
    diags = malloc(MAX_DIAGNOSIS_ERRS * sizeof(err_diagonsis));
}

uint16_t get_err_count(void)
{
    return (err_count >= MAX_DIAGNOSIS_ERRS) ? MAX_DIAGNOSIS_ERRS - 1 : err_count;
}

void throw_errs(void)
{
    printf("This is an error :)");
}
