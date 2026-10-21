#include "base_types.h"
#include "error.h"
#include "file.h"
#include "parser.h"

#include <stdio.h>

int main(int argc, char **ppArgv)
{
    (void)argc;
    initCall(*ppArgv);
    File input;
    if (openFile("test.bf", FILE_MODE_READ, &input) == FALSE)
    {
        flushErrors();
        return 1;
    }

    IrInstruction instructionBuf[input.size];
    SSize instructionCount = parseFile(input, &instructionBuf[0]);
    if (instructionCount == -1)
    {
        fprintf(stderr, "Failed\n");
        return 1;
    }

    return 0;
}
