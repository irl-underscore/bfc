#include <bf_arena.h>
#include <bf_defs.h>
#include <bf_dyn_array.h>
#include <bf_error.h>
#include <bf_parser.h>

#include <stdio.h>

int main(int argc, char **ppArgv)
{
    (void)argc;
    initCall(*ppArgv);
    BfArena arena;
    bfArenaCreate(BF_MB, &arena);
    BfDynArray instructions;
    bfDynArrayCreate(128, sizeof(BfIrInstruction), &instructions, &arena);
    if (bfParseFile("test.bf", &instructions) == FALSE)
    {
        fprintf(stderr, "Failed\n");
        return 1;
    }

    bfArenaDestroy(&arena);
    return 0;
}
