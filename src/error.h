#ifndef ERROR_H
#define ERROR_H
#include "base_types.h"

typedef enum ErrorId_e
{
    ERROR_ID_UNMATCHED_LOOP_START,
    ERROR_ID_UNMATCHED_LOOP_END
} ErrorId;

void initCall(char *pCall);

void emitStdError(void);
void emitStdErrorFmt(const Char *pFmt, ...);

void emitError(const Char *pMsg);
void emitErrorFmt(const Char *pFmt, ...);

void emitSyntaxError(const ErrorId id, const Char *pFile, U32 row, U32 col);

#endif /* ERROR_H */
