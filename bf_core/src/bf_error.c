#include "bf_error.h"

#include "bf_base_types.h"

#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define BF_MAX_ERROR_BUF_LEN 128

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
    Char fmtBuf[BF_MAX_ERROR_BUF_LEN];
    vsnprintf(fmtBuf, BF_MAX_ERROR_BUF_LEN, pFmt, args);
    va_end(args);
    fprintf(stderr, "%s: %s: %s\n", _pCallName, fmtBuf, strerror(errno));
}
