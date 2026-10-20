#include "base_types.h"
#include "arena.h"
#include "defs.h"
#include "dyn_array.h"
#include "error.h"
#include "file.h"
#include "parser.h"

#include <stdio.h>

int main(int argc, char **ppArgv)
{
    (void)argc;
    initCall(*ppArgv);
    Arena arena;
    if (arenaCreate(MB, &arena) == FALSE)
    {
        flushErrors();
        return 1;
    }

    DynArray instructions;
    if (dynArrayCreate(128, sizeof(IrInstruction), &instructions, &arena) == FALSE)
    {
        flushErrors();
        return 1;
    }

    File input;
    if (openFile("test.bf", &input) == FALSE)
    {
        flushErrors();
        return 1;
    }

    if (parseFile(input, &instructions) == FALSE)
    {
        fprintf(stderr, "Failed\n");
        return 1;
    }

    arenaDestroy(&arena);
    return 0;
}
