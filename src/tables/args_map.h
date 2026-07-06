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

#ifndef ARGS_MAP_H
#define ARGS_MAP_H
#include <stdint.h>

typedef enum flag_type
{
    FLAG_TYPE_BOOL,
    FLAG_TYPE_STRING,
    FLAG_TYPE_CUSTOM,
    FLAG_TYPE_FUNCTION
} flag_type;

typedef enum flag_pos
{
    FLAG_POS_THIS,
    FLAG_POS_NEXT
} flag_pos;

typedef struct flag
{
    char short_form;
    char *long_form;
    flag_type type;
    flag_pos pos;
    uint8_t offset;
    void (*custom_func)(char *, void *);
    void (*func)(void);
} flag;

extern flag map[];
extern size_t map_len;

#endif /* ARGS_MAP_H */
