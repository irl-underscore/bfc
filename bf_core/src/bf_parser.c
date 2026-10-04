#include "bf_parser.h"

#include "bf_stack.h"
#include "bf_defs.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

BfDynArray *bfParseFile(const char *pFileName)
{
    static const BfIrInstruction _instructionTable[U8_MAX] = {
        ['+'] = {BF_IR_INSTRUCTION_TYPE_ADD, 1},
        ['-'] = {BF_IR_INSTRUCTION_TYPE_SUB, 1},
        ['>'] = {BF_IR_INSTRUCTION_TYPE_RIGHT_SHIFT, 1},
        ['<'] = {BF_IR_INSTRUCTION_TYPE_LEFT_SHIFT, 1},
        ['['] = {BF_IR_INSTRUCTION_TYPE_LOOP_START, 1},
        [']'] = {BF_IR_INSTRUCTION_TYPE_LOOP_END, 1},
        ['.'] = {BF_IR_INSTRUCTION_TYPE_OUT, 1},
        [','] = {BF_IR_INSTRUCTION_TYPE_IN, 1}
    };

    I16 fd = open(pFileName, O_RDONLY);
    if (UNLIKELY(fd == -1))
    {
        perror("bfc"); // TODO: Improve error system
        return NULL;
    }

    struct stat stats;
    fstat(fd, &stats);
    Byte *data = mmap(NULL, stats.st_size, PROT_READ, MAP_DENYWRITE | MAP_PRIVATE, fd, 0);
    if (UNLIKELY(data == MAP_FAILED))
    {
        perror("bfc");
        return NULL;
    }

    BfDynArray *instructions = bfDynArrayCreate(128, sizeof(BfIrInstruction));
    BfStack stack;
    U32 loopCount = 0;
    for (Size i = 0; i < (Size)stats.st_size; i++)
    {
        BfIrInstruction inst = _instructionTable[*(data + i)];
        if (LIKELY(inst.val == 1))
        {
            if (inst.type == BF_IR_INSTRUCTION_TYPE_LOOP_START)
            {
                bfStackPush(&stack, loopCount);
                inst.val = loopCount++;
            } else if (inst.type == BF_IR_INSTRUCTION_TYPE_LOOP_END)
            {
                U32 endId = bfStackPop(&stack);
                if (UNLIKELY(endId == U8_MAX))
                {
                    // error
                    printf("Failing because lonely ]\n");
                    break;
                }

                inst.val = endId;
            }

            bfDynArrayAppend(instructions, &inst);
        }
    }

    if (UNLIKELY(stack.head != 0))
    {
        // error
        printf("Failed because lonely [/]\n");
        printf("Stack entries: ");
        while (stack.head != 0) printf("%u ", bfStackPop(&stack));
        printf("\n");
        return NULL;
    }

    return instructions;
}

BfDynArray *bfOptimizeIntructions(BfDynArray *pDynArray)
{
    return pDynArray;
}
