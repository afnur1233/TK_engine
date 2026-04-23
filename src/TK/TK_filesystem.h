#ifndef __TK_FILESYSTEM_H__
#define __TK_FILESYSTEM_H__
#include <TK/TK_stream.h>
#include <TK/TK_macro.h>
#include <stdio.h>

typedef FILE *TK_CFileHandle;
typedef struct TK_IFile  TK_IFile;
typedef struct TK_OFile  TK_OFile;
typedef struct TK_IOFile TK_IOFile;

struct TK_IFile
{
    TK_CFileHandle handle;
    TK_IStream stream;
};

struct TK_OFile
{
    TK_CFileHandle handle;
    TK_OStream stream;
};

struct TK_IOFile
{
    TK_CFileHandle handle;
    TK_IOStream stream;
};

#endif