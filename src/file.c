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

Bool openFile(char *__restrict pFileName, File *__restrict pFile)
{
#if defined (OS_LINUX) || defined (OS_APPLE)
    pFile->fd = open(pFileName, O_RDONLY);
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
