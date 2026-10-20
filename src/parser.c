#include "parser.h"

#include "base_types.h"
#include "dyn_array.h"
#include "error.h"
#include "defs.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_LOOP_INFOS 128

struct _LoopInfoStack
{
    U8 head;
    CursorSpec loops[MAX_LOOP_INFOS];
};

static struct _LoopInfoStack loopStack = {0};
static CursorSpec spec;

static Bool _appendInstruction(DynArray *pDst, Char c);

Bool parseFile(const Char *pFileName, DynArray *pDst)
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
    close(fd);
    if (UNLIKELY(pData == MAP_FAILED))
    {
        emitStdError();
        return FALSE;
    }

    Bool success = TRUE;
    spec.pFileName = pFileName;
    spec.row = 1;
    spec.col = 1;
    Char *pEndPtr = pData + stats.st_size;
    while (pData < pEndPtr)
    {
        Char c = *pData++;
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

    IrInstruction inst = _instructionTable[c];
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
