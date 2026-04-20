#ifndef __TK_ECS_H__
#define __TK_ECS_H__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <TK/TK_stdint.h>
#include <TK/TK_memory.h>
#include <TK/TK_macro.h>
#include <cglm/struct.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct TK_EntityContainer TK_EntityContainer;
typedef struct TK_EntityData      TK_EntityData;
typedef TK_u64 TK_Entity;
typedef TK_u64 TK_EntityComponentBitset;
typedef void   TK_EntityCleanupFunc ( TK_EntityContainer *container, TK_Entity entity );
#define TK_INVALID_ENTITY ((TK_Entity)(-1))

enum : TK_EntityComponentBitset
{
    TK_EntityComponentBit_Position3,
    TK_EntityComponentBit_Scaling3,
    TK_EntityComponentBit_CubeMesh,
};

struct TK_EntityData
{
    TK_EntityComponentBitset  bitset;
    TK_EntityCleanupFunc     *cleanup;
    TK_bool                   alive;
    
    vec3                      position3;
    vec3                      scaling3;
};

#ifndef TK_ENTITY_PAGES_GROW_AMOUNT
    #define TK_ENTITY_PAGES_GROW_AMOUNT(...) ( (__VA_ARGS__) + 5 )
#endif

#ifndef TK_ENTITY_PAGE_SIZE
    #define TK_ENTITY_PAGE_SIZE 50
#endif

typedef TK_EntityData TK_EntityPage[TK_ENTITY_PAGE_SIZE];

struct TK_EntityContainer
{
    TK_Allocator  *allocator;
    TK_EntityPage *pages;
    TK_u64         pages_cap;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TK_EntityComponentBitset TK_Entity_GetBitset     ( TK_EntityContainer *container, TK_Entity entity );
TK_EntityContainer       TK_EntityContainer_Free ( TK_EntityContainer container );
void                     TK_Entity_SetPosition3  ( TK_EntityContainer *container, TK_Entity entity, vec3 position3 );
void                     TK_Entity_SetScaling3   ( TK_EntityContainer *container, TK_Entity entity, vec3 scaling3 );
void                     TK_Entity_SetCubeMesh   ( TK_EntityContainer *container, TK_Entity entity );
void                     TK_Entity_GetPosition3  ( TK_EntityContainer *container, TK_Entity entity, vec3 dest );
void                     TK_Entity_GetScaling3   ( TK_EntityContainer *container, TK_Entity entity, vec3 dest );
TK_Entity                TK_CreateEntity         ( TK_EntityContainer *container, TK_Entity parent );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif