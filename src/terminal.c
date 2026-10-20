#include "terminal.h"

#include "defs.h"

void setTerminalColor(FILE *pFile, TerminalColor color)
{
#if defined (OS_LINUX) || defined (OS_APPLE)
    static const char *_pLinuxColorLookupTable[] = {
        [TERMINAL_COLOR_WHITE] = "\x1b[0;37m",
        [TERMINAL_COLOR_BOLD_RED] = "\x1b[1;31m",
        [TERMINAL_COLOR_BOLD_YELLOW] = "\x1b[1;33m",
        [TERMINAL_COLOR_BOLD_WHITE] = "\x1b[1;37m"
    };

    fprintf(pFile, "%s", _pLinuxColorLookupTable[color]);
#endif /* os */
}
