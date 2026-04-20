#include <TK/ECS/TK_ecs.h>
#include <TK/TK_assert.h>

void TK_Entity_SetPosition3( TK_EntityContainer *container, TK_Entity entity, vec3 position3 )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert( ( entity / TK_ENTITY_PAGE_SIZE ) < container->pages_cap, "Invalid entity ID" );
    
    TK_EntityData *page = container->pages[entity / TK_ENTITY_PAGE_SIZE];
    TK_EntityData *data = &page[entity % TK_ENTITY_PAGE_SIZE];
    
    data->bitset |= TK_EntityComponentBit_Position3;
    
    glm_vec3_dup(position3, data->position3);
}

void TK_Entity_SetScaling3( TK_EntityContainer *container, TK_Entity entity, vec3 scaling3 )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert( ( entity / TK_ENTITY_PAGE_SIZE ) < container->pages_cap, "Invalid entity ID" );
    
    TK_EntityData *page = container->pages[entity / TK_ENTITY_PAGE_SIZE];
    TK_EntityData *data = &page[entity % TK_ENTITY_PAGE_SIZE];
    
    data->bitset |= TK_EntityComponentBit_Scaling3;
    
    glm_vec3_dup(scaling3, data->scaling3);
}

void TK_Entity_SetCubeMesh( TK_EntityContainer *container, TK_Entity entity )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert( ( entity / TK_ENTITY_PAGE_SIZE ) < container->pages_cap, "Invalid entity ID" );
    
    TK_EntityData *page = container->pages[entity / TK_ENTITY_PAGE_SIZE];
    TK_EntityData *data = &page[entity % TK_ENTITY_PAGE_SIZE];
    
    data->bitset |= TK_EntityComponentBit_CubeMesh;
}


void TK_Entity_GetPosition3( TK_EntityContainer *container, TK_Entity entity, vec3 dest )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert( ( entity / TK_ENTITY_PAGE_SIZE ) < container->pages_cap, "Invalid entity ID" );
    
    TK_EntityData *page = container->pages[entity / TK_ENTITY_PAGE_SIZE];
    TK_EntityData *data = &page[entity % TK_ENTITY_PAGE_SIZE];
    
    data->bitset |= TK_EntityComponentBit_Position3;
    
    glm_vec3_dup(data->position3, dest);
}

void TK_Entity_GetScaling3( TK_EntityContainer *container, TK_Entity entity, vec3 dest )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert( ( entity / TK_ENTITY_PAGE_SIZE ) < container->pages_cap, "Invalid entity ID" );
    
    TK_EntityData *page = container->pages[entity / TK_ENTITY_PAGE_SIZE];
    TK_EntityData *data = &page[entity % TK_ENTITY_PAGE_SIZE];
    
    data->bitset |= TK_EntityComponentBit_Scaling3;
    
    glm_vec3_dup(data->scaling3, dest);
}