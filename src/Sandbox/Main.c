#include <TK/TK.h>

typedef struct TK_EntityContainer TK_EntityContainer;

struct TK_EntityContainer
{
    
};

#define TK_EntityContainer(...)

int main()
{
    TK_AllocatorTracker tracker = TK_AllocatorTracker( &TK_AllocatorLibc );
    TK_Allocator *allocator = &tracker.allocator;
    
    TK_EntityContainer ECS = TK_EntityContainer( .allocator = allocator );
    
    TK_IFile file = TK_IOpen("Shader.glsl")
    TK_IStream *stream = &file.stream;
}