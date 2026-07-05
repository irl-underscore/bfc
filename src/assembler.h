#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "dyn_array.h"

typedef enum
{
    ARC_X86_LINUX,
    ARC_X86_64_LINUX
} arc_type;

typedef struct assemble_ctx assemble_ctx;

assemble_ctx *ctx_init(uint32_t tape_size);
void ctx_process(assemble_ctx *ctx, dyn_array *instructions, arc_type type);
char *ctx_assemble(assemble_ctx *ctx);
void ctx_destroy(assemble_ctx *ctx);

#endif /* ASSEMBLER_H  */
