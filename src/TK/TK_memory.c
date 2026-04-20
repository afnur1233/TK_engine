#include <TK/TK_memory.h>

TK_Allocator TK_AllocatorLibc = {
    .realloc = TK_AllocatorLibc_Realloc,
    .free    = TK_AllocatorLibc_Free,
    .alloc   = TK_AllocatorLibc_Alloc,
};

void *TK_AllocatorLibc_Realloc( TK_Allocator *allocator, void *memory, TK_usize size_old, TK_usize size_new )
{
    if ( !allocator || !memory || size_old == 0 || size_new == 0 )
        return NULL;
    
    return realloc( memory, size_new );
}

void *TK_AllocatorLibc_Free( TK_Allocator *allocator, void *memory, TK_usize size )
{
    if ( !allocator || !memory || size == 0 )
        return NULL;
    
    free( memory );
    
    return NULL;
}

void *TK_AllocatorLibc_Alloc( TK_Allocator *allocator, TK_usize size )
{
    if ( !allocator || size == 0 )
        return NULL;
    
    return malloc(size);
}

void *TK_AllocatorTracker_Realloc( TK_Allocator *ptr, void *memory, TK_usize size_old, TK_usize size_new )
{
    if ( !ptr || !memory || size_old == 0 || size_new == 0 )
        return NULL;
    
    TK_AllocatorTracker *tracker = TK_REINTERPRET_CAST( tracker, ptr );
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
    if ( !ptr || !memory || size == 0 )
        return NULL;
    
    TK_AllocatorTracker *tracker = TK_REINTERPRET_CAST( tracker, ptr );
    TK_Allocator *allocator = tracker->tracked;
    if ( !allocator )
        return NULL;
    
    TK_AllocatorFreeFunc *f_free = allocator->free;
    if ( !f_free )
        return NULL;
    
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
    if ( !ptr || size == 0 )
        return NULL;
    
    TK_AllocatorTracker *tracker = TK_REINTERPRET_CAST( tracker, ptr );
    TK_Allocator *allocator = tracker->tracked;
    if ( !allocator )
        return NULL;
    
    TK_AllocatorAllocFunc *f_alloc = allocator->alloc;
    if ( !f_alloc )
        return NULL;
    
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
    if ( !ptr || !memory || size_old == 0 || size_new == 0 )
        return NULL;
    
    TK_AllocatorFixedArena *fixedArena = TK_REINTERPRET_CAST( fixedArena, ptr );
    
    if ( memory != fixedArena->lastAlloc )
    {
        TK_byte *reallocated = TK_AllocatorFixedArena_Alloc( ptr, size_new );
        if (!reallocated)
            return NULL;
        
        for ( TK_usize i = 0; i < ( size_old < size_new ) ? size_old : size_new; ++i )
        {
            reallocated[i] = ( ( TK_byte * ) memory )[i];
        }
        
        return reallocated;
    }
    else if ( size_new <= ( fixedArena->size - fixedArena->pos ) )
    {
        fixedArena->pos = ( TK_usize )( fixedArena->lastAlloc - fixedArena->buf ) + size_new;
        
        return memory;
    }
    
    return NULL;
}

void *TK_AllocatorFixedArena_Alloc( TK_Allocator *ptr, TK_usize size )
{
    if ( !ptr )
        return NULL;
    
    TK_AllocatorFixedArena *fixedArena = TK_REINTERPRET_CAST( fixedArena, ptr );
    
    if ( size > ( fixedArena->size - fixedArena->pos ) )
        return NULL;
    
    void *allocation = fixedArena->buf + fixedArena->pos;
    
    fixedArena->lastAlloc = allocation;
    fixedArena->pos += size;
    
    return allocation;
}
