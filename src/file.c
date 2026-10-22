#include "file.h"

#include "base_types.h"
#include "error.h"

#include <stdlib.h>

#if defined (OS_LINUX) || defined (OS_APPLE)
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/mman.h>
    #include <unistd.h>
#endif /*os */

static int _getOFlags(FileMode fileModes);

Bool openFile(const char *__restrict pFileName, const FileMode fileModes, File *__restrict pFile)
{
#if defined (OS_LINUX) || defined (OS_APPLE)
    pFile->fd = open(pFileName, _getOFlags(fileModes));
    if (pFile->fd == -1)
    {
        emitStdError();
        return FALSE;
    }

    struct stat fileStat;
    if (fstat(pFile->fd, &fileStat) == -1)
    {
        close(pFile->fd);
        pFile->fd = -1;
        emitStdError();
        return FALSE;
    }

    if (S_ISDIR(fileStat.st_mode))
    {
        close(pFile->fd);
        pFile->fd = -1;
        emitErrorFmt("%s: is a directory", pFileName);
        return FALSE;
    }

    pFile->size = fileStat.st_size;
    pFile->pData = mmap(NULL, pFile->size, PROT_READ, MAP_PRIVATE | MAP_FILE, pFile->fd, 0);
    if (pFile->pData == MAP_FAILED)
    {
        close(pFile->fd);
        pFile->fd = -1;
        pFile->pData = NULL;
        emitStdError();
        return FALSE;
    }

    pFile->pFileName = pFileName;
    return TRUE;
#endif /* os */
}

Bool closeFile(File *pFile)
{
#if defined (OS_LINUX) || defined (OS_APPLE)
    if (munmap(pFile->pData, pFile->size) == -1)
    {
        emitStdError();
        return FALSE;
    }

    pFile->pData = NULL;
    pFile->size = 0;
    if (close(pFile->fd) == -1)
    {
        emitStdError();
        return FALSE;
    }

    pFile->fd = -1;
    return TRUE;
#endif /* os */
}

static int _getOFlags(FileMode fileModes)
{
    int oFlags = 0;
    static int _oFlagTable[] = {
        [0] = -1,
        [FILE_MODE_READ] = O_RDONLY,
        [FILE_MODE_WRITE] = O_WRONLY,
        [FILE_MODE_CREATE] = O_CREAT
    };

    for (U8 i = 0; i < _FILE_MODE_COUNT; i++)
    {
        int oFlag = fileModes & (1 << i);
        if (_oFlagTable[oFlag] != -1)
        {
            oFlags |= _oFlagTable[oFlag];
        }
    }

    return oFlags;
}
