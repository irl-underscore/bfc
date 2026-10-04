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
    BF_IR_INSTRUCTION_TYPE_LOOP_START, // [
    BF_IR_INSTRUCTION_TYPE_LOOP_END, // ]
    BF_IR_INSTRUCTION_TYPE_OUT, // .
    BF_IR_INSTRUCTION_TYPE_IN // ,
} BfIrInstructionType;

typedef struct BfIrInstruction_s
{
    BfIrInstructionType type;
    U16 val;
} BfIrInstruction;

BfDynArray *bfParseFile(const char *pFileName);
BfDynArray *bfOptimizeIntructions(BfDynArray *pDynArray);

#endif /* BF_LEXER_H */
