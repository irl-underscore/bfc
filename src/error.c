#include "error.h"

#include "base_types.h"
#include "terminal.h"

#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define MAX_ERROR_FMT_LEN 8
#define MAX_ERROR_BUF_LEN 128
#define MAX_ERROR_COUNT 16

static Char _errors[MAX_ERROR_COUNT][MAX_ERROR_BUF_LEN];
static U8 _errorCount;

static const Char *_pCallName;

void initCall(const Char *call)
{
    _pCallName = call;
}

void emitStdError(void)
{
    perror(_pCallName); // I trust you !!!
}

void emitStdErrorFmt(const Char *pFmt, ...)
{
    va_list args;
    va_start(args, pFmt);
    Char fmtBuf[MAX_ERROR_FMT_LEN];
    vsnprintf(fmtBuf, MAX_ERROR_FMT_LEN, pFmt, args);
    va_end(args);
    snprintf(_errors[_errorCount++], MAX_ERROR_BUF_LEN, "%s: %s\n", fmtBuf, strerror(errno));
}

void emitError(const Char *pMsg)
{
    fprintf(stderr, "%s\n", pMsg);
}

void emitErrorFmt(const Char *pFmt, ...)
{
    va_list args;
    va_start(args, pFmt);
    Char fmtBuf[MAX_ERROR_FMT_LEN];
    vsnprintf(fmtBuf, MAX_ERROR_FMT_LEN, pFmt, args);
    va_end(args);
    snprintf(_errors[_errorCount++], MAX_ERROR_BUF_LEN, "%s\n", fmtBuf);
}

// TODO: Make some extra rules for syntax errors
void emitSyntaxError(const ErrorId id, const Char *pFile, U32 row, U32 col)
{
    static const Char *_pErrorIdTable[] = {
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

void flushErrors(void)
{
    for (U8 i = 0; i < _errorCount; i++)
    {
        fprintf(stderr, "%s: %s", _pCallName, _errors[i]);
    }
}
