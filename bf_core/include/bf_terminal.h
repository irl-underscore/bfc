#ifndef BF_TERMINAL_H
#define BF_TERMINAL_H
#include <stdio.h>

typedef enum BfTerminalColor_e
{
    BF_TERMINAL_COLOR_WHITE,
    BF_TERMINAL_COLOR_BOLD_RED,
    BF_TERMINAL_COLOR_BOLD_YELLOW,
    BF_TERMINAL_COLOR_BOLD_WHITE
} BfTerminalColor;

void bfSetTerminalCol(FILE *file, const BfTerminalColor color);

#endif /* BF_TERMINAL_H */
