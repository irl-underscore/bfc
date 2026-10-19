#ifndef TERMINAL_H
#define TERMINAL_H
#include <stdio.h>

typedef enum TerminalColor_e
{
    TERMINAL_COLOR_WHITE,
    TERMINAL_COLOR_BOLD_RED,
    TERMINAL_COLOR_BOLD_YELLOW,
    TERMINAL_COLOR_BOLD_WHITE
} TerminalColor;

void setTerminalCol(FILE *file, const TerminalColor color);

#endif /* TERMINAL_H */
