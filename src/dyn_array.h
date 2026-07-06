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

#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H
#include <stdint.h>

typedef struct dyn_array dyn_array;

dyn_array *dyn_array_create(size_t intial_size, size_t obj_size);
void dyn_array_insert_end(dyn_array *arr, void *obj);
void dyn_array_restrict_insert_end(dyn_array *__restrict__ arr, void *__restrict__ obj);
size_t dyn_array_get_size(dyn_array *arr);
void *dyn_array_get(dyn_array *arr, size_t index);
void dyn_array_destroy(dyn_array *arr);

#endif
