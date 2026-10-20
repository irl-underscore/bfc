#include "error.h"

#include "base_types.h"
#include "terminal.h"

#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define MAX_ERROR_BUF_LEN 128

static char *_pCallName;

void initCall(char *call)
{
    _pCallName = call;
}

void emitStdError(void)
{
    perror(_pCallName); // I trust you !!!
}

void emitStdErrorFmt(const char *pFmt, ...)
{
    va_list args;
    va_start(args, pFmt);
    Char fmtBuf[MAX_ERROR_BUF_LEN];
    vsnprintf(fmtBuf, MAX_ERROR_BUF_LEN, pFmt, args);
    va_end(args);
    fprintf(stderr, "%s: %s: %s\n", _pCallName, fmtBuf, strerror(errno));
}

void emitSyntaxError(const ErrorId id, const char *pFile, U32 row, U32 col)
{
    static const char *_pErrorIdTable[] = {
        [ERROR_ID_UNMATCHED_LOOP_END] = "Unmatched ']'",
        [ERROR_ID_UNMATCHED_LOOP_START] = "Unmatched '['"
    };

    setTerminalColor(stderr, TERMINAL_COLOR_BOLD_WHITE);
    fprintf(stderr, "%s:%u:%u:", pFile, row, col);
    setTerminalColor(stderr, TERMINAL_COLOR_BOLD_RED);
    fprintf(stderr, " Error: ");
    setTerminalColor(stderr, TERMINAL_COLOR_WHITE);
    fprintf(stderr, "%s\n", _pErrorIdTable[id]);
}
