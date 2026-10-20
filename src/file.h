#ifndef FILE_H
#define FILE_H
#include "base_types.h"
#include "defs.h"

typedef struct File_s
{
    Char *pData;
    Char *pFileName;
    Size size;
#if defined (OS_LINUX) || defined (OS_APPLE)
    I32 fd;
#endif /* os */
} File;

Bool openFile(char *__restrict pFileName, File *__restrict pFile);
Bool closeFile(File *pFile);

#endif /* FILE_H */
