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

#include "dyn_string.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

struct string {
    size_t length;
    size_t capacity;
    char *data;
};

string *string_create(size_t initial_size)
{
    initial_size = (initial_size == 0) ? 1 : initial_size;
    string *str = malloc(sizeof(string));
    if (!str) return NULL;

    str->data = malloc(initial_size + 1);
    if (!str->data)
    {
        free(str);
        return NULL;
    }

    str->data[0] = '\0';
    str->capacity = initial_size;
    str->length = 0;
    return str;
}

string *string_copy(const char *data)
{
    if (!data) return NULL;

    size_t len = strlen(data);
    string *str = string_create(len);
    if (!str) return NULL;

    strcpy(str->data, data);
    str->length = len;
    return str;
}

static void string_realloc(string *str, size_t target_size)
{
    size_t new_capacity = str->capacity * 2;
    if (new_capacity < target_size) new_capacity = target_size;

    char *temp = realloc(str->data, new_capacity + 1);
    if (!temp) return;

    str->data = temp;
    str->capacity = new_capacity;
}

void string_append_string(string *str, const char *data)
{
    if (!str || !str->data || !data) return;

    size_t data_len = strlen(data);
    size_t target_len = str->length + data_len;
    if (target_len > str->capacity) string_realloc(str, target_len);

    memcpy(str->data + str->length, data, data_len);
    str->length = target_len;
    str->data[str->length] = '\0';
}

void string_append_format(string *str, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    size_t len = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (len < 0) return;

    size_t target_len = str->length + len;
    if (target_len > str->capacity) string_realloc(str, target_len);

    va_start(args, format);
    vsnprintf(str->data + str->length, len + 1, format, args);
    va_end(args);

    str->length = target_len;
    str->data[str->length] = '\0';
}

char *string_get_raw(string *str)
{
    if (!str || !str->data) return NULL;

    return str->data;
}

void string_destroy(string *str)
{
    if (!str) return;

    if (str->data) free(str->data);

    free(str);
}
