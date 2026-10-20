#ifndef PARSER_H
#define PARSER_H
#include "base_types.h"
#include "file.h"
#include "dyn_array.h"

typedef enum IrInstructionKind_e
{
    IR_INSTRUCTION_KIND_ADD, // +/-
    IR_INSTRUCTION_KIND_SHIFT, // >/<
    IR_INSTRUCTION_KIND_LOOP_START, // [
    IR_INSTRUCTION_KIND_LOOP_END, // ]
    IR_INSTRUCTION_KIND_OUT, // .
    IR_INSTRUCTION_KIND_IN // ,
} IrInstructionKind;

typedef struct IrInstruction_s
{
    IrInstructionKind kind;
    I32 val;
} IrInstruction;

typedef struct CursorSpec_s
{
    U32 row, col;
    const Char *pFileName;
} CursorSpec;

SSize parseFile(File file, IrInstruction *pBuf);
DynArray *optimizeIntructions(DynArray *pDynArray);

#endif /* PARSER_H */
