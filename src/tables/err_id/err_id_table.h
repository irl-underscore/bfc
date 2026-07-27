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

#ifndef ERR_ID_TABLE_H
#define ERR_ID_TABLE_H

typedef enum err_id_type
{
    #define X(prefix, num, name) prefix##num,
    #include "err_id_type.def"
    #undef X
} err_id_type;

typedef struct err_id
{
    char *as_str;
    char *name;
} err_id;

extern const err_id err_id_table[];

#endif /* ERR_ID_TABLE */
