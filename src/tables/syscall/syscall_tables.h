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

#ifndef SYSCALL_TABLES_H
#define SYSCALL_TABLES_H
#include <stdint.h>

typedef enum
{
    BF_CALL_EXIT = 0,
    BF_CALL_READ = 1,
    BF_CALL_WRITE = 2
} bf_call;

extern int64_t x86_64_linux_syscall_table[];
extern int64_t x86_linux_syscall_table[];

#endif /* SYSCALL_TABLES_H */
