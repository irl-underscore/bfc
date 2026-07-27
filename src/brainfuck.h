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

#ifndef BRAINFUCK_H
#define BRAINFUCK_H
#include "args.h"

#define DEFAULT_TAPE_SIZE 1050
#define DEFAULT_OUTPUT "a.s"

res process(compiler_options defaults, char *argv[], uint32_t argc);

#endif /* BRAINFUCK_H */
