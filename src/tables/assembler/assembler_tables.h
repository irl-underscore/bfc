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

#ifndef ASSEMBLER_TABLES_H
#define ASSEMBLER_TABLES_H

typedef enum reg_type
{
    ASM_REG_TAPE_BASE,
    ASM_REG_COUNT,
    ASM_REG_PRINT_BUF,
    ASM_REG_BYTE_BUFFER,
    ASM_REG_CALL,
    ASM_REG_PARAM0,
    ASM_REG_PARAM1,
    ASM_REG_PARAM2,
    ASM_SYSCALL,
    ASM_EXT_MAX_LEN,
    ASM_EXT_BYTE
} reg_type;

extern const char *x86_64_linux_assembler_table[];
extern const char *x86_linux_assembler_table[];
extern const char **assembler_table[];

#endif /* ASSEMBLER_TABLES_H */
