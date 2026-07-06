/*
 * Copyright (c) 2026 irl-underscore
 * SPDX-License-Identifier: Apache-2.0
 * This file is part of brainfuck and is licensed under the Apache 2.0 License.
 */

#define TAR(arc, num) _MATCH_##arc(num)
#ifndef _MATCH_X86_LINUX
#define _MATCH_X86_LINUX(num)
#endif /* _MATCH_X86_LINUX */
#ifndef _MATCH_X86_64_LINUX
#define _MATCH_X86_64_LINUX(num)
#endif /* _MATCH_X86_64_LINUX */
#ifndef _MATCH_ARM_LINUX
#define _MATCH_ARM_LINUX(num)
#endif /* _MATCH_ARM_LINUX */
#ifndef _MATCH_ARM64_LINUX
#define _MATCH_ARM64_LINUX(num)
#endif /* _MATCH_ARM64_LINUX */

#define X(bf_code, matches) [bf_code] matches + 1,
