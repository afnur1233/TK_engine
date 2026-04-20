#include <TK/TK.h>
#include <cglm/struct.h>

int main()
{
    TK_AllocatorTracker tracker = TK_AllocatorTracker( &TK_AllocatorLibc );
    TK_Allocator *allocator = &tracker.allocator;
    
    TK_EntityContainer ECS = { .allocator = allocator };
    
    TK_Entity root = TK_CreateEntity( &ECS, TK_INVALID_ENTITY );
    TK_CoreAssert( root != TK_INVALID_ENTITY );
    
    TK_Entity cube = TK_CreateEntity( &ECS, root );
    TK_CoreAssert( cube != TK_INVALID_ENTITY );
    
    TK_Entity_SetPosition3( &ECS, cube, (vec4){ 0.0f, 0.0f, 2.0f } );
    TK_Entity_SetCubeMesh( &ECS, cube );
    
    ECS = TK_EntityContainer_Free(ECS);
    
    TK_CoreAssert( tracker.first == NULL, "This will trigger on a memory leak!" );
}