#ifndef __TK_ENTITY_H__
#define __TK_ENTITY_H__
#include <TK/TK_stdint.h>
#include <TK/TK_memory.h>
#include <TK/TK_string.h>
#include <cglm/struct.h>

typedef struct TK_Entity          TK_Entity;
typedef union  TK_EntityValue     TK_EntityValue;
typedef struct TK_EntityPage      TK_EntityPage;
typedef struct TK_EntityContainer TK_EntityContainer;
typedef struct TK_EntitySpecs     TK_EntitySpecs;

enum TK_EntityType
{
    TK_EntityType_None,
    TK_EntityType_Position3,
    TK_EntityType_Scale3,
    TK_EntityType_CubeMesh,
    TK_EntityType_MAX,
};
typedef enum TK_EntityType TK_EntityType;

union TK_EntityValue
{
    vec3s position3;
    vec3s scale3;
};

struct TK_Entity
{
    TK_StringSlice nickname;
    TK_EntityType type;
    TK_Entity *parent;
    TK_Entity *children;
    TK_Entity *next;
    TK_Entity *prev;
    TK_bool isAlive;
    TK_EntityValue value;
};

#ifndef TK_ENTITY_PAGE_SIZE
    #define TK_ENTITY_PAGE_SIZE 1024
#endif

struct TK_EntityPage
{
    TK_EntityPage *next;
    TK_EntityPage *prev;
    TK_usize emptySpace;
    TK_Entity entities[TK_ENTITY_PAGE_SIZE];
};

struct TK_EntityContainer
{
    TK_EntityPage *first;
    TK_EntityPage *last;
    TK_Allocator *allocator;
};

struct TK_EntitySpecs
{
    TK_Entity *entity;
    TK_StringSlice nickname;
    TK_Entity *parent;
    TK_EntityType type;
    TK_EntityValue value;
    TK_usize childCount;
    TK_EntitySpecs *children;
};

#define TK_ForeachChild(iteratorVariable, ...)                                  \
    for ( TK_bool __FOR=true;                            __FOR; __FOR = false ) \
    for ( TK_Entity *__Entity = (__VA_ARGS__)->children; __FOR; __FOR = false ) \
    for ( TK_Entity *iteratorVariable = __Entity;        __FOR; __FOR = false ) \
    if  ( __Entity )                                                            \
                                                                                \
    for ( ;;                                                                    \
        __FOR = false,                                                          \
        iteratorVariable = iteratorVariable->next,                              \
        ({ if (iteratorVariable == __Entity) break; })                          \
    )

#define TK_CreateEntity( container, ... ) TK_CreateEntity_o((container), (TK_EntitySpecs){ __VA_ARGS__ })

TK_Entity *TK_FindFreeEntity( TK_EntityContainer *container );
TK_Entity *TK_CreateEntity_o( TK_EntityContainer *container, TK_EntitySpecs specs );
TK_EntityContainer TK_DestroyEntityContainer( TK_EntityContainer container );

#endif