#include <TK/ECS/TK_component.c>
#include <TK/ECS/TK_ecs.h>
#include <TK/TK_assert.h>


TK_EntityComponentBitset TK_Entity_GetBitset( TK_EntityContainer *container, TK_Entity entity )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert( ( entity / TK_ENTITY_PAGE_SIZE ) < container->pages_cap, "Invalid entity ID" );
    
    TK_EntityData *page = container->pages[entity / TK_ENTITY_PAGE_SIZE];
    TK_EntityData *data = &page[entity % TK_ENTITY_PAGE_SIZE];
    
    return data->bitset; 
}

TK_EntityContainer TK_EntityContainer_Free( TK_EntityContainer container )
{
    container.allocator->free( container.allocator, container.pages,  container.pages_cap * sizeof ( *container.pages ) );
    
    return (TK_EntityContainer){ 0 };
}

TK_Entity TK_PoolFreeEntity( TK_EntityContainer *container )
{
    TK_DebugAssert( container != NULL );
    
    for (TK_usize pageIndex = 0; pageIndex < container->pages_cap; ++pageIndex)
    {
        const TK_EntityData *page = container->pages[pageIndex];
        
        for (TK_usize entityIndex = 0; entityIndex < TK_ENTITY_PAGE_SIZE; ++entityIndex)
            if ( page[entityIndex].alive )
                continue;
            else
                return ( pageIndex * TK_ENTITY_PAGE_SIZE ) + entityIndex;
    }
    
    return TK_INVALID_ENTITY;
}

TK_Entity TK_CreateEntity( TK_EntityContainer *container, TK_Entity parent )
{
    TK_DebugAssert( container != NULL );
    TK_DebugAssert(
        (0
        || ( parent / TK_ENTITY_PAGE_SIZE ) < container->pages_cap 
        || parent == TK_INVALID_ENTITY),
        "Invalid parent entity ID"
    );
    
    TK_Entity entity = TK_PoolFreeEntity( container );
    if ( entity != TK_INVALID_ENTITY )
        return entity;
    
    TK_Allocator *allocator = container->allocator;
    TK_usize new_cap = container->pages
        ? TK_ENTITY_PAGES_GROW_AMOUNT( container->pages_cap )
        : 1
        ;
    
    if ( container->pages )
        container->pages = allocator->realloc(
            allocator,
            container->pages,
            sizeof ( *container->pages ) * container->pages_cap,
            sizeof ( *container->pages ) * new_cap
        );
    else
        container->pages = allocator->alloc(
            allocator,
            sizeof ( *container->pages ) * new_cap
        );
    
    if ( !container->pages )
    {
        container->pages_cap = 0;
        return TK_INVALID_ENTITY;
    }
    
    TK_EntityData *page = container->pages[container->pages_cap];
    
    for (TK_usize i = 0; i < TK_ENTITY_PAGE_SIZE; ++i)
        page[i] = (TK_EntityData){ 0 };
    
    entity = container->pages_cap * TK_ENTITY_PAGE_SIZE;
    
    page[0] = (TK_EntityData){ .alive = true };
    
    container->pages_cap = new_cap;
    
    return entity;
}