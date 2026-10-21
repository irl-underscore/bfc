#include "parser.h"

#include "base_types.h"
#include "error.h"

#define MAX_LOOP_INFOS 128

struct _CursorInfo_s
{
    U32 row, col;
    const Char *pFileName;
};

struct _LoopInfoStack
{
    U8 head;
    struct _CursorInfo_s loops[MAX_LOOP_INFOS];
};

static struct _LoopInfoStack _loopStack = {0};
static struct _CursorInfo_s _cursorInfo;
static Size _bufIdx = 0;

static Bool _appendInstruction(IrInstruction *pBuf, Char c);

SSize parseFile(File file, IrInstruction *pBuf)
{
    Bool success = TRUE;
    _cursorInfo.pFileName = file.pFileName;
    _cursorInfo.row = 1;
    _cursorInfo.col = 1;
    Char *pEndPtr = file.pData + file.size;
    Char *_pData = file.pData;
    while (_pData < pEndPtr)
    {
        Char c = *_pData++;
        success = (_appendInstruction(pBuf, c) == FALSE) ? FALSE : success;
        if (c == '\n')
        {
            _cursorInfo.row++;
            _cursorInfo.col = 1;
            continue;
        }

        _cursorInfo.col++;
    }

    if (_loopStack.head != 0)
    {
        for (U8 i = 0; i < _loopStack.head; i++)
        {
            struct _CursorInfo_s loopSpec = _loopStack.loops[i];
            emitSyntaxError(ERROR_ID_UNMATCHED_LOOP_START, loopSpec.pFileName, loopSpec.row, loopSpec.col);
        }

        success = FALSE;
    }

    SSize reBufIdx = _bufIdx;
    _bufIdx = 0;
    return (success == TRUE) ? reBufIdx : -1;
}

DynArray *optimizeIntructions(DynArray *pDynArray)
{
    return pDynArray;
}

static Bool _appendInstruction(IrInstruction *pBuf, Char c)
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
    if (inst.val == 0) return TRUE;

    if (inst.kind == IR_INSTRUCTION_KIND_LOOP_START)
    {
        _loopStack.loops[_loopStack.head++] = _cursorInfo;
    } else if (inst.kind == IR_INSTRUCTION_KIND_LOOP_END)
    {
        if (_loopStack.head == 0)
        {
            emitSyntaxError(ERROR_ID_UNMATCHED_LOOP_END, _cursorInfo.pFileName, _cursorInfo.row, _cursorInfo.col);
            return FALSE;
        } else
        {
            _loopStack.head--;
        }
    }

    pBuf[_bufIdx++] = inst;
    return TRUE;
}
