#include <TK/TK_memory.h>
#include <TK/TK_assert.h>



TK_Allocator TK_AllocatorLibc = {
    .realloc = TK_AllocatorLibc_Realloc,
    .free    = TK_AllocatorLibc_Free,
    .alloc   = TK_AllocatorLibc_Alloc,
};



void *TK_AllocatorLibc_Realloc( TK_Allocator *allocator, void *memory, TK_usize size_old, TK_usize size_new )
{
    TK_DebugAssert( allocator != NULL );
    TK_DebugAssert( memory != NULL );
    TK_DebugAssert( size_old != 0 );
    TK_DebugAssert( size_new != 0 );
    
    return realloc( memory, size_new );
}



void *TK_AllocatorLibc_Free( TK_Allocator *allocator, void *memory, TK_usize size )
{
    TK_DebugAssert( allocator != NULL );
    TK_DebugAssert( memory != NULL );
    TK_DebugAssert( size != 0 );
    
    free( memory );
    
    return NULL;
}



void *TK_AllocatorLibc_Alloc( TK_Allocator *allocator, TK_usize size )
{
    TK_DebugAssert( allocator != NULL );
    TK_DebugAssert( size != 0 );
    
    return malloc(size);
}



void *TK_AllocatorTracker_Realloc( TK_Allocator *ptr, void *memory, TK_usize size_old, TK_usize size_new )
{
    TK_DebugAssert( ptr != NULL );
    TK_DebugAssert( memory != NULL );
    TK_DebugAssert( size_old != 0 );
    TK_DebugAssert( size_new != 0 );
    
    TK_AllocatorTracker *tracker = (TK_AllocatorTracker *)(void *)ptr;
    TK_Allocator *allocator = tracker->tracked;
    if ( !allocator )
        return NULL;
    
    TK_AllocatorReallocFunc *f_realloc = allocator->realloc;
    if ( !f_realloc )
        return NULL;
    
    TK_AllocatorTrackerAllocation *old_allocation = memory - sizeof ( *old_allocation );
    TK_AllocatorTrackerAllocation *old_next = old_allocation->next;
    TK_AllocatorTrackerAllocation *old_prev = old_allocation->prev;
    TK_AllocatorTrackerAllocation *allocation = f_realloc(
        allocator ,
        old_allocation ,
        size_old + sizeof ( *allocation ) ,
        size_new + sizeof ( *allocation )
    );
    
    old_next->prev = allocation;
    old_prev->next = allocation;
    
    if ( !allocation )
        return NULL;
    
    allocation->next = old_next;
    allocation->prev = old_prev;
    
    return allocation + 1;
}



void *TK_AllocatorTracker_Free( TK_Allocator *ptr, void *memory, TK_usize size )
{
    TK_DebugAssert( ptr != NULL );
    TK_DebugAssert( memory != NULL );
    TK_DebugAssert( size != 0 );
    
    TK_AllocatorTracker *tracker = (TK_AllocatorTracker *)(void *)ptr;
    TK_DebugAssert( tracker->tracked != NULL );
    TK_DebugAssert( tracker->tracked->free != NULL );
    
    TK_Allocator *allocator = tracker->tracked;
    TK_AllocatorFreeFunc *f_free = allocator->free;
    
    TK_AllocatorTrackerAllocation *allocation = memory - sizeof ( *allocation );
    if ( allocation->next == allocation )
    {
        tracker->first = NULL;
        tracker->last  = NULL;
    }
    else
    {
        allocation->next->prev = allocation->prev;
        allocation->prev->next = allocation->next;
    }
    
    return f_free(allocator, allocation, size);
}



void *TK_AllocatorTracker_Alloc( TK_Allocator *ptr, TK_usize size )
{
    TK_DebugAssert( ptr != NULL );
    TK_DebugAssert( size != 0 );
    
    TK_AllocatorTracker *tracker = (TK_AllocatorTracker *)(void *)ptr;
    TK_DebugAssert( tracker->tracked != NULL );
    TK_DebugAssert( tracker->tracked->alloc != NULL );
    
    TK_Allocator *allocator = tracker->tracked;
    TK_AllocatorAllocFunc *f_alloc = allocator->alloc;
    
    TK_AllocatorTrackerAllocation *allocation = f_alloc(
        allocator,
        size + sizeof ( *allocation )
    );
    
    if ( !allocation )
        return NULL;
    
    if ( !tracker->first )
    {
        tracker->last    = allocation;
        tracker->first   = allocation;
        allocation->next = allocation;
        allocation->prev = allocation;
    }
    else
    {
        tracker->last->next  = allocation;
        tracker->first->prev = allocation;
        allocation->next     = tracker->first;
        allocation->prev     = tracker->last;
        tracker->last        = allocation;
    }
    
    return allocation + 1;
    
}



void *TK_AllocatorFixedArena_Realloc( TK_Allocator *ptr, void *memory, TK_usize size_old, TK_usize size_new )
{
    TK_DebugAssert( ptr != NULL );
    TK_DebugAssert( memory != NULL );
    TK_DebugAssert( size_old != 0 );
    TK_DebugAssert( size_new != 0 );
    
    TK_AllocatorFixedArena *fixedArena = (TK_AllocatorFixedArena *)(void *)ptr;
    
    const TK_usize remaningSpace = fixedArena->size - fixedArena->pos;
    if ( memory == fixedArena->lastAlloc )
    {
        fixedArena->pos = ( TK_usize )( fixedArena->lastAlloc - fixedArena->buf ) + size_new;
        
        return memory;
    }
    else if ( size_new <= remaningSpace )
    {
        TK_byte *reallocated = TK_AllocatorFixedArena_Alloc( ptr, size_new );
        if (!reallocated)
            return NULL;
        
        for ( TK_usize i = 0; i < ( size_old < size_new ) ? size_old : size_new; ++i )
            reallocated[i] = ( ( TK_byte * ) memory )[i];
        
        return reallocated;
    }
    
    return NULL;
}



void *TK_AllocatorFixedArena_Alloc( TK_Allocator *ptr, TK_usize size )
{
    TK_DebugAssert( ptr != NULL );
    TK_DebugAssert( size != 0 );
    
    TK_AllocatorFixedArena *const fixedArena = (TK_AllocatorFixedArena *)(void *)ptr;
    
    const TK_usize remaningSpace = fixedArena->size - fixedArena->pos;
    if ( size > remaningSpace )
        return NULL;
    
    void *allocation = fixedArena->buf + fixedArena->pos;
    
    fixedArena->lastAlloc = allocation;
    fixedArena->pos += size;
    
    return allocation;
}
