#ifndef FILE_H
#define FILE_H
#include "base_types.h"
#include "defs.h"

enum
{
    _FILE_MODE_READ_ID,
    _FILE_MODE_WRITE_ID,
    _FILE_MODE_CREATE_ID,
    _FILE_MODE_COUNT
};

typedef enum FileMode_e
{
    FILE_MODE_READ = (1 << _FILE_MODE_READ_ID),
    FILE_MODE_WRITE = (1 << _FILE_MODE_WRITE_ID),
    FILE_MODE_READ_WRITE = FILE_MODE_READ | FILE_MODE_WRITE,
    FILE_MODE_CREATE = (1 << _FILE_MODE_CREATE_ID) // ill keep the e :)
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

Bool openFile(const char *__restrict pFileName, const FileMode fileModes, File *__restrict pFile);
Bool closeFile(File *pFile);

#endif /* FILE_H */
