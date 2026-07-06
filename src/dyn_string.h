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

#ifndef STRING_H
#define STRING_H
#include <stdint.h>

/*
 * So the string struct is build like this:
 * if initial_size = 3
 * the structure would look like this:
 * ['\0', '', '', '']
 * wich means that the acutal size of the string is
 * one bigger than the struct says.
 * So when I acess str->data[capacity] it returns NULL
 * and str->data[length] always returns '\0'
 */

typedef struct string string;

string *string_create(size_t initial_size);
string *string_copy(const char *data);
void string_append_string(string *str, const char *data);
void string_append_format(string *str, const char *format, ...);
char *string_get_raw(string *str);
void string_destroy(string *str);

#endif
