#include "terminal.h"

#if defined (__linux__)
static const char *_pLinuxColorLookupTable[] = {
    [TERMINAL_COLOR_WHITE] = "\e[0;37m",
    [TERMINAL_COLOR_BOLD_RED] = "\e[1;31m",
    [TERMINAL_COLOR_BOLD_YELLOW] = "\e[1;33m",
    [TERMINAL_COLOR_BOLD_WHITE] = "\e[1;37m"
};

static inline void _setTerminalColor(FILE *pFile, TerminalColor color)
{
    fprintf(pFile, "%s", _pLinuxColorLookupTable[color]);
}

#elif defined (_WIN32)

#elif  defined (__APPLE__) && defined (__MACH__)

#endif /* os */

void setTerminalCol(FILE *pFile, const TerminalColor color)
{
    _setTerminalColor(pFile, color);
}
