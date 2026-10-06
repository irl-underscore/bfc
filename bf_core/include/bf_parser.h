#ifndef BF_LEXER_H
#define BF_LEXER_H
#include "bf_base_types.h"
#include "bf_dyn_array.h"

typedef enum BfIrInstructionType_e
{
    BF_IR_INSTRUCTION_TYPE_ADD, // +
    BF_IR_INSTRUCTION_TYPE_SUB, // -
    BF_IR_INSTRUCTION_TYPE_RIGHT_SHIFT, // >
    BF_IR_INSTRUCTION_TYPE_LEFT_SHIFT, // <
    BF_IR_INSTRUCTION_TYPE_LOOP, // [
    BF_IR_INSTRUCTION_TYPE_OUT, // .
    BF_IR_INSTRUCTION_TYPE_IN // ,
} BfIrInstructionType;

typedef struct BfIrInstruction_s
{
    BfIrInstructionType type;
    I32 val;
    Size loopLen;
} BfIrInstruction;

typedef struct BfCursorSpec_s
{
    U32 row, col;
    const Char *pFileName;
} BfCursorSpec;

Bool bfParseFile(const Char *pFileName, BfDynArray *pDst);
BfDynArray *bfOptimizeIntructions(BfDynArray *pDynArray);

#endif /* BF_LEXER_H */
