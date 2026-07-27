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

#ifndef ERROR_H
#define ERROR_H
#include "tables/err_id/err_id_table.h"

#include <stdint.h>

typedef enum res // TODO: get better name
{
    OK,
    ERR
} res;

typedef struct err_pos
{
    uint16_t row;
    uint16_t col;
    char *file;
} err_pos;

typedef enum err_type
{
    NOTE,
    WARN,
    ERROR
} err_type;

typedef struct err_diagonsis
{
    char *msg;
    err_pos pos;
    err_id_type id;
    err_type type;
} err_diagonsis;

void emit_err(const char *msg, const char *file, const uint16_t row, const uint16_t col, const err_id_type id, const err_type type);
err_diagonsis *get_all_errs(void);
uint16_t get_err_count(void);
void init_diags(void);
void throw_errs(void);

#endif /* ERROR_H */
