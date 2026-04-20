#ifndef __TK_MEMORY_H__
#define __TK_MEMORY_H__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <TK/TK_stdint.h>
#include <TK/TK_macro.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TK_REINTERPRET_CAST(T, ...)                      \
    ({                                                   \
        __typeof__(__VA_ARGS__) __value = (__VA_ARGS__); \
        __typeof__(T) *casted = (void *)&__value;        \
        *casted;                                         \
    })

typedef struct TK_Allocator                  TK_Allocator;
typedef struct TK_AllocatorFixedArena        TK_AllocatorFixedArena;
typedef struct TK_AllocatorTrackerAllocation TK_AllocatorTrackerAllocation;
typedef struct TK_AllocatorTracker           TK_AllocatorTracker;

typedef void *TK_AllocatorReallocFunc ( TK_Allocator *, void *memory, TK_usize size_old, TK_usize size_new );
typedef void *TK_AllocatorFreeFunc    ( TK_Allocator *, void *memory, TK_usize size );
typedef void *TK_AllocatorAllocFunc   ( TK_Allocator *, TK_usize size );

#define TK_ALLOCATOR_IMPL()                   \
    union                                     \
    {                                         \
        TK_Allocator allocator;               \
        struct                                \
        {                                     \
            TK_AllocatorReallocFunc *realloc; \
            TK_AllocatorFreeFunc    *free;    \
            TK_AllocatorAllocFunc   *alloc;   \
        };                                    \
    }

struct TK_Allocator
{
    TK_AllocatorReallocFunc *realloc;
    TK_AllocatorFreeFunc    *free;
    TK_AllocatorAllocFunc   *alloc;
};

struct TK_AllocatorTrackerAllocation
{
    TK_AllocatorTrackerAllocation *next;
    TK_AllocatorTrackerAllocation *prev;
    TK_usize size;
    TK_u8 data[];
};

struct TK_AllocatorTracker
{
    TK_ALLOCATOR_IMPL();
    TK_Allocator *tracked;
    TK_AllocatorTrackerAllocation *first;
    TK_AllocatorTrackerAllocation *last;
};

#define TK_AllocatorTracker(...)                \
    ((TK_AllocatorTracker){                     \
        .realloc = TK_AllocatorTracker_Realloc, \
        .free    = TK_AllocatorTracker_Free,    \
        .alloc   = TK_AllocatorTracker_Alloc,   \
        .tracked = (__VA_ARGS__),               \
        .first   = NULL,                        \
        .last    = NULL,                        \
    })                                          \

struct TK_AllocatorFixedArena
{
    TK_ALLOCATOR_IMPL();
    void    *lastAlloc;
    TK_usize pos;
    void    *buf;
    TK_usize size;
};

#define TK_AllocatorFixedArena(...)                  \
    ((TK_AllocatorFixedArena){                       \
        .realloc   = TK_AllocatorFixedArena_Realloc, \
        .free      = NULL,                           \
        .alloc     = TK_AllocatorFixedArena_Alloc,   \
        .lastAlloc = NULL,                           \
        .pos       = 0,                              \
        __VA_ARGS__                                  \
    })

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TKAPI void *TK_AllocatorLibc_Realloc ( TK_Allocator *, void *memory, TK_usize size_old, TK_usize size_new );
TKAPI void *TK_AllocatorLibc_Free    ( TK_Allocator *, void *memory, TK_usize size );
TKAPI void *TK_AllocatorLibc_Alloc   ( TK_Allocator *, TK_usize size );
TKAPI void *TK_AllocatorTracker_Realloc ( TK_Allocator *, void *memory, TK_usize size_old, TK_usize size_new );
TKAPI void *TK_AllocatorTracker_Free    ( TK_Allocator *, void *memory, TK_usize size );
TKAPI void *TK_AllocatorTracker_Alloc   ( TK_Allocator *, TK_usize size );
TKAPI void *TK_AllocatorFixedArena_Realloc ( TK_Allocator *, void *memory, TK_usize size_old, TK_usize size_new );
TKAPI void *TK_AllocatorFixedArena_Alloc   ( TK_Allocator *, TK_usize size );

TKAPI TK_Allocator TK_AllocatorLibc;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif