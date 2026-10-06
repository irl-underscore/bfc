#ifndef BF_ERROR_H
#define BF_ERROR_H
#include "bf_base_types.h"

typedef enum ErrorId_e
{
    BF_ERROR_ID_TYPE_UNMATCHED_LOOP_START,
    BF_ERROR_ID_TYPE_UNMATCHED_LOOP_END
} ErrorId;

void initCall(char *pCall);

void emitStdError(void);
void emitStdErrorFmt(const char *pFmt, ...);

void emitSyntaxError(const ErrorId id, const char *pFile, U32 row, U32 col);

#endif /* BF_ERROR_H */
