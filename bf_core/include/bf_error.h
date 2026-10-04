#ifndef BF_ERROR_H
#define BF_ERROR_H

void initCall(char *pCall);

void emitStdError(void);
void emitStdErrorFmt(const char *pFmt, ...);

#endif /* BF_ERROR_H */
