#include <TK/TK.h>

int main()
{
    TK_AllocatorTracker tracker = TK_AllocatorTracker(&TK_AllocatorLibc);
    TK_Allocator *allocator = &tracker.allocator;
    
    TK_IOFile file = TK_OpenIOFile(.path=TK_CreateStringSlice("../src/Sandbox/Main.c"));
    TK_IOStream *istream = &file.iostream;
    TK_StringSlice error = {0};
    if (istream->geterror(istream, &error))
    {
        printf("[ERROR][%s:%i] Failed to open file: %.*s\n", __FILE__, __LINE__, error.len, error.buf);
        return -1;
    }
    
    TK_String source = {0};
    for ( char buffer[256]; ; )
    {
        TK_usize read = istream->read(istream, buffer, sizeof(buffer));
        if (istream->geterror(istream, &error))
        {
            printf("[ERROR][%s:%i] Failed to read file: %.*s\n", __FILE__, __LINE__, error.len, error.buf);
            return -1;
        }
        
        if (!TK_StringAppend(allocator, &source, buffer, read))
        {
            printf("[ERROR][%s:%i] Failed to allocate memory\n", __FILE__, __LINE__);
            return -1;
        }
        
        if (istream->atend(istream))
            break;
        
        if (istream->geterror(istream, &error))
        {
            printf("[ERROR][%s:%i] Failed to read file: %.*s\n", __FILE__, __LINE__, error.len, error.buf);
            return -1;
        }
    }
    
    
    printf("[INFO][%s:%i] This files source code:\n", __FILE__, __LINE__);
    printf("%.*s\n", source.len, source.buf);
    
    source = TK_StringFree(allocator, source);
    TK_CloseFile(&file);
}