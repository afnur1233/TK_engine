#define _FILE_OFFSET_BITS 64
#include <TK/TK_filesys.h>
#include <TK/TK_assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif



/****************************************************************
 * IFile functions
 ****************************************************************/



TK_bool __TK_IFile_Stream_AtEnd( TK_IStream *self )
{
    TK_DebugAssert(self != NULL);
    
    TK_IFile *file = (TK_IFile *)(void *)self;
    
    FILE *handle = (FILE *)file->nativeHandle;
    TK_DebugAssert(handle != NULL);
    
    return !!(feof(handle));
}



TK_bool __TK_IFile_Stream_GetError( TK_IStream *self, TK_StringSlice *description )
{
    TK_DebugAssert(self != NULL);
    
    TK_IFile *file = (TK_IFile *)(void *)self;
    if ( !file->_detail.hasError )
        return TK_False;
    
    if ( description )
        *description = TK_CreateStringSlice(strerror(file->_detail.error));
    
    return TK_True;
}



TK_usize __TK_IFile_Stream_Size( TK_IStream *self )
{
    TK_DebugAssert(self != NULL);
    
    TK_IFile *file = (TK_IFile *)(void *)self;
    
    FILE *handle = (FILE *)file->nativeHandle;
    TK_DebugAssert(handle != NULL);
    
    fpos_t original;
    if ( fgetpos(handle, &original) == 0 )
        goto fail;
    
    if ( fseek(handle, 0, SEEK_END) == 0 )
        goto fail;
    
    TK_usize size = ftell(handle);
    if ( size == -1 )
        goto fail;
    
    if ( fsetpos(handle, &original) == 0 )
        goto fail;
    
fail:
    file->_detail.hasError = true;
    file->_detail.error = errno;
    return 0;
}



TK_usize __TK_IFile_Stream_Where( TK_IStream *self )
{
    TK_DebugAssert(self != NULL);
    
    TK_IFile *file = (TK_IFile *)(void *)self;
    
    FILE *handle = (FILE *)file->nativeHandle;
    TK_DebugAssert(handle != NULL);
    
    TK_usize where = ftell(handle);
    if ( where == -1 )
    {
        file->_detail.hasError = true;
        file->_detail.error = errno;
        return 0;
    }
    
    return where;
}



TK_bool __TK_IFile_Stream_Go( TK_IStream *self, TK_StreamOrigin origin, TK_isize offset )
{
    TK_DebugAssert(self != NULL);
    TK_DebugAssert(origin < 4, "Invalid origin");
    
    TK_IFile *file = (TK_IFile *)(void *)self;
    
    FILE *handle = (FILE *)file->nativeHandle;
    TK_DebugAssert(handle != NULL);
    
    
    switch (origin)
    {
        case TK_STREAM_GO_FROM_BEGIN:
            if ( fseek(handle, offset, SEEK_SET) != 0 )
                goto fail;
        
        case TK_STREAM_GO_FROM_END:
            if ( fseek(handle, -offset, SEEK_END) != 0 )
                goto fail;
        
        case TK_STREAM_GO_FORWARD:
            if ( fseek(handle, offset, SEEK_CUR) != 0 )
                goto fail;
        
        case TK_STREAM_GO_BACK:
            if ( fseek(handle, -offset, SEEK_CUR) != 0 )
                goto fail;
    }
    
    return TK_True;
fail:
    file->_detail.hasError = true;
    file->_detail.error = errno;
    return TK_False;
}



TK_usize __TK_IFile_Stream_Read( TK_IStream *self, void *data, TK_usize size )
{
    TK_DebugAssert(self != NULL);
    
    TK_IFile *file = (TK_IFile *)(void *)self;
    
    FILE *handle = (FILE *)file->nativeHandle;
    TK_DebugAssert(handle != NULL);
    
    TK_usize read = fread(data, 1, size, handle);
    if ( !ferror(handle) )
        return read;
    
    file->_detail.hasError = true;
    file->_detail.error = errno;
    return read;
}



TK_IFile TK_OpenIFile_o( TK_OpenFileOpt opt )
{
    TK_DebugAssert(opt.path.buf != NULL, "No path was specified");
    
    char *pathCString = alloca(opt.path.len + 1);
    for ( TK_usize i = 0; i < opt.path.len; ++i )
        pathCString[i] = opt.path.buf[i];
    
    FILE *handle = fopen(pathCString, "r");
    
    return (TK_IFile){
        ._detail.hasError = handle == NULL,
        ._detail.error = errno,
        .istream = {
            .atend    = __TK_IFile_Stream_AtEnd,
            .geterror = __TK_IFile_Stream_GetError,
            .size     = __TK_IFile_Stream_Size,
            .where    = __TK_IFile_Stream_Where,
            .go       = __TK_IFile_Stream_Go,
            .read     = __TK_IFile_Stream_Read,
        },
        .nativeHandle = handle,
    };
}



/****************************************************************
 * OFile functions
 ****************************************************************/



TK_usize __TK_OFile_Stream_Write( TK_OStream *self, void *data, TK_usize size )
{
    TK_DebugAssert(self != NULL);
    
    TK_OFile *file = (TK_OFile *)(void *)self;
    
    FILE *handle = (FILE *)file->nativeHandle;
    TK_DebugAssert(handle != NULL);
    
    TK_usize wrote = fwrite(data, 1, size, handle);
    if ( !ferror(handle) )
        return wrote;
    
    file->_detail.hasError = true;
    file->_detail.error = errno;
    return wrote;
}




TK_OFile TK_OpenOFile_o( TK_OpenFileOpt opt )
{
    TK_DebugAssert(opt.path.buf != NULL, "No path was specified");
    
    char *pathCString = alloca(opt.path.len + 1);
    for ( TK_usize i = 0; i < opt.path.len; ++i )
        pathCString[i] = opt.path.buf[i];
    
    FILE *handle = fopen(pathCString, opt.append ? "a" : "w");
    
    return (TK_OFile){
        ._detail.hasError = handle == NULL,
        ._detail.error = errno,
        .ostream = {
            .atend    = (void *)__TK_IFile_Stream_AtEnd,
            .geterror = (void *)__TK_IFile_Stream_GetError,
            .size     = (void *)__TK_IFile_Stream_Size,
            .where    = (void *)__TK_IFile_Stream_Where,
            .go       = (void *)__TK_IFile_Stream_Go,
            .write    = (void *)__TK_OFile_Stream_Write,
        },
        .nativeHandle = handle,
    };
}



TK_IOFile TK_OpenIOFile_o( TK_OpenFileOpt opt )
{
    TK_DebugAssert(opt.path.buf != NULL, "No path was specified");
    
    char *pathCString = alloca(opt.path.len + 1);
    for ( TK_usize i = 0; i < opt.path.len; ++i )
        pathCString[i] = opt.path.buf[i];
    
    FILE *handle = fopen(pathCString, "a+");
    
    return (TK_IOFile){
        ._detail.hasError = handle == NULL,
        ._detail.error = errno,
        .iostream = {
            .atend    = (void *)__TK_IFile_Stream_AtEnd,
            .geterror = (void *)__TK_IFile_Stream_GetError,
            .size     = (void *)__TK_IFile_Stream_Size,
            .where    = (void *)__TK_IFile_Stream_Where,
            .go       = (void *)__TK_IFile_Stream_Go,
            .read     = (void *)__TK_IFile_Stream_Read,
            .write    = (void *)__TK_OFile_Stream_Write,
        },
        .nativeHandle = handle,
    };
}



TK_bool TK_CloseIFile( TK_IFile *file )
{
    TK_DebugAssert( file != NULL );
    
    FILE *handle = file->nativeHandle;
    TK_DebugAssert( handle != NULL );
    
    fclose(handle);
}



TK_bool TK_CloseOFile( TK_OFile *file )
{
    TK_DebugAssert( file != NULL );
    
    FILE *handle = file->nativeHandle;
    TK_DebugAssert( handle != NULL );
    
    fclose(handle);
}



TK_bool TK_CloseIOFile( TK_IOFile *file )
{
    TK_DebugAssert( file != NULL );
    
    FILE *handle = file->nativeHandle;
    TK_DebugAssert( handle != NULL );
    
    fclose(handle);
}

#ifdef __cplusplus
}
#endif