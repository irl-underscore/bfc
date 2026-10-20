#include "parser.h"

#include "error.h"

#define MAX_LOOP_INFOS 128

struct _LoopInfoStack
{
    U8 head;
    CursorSpec loops[MAX_LOOP_INFOS];
};

static struct _LoopInfoStack loopStack = {0};
static CursorSpec spec;

static Bool _appendInstruction(DynArray *pDst, Char c);

Bool parseFile(File file, DynArray *pDst)
{
    Bool success = TRUE;
    spec.pFileName = file.pFileName;
    spec.row = 1;
    spec.col = 1;
    Char *pEndPtr = file.pData + file.size;
    Char *_pData = file.pData;
    while (_pData < pEndPtr)
    {
        Char c = *_pData++;
        success = (_appendInstruction(pDst, c) == FALSE) ? FALSE : success;
        if (c == '\n')
        {
            spec.row++;
            spec.col = 1;
            continue;
        }

        spec.col++;
    }

    if (loopStack.head != 0)
    {
        for (U8 i = 0; i < loopStack.head; i++)
        {
            CursorSpec loopSpec = loopStack.loops[i];
            emitSyntaxError(ERROR_ID_UNMATCHED_LOOP_START, loopSpec.pFileName, loopSpec.row, loopSpec.col);
        }

        success = FALSE;
    }

    return success;
}

DynArray *optimizeIntructions(DynArray *pDynArray)
{
    return pDynArray;
}

static Bool _appendInstruction(DynArray *pDst, Char c)
{
    static const IrInstruction _instructionTable[U8_MAX] = {
        ['+'] = {IR_INSTRUCTION_KIND_ADD, 1},
        ['-'] = {IR_INSTRUCTION_KIND_ADD, -1},
        ['>'] = {IR_INSTRUCTION_KIND_SHIFT, 1},
        ['<'] = {IR_INSTRUCTION_KIND_SHIFT, -1},
        ['['] = {IR_INSTRUCTION_KIND_LOOP_START, 1},
        [']'] = {IR_INSTRUCTION_KIND_LOOP_END, 1},
        ['.'] = {IR_INSTRUCTION_KIND_OUT, 1},
        [','] = {IR_INSTRUCTION_KIND_IN, 1}
    };

    IrInstruction inst = _instructionTable[(U8)c];
    if (inst.kind == IR_INSTRUCTION_KIND_LOOP_START)
    {
        loopStack.loops[loopStack.head++] = spec;
    } else if (inst.kind == IR_INSTRUCTION_KIND_LOOP_END)
    {
        if (loopStack.head == 0)
        {
            emitSyntaxError(ERROR_ID_UNMATCHED_LOOP_END, spec.pFileName, spec.row, spec.col);
            return FALSE;
        } else
        {
            loopStack.head--;
        }
    }

    dynArrayAppend(pDst, &inst);
    return TRUE;
}
