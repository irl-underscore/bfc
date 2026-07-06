/*
 * Copyright (c) 2026 irl-underscore
 * SPDX-License-Identifier: Apache-2.0
 * This file is part of brainfuck and is licensed under the Apache 2.0 License.
 */

// Format: x(bf_call, TAR(arc, syscall_num) ...)
X(BF_CALL_READ, TAR(X86_LINUX, 3) TAR(X86_64_LINUX, 0))
X(BF_CALL_WRITE, TAR(X86_LINUX, 4) TAR(X86_64_LINUX, 1))
X(BF_CALL_EXIT, TAR(X86_LINUX, 5) TAR(X86_64_LINUX, 60))
