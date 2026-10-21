#ifndef FILE_H
#define FILE_H
#include "base_types.h"
#include "defs.h"

typedef enum FileMode_e
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_READ_WRITE,
    FILE_MODE_CREATE // ill keep the e :)
} FileMode;

typedef struct File_s
{
    Char *pData;
    const Char *pFileName;
    Size size;
#if defined (OS_LINUX) || defined (OS_APPLE)
    I32 fd;
#endif /* os */
} File;

Bool openFile(const char *__restrict pFileName, const FileMode fileMode, File *__restrict pFile);
Bool closeFile(File *pFile);

#endif /* FILE_H */
