#include "bf_parser.h"

#include "bf_arena.h"
#include "bf_dyn_array.h"
#include "bf_error.h"
#include "bf_defs.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

static BfIrInstruction _bfGetInstruction(Char c);
static Bool _bfParseLoop(Char **ppData, Size dataLen, BfDynArray *pDst, BfCursorSpec *pSpec);

Bool bfParseFile(const Char *pFileName, BfDynArray *pDst)
{
    I32 fd = open(pFileName, O_RDONLY);
    if (UNLIKELY(fd == -1))
    {
        emitStdErrorFmt(pFileName);
        return FALSE;
    }

    struct stat stats;
    if (UNLIKELY(fstat(fd, &stats) != 0))
    {
        emitStdErrorFmt(pFileName);
        close(fd);
        return FALSE;
    }

    if (UNLIKELY(stats.st_size == 0))
    {
        // TODO: Custom error
        close(fd);
        return FALSE;
    }

    if (UNLIKELY(S_ISDIR(stats.st_mode)))
    {
        // TODO: Custom error
        close(fd);
        return FALSE;
    }

    Char *pData = mmap(NULL, stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (UNLIKELY(pData == MAP_FAILED))
    {
        emitStdError();
        close(fd);
        return FALSE;
    }

    BfCursorSpec spec;
    spec.pFileName = pFileName;
    spec.row = 0;
    spec.col = 0;
    for (Size i = 0; i < (Size)stats.st_size; i++)
    {
        pData++;
        Char current = *pData;
        BfIrInstruction inst = _bfGetInstruction(current);
        if (inst.val == 1)
        {
            if (inst.type == BF_IR_INSTRUCTION_TYPE_LOOP)
            {
                UPtr tempDataPtr = (UPtr)pData;
                if (_bfParseLoop(&pData, stats.st_size, pDst, &spec) == FALSE) return FALSE;

                Size offset = (UPtr)pData - tempDataPtr;
                i += offset;
                goto lNextCol;
            }

            bfDynArrayAppend(pDst, &inst);
            goto lNextCol;
        } else if (current == ']')
        {
            emitSyntaxError(BF_ERROR_ID_TYPE_UNMATCHED_LOOP_START, spec.pFileName, spec.row, spec.col);
            return FALSE;
        } else if (current == '\n')
        {
            spec.row++;
            spec.col = 0;
            continue;
        }

        lNextCol:
        spec.col++;
    }

    close(fd);
    return TRUE;
}

BfDynArray *bfOptimizeIntructions(BfDynArray *pDynArray)
{
    return pDynArray;
}

static BfIrInstruction _bfGetInstruction(Char c)
{
    static const BfIrInstruction _instructionTable[U8_MAX] = {
        ['+'] = {BF_IR_INSTRUCTION_TYPE_ADD, 1},
        ['-'] = {BF_IR_INSTRUCTION_TYPE_SUB, 1},
        ['>'] = {BF_IR_INSTRUCTION_TYPE_RIGHT_SHIFT, 1},
        ['<'] = {BF_IR_INSTRUCTION_TYPE_LEFT_SHIFT, 1},
        ['['] = {BF_IR_INSTRUCTION_TYPE_LOOP, 1},
        ['.'] = {BF_IR_INSTRUCTION_TYPE_OUT, 1},
        [','] = {BF_IR_INSTRUCTION_TYPE_IN, 1}
    };

    return _instructionTable[(U8)c];
}

static Bool _bfParseLoop(Char **ppData, Size dataLen, BfDynArray *pDst, BfCursorSpec *pSpec)
{
    BfCursorSpec startSpec = *pSpec;
    BfIrInstruction loop;
    loop.type = BF_IR_INSTRUCTION_TYPE_LOOP;
    bfDynArrayAppend(pDst, &loop);
    BfIrInstruction *pLoop = (BfIrInstruction*)(pDst->data + (pDst->used - pDst->elemSize));
    for (Size i = 0; i < dataLen; i++)
    {
        (*ppData)++;
        Char current = **(ppData);
        BfIrInstruction inst = _bfGetInstruction(current);
        if (inst.val == 1)
        {
            if (inst.type == BF_IR_INSTRUCTION_TYPE_LOOP)
            {
                UPtr tempDataPtr = (UPtr)*ppData;
                if (_bfParseLoop(ppData, dataLen - i, pDst, pSpec) == FALSE) return FALSE;

                Size offset = (UPtr)*ppData - tempDataPtr;
                i += offset;
                goto lNextCol;
            }

            bfDynArrayAppend(pDst, &inst);
            pLoop->loopLen++;
            goto lNextCol;
        } else if (current == ']')
        {
            return TRUE;
        } else if (current == '\n')
        {
            pSpec->row++;
            pSpec->col = 0;
            continue;
        }

        lNextCol:
        pSpec->col++;
    }

    emitSyntaxError(BF_ERROR_ID_TYPE_UNMATCHED_LOOP_END, startSpec.pFileName, startSpec.row, startSpec.col);
    return FALSE;
}
