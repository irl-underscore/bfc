#include "bf_dyn_array.h"
#include <bf_parser.h>
#include <stdio.h>

int main()
{
    BfDynArray *instructions = bfParseFile("test.bf");
    if (!instructions)
    {
        fprintf(stderr, "Failed\n");
        return 1;
    }

    return 0;
}
