#include "bf_terminal.h"

#if defined (__linux__)
static const char *_pLinuxColorLookupTable[] = {
    [BF_TERMINAL_COLOR_WHITE] = "\e[0;37m",
    [BF_TERMINAL_COLOR_BOLD_RED] = "\e[1;31m",
    [BF_TERMINAL_COLOR_BOLD_YELLOW] = "\e[1;33m",
    [BF_TERMINAL_COLOR_BOLD_WHITE] = "\e[1;37m"
};

static inline void _bfSetTerminalColor(FILE *pFile, BfTerminalColor color)
{
    fprintf(pFile, "%s", _pLinuxColorLookupTable[color]);
}

#elif defined (_WIN32)

#elif  defined (__APPLE__) && defined (__MACH__)

#endif /* os */

void bfSetTerminalCol(FILE *pFile, const BfTerminalColor color)
{
    _bfSetTerminalColor(pFile, color);
}
