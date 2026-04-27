#include <TK/TK_entity.h>
#include <TK/TK_assert.h>

#ifdef __cplusplus
extern "C" {
#endif



TK_Entity *TK_FindFreeEntity( TK_EntityContainer *container )
{
    TK_DebugAssert(container != NULL);
    TK_DebugAssert(container->allocator != NULL);
    
    TK_usize pageIndex = 0;
    for ( TK_EntityPage *page = container->first ; page;)
    {
        if ( page->emptySpace == 0 )
        {
            pageIndex++;
            page = page->next;
            if ( page == container->first )
                break;
            
            continue;
        }
        
        for ( TK_usize entityIndex = 0; entityIndex < TK_ENTITY_PAGE_SIZE; ++entityIndex )
            if ( !page->entities[entityIndex].isAlive )
            {
                page->emptySpace--;
                return &page->entities[entityIndex];
            }
    }
    
    return NULL;
}



TK_Entity *TK_CreateEntity_o( TK_EntityContainer *container, TK_EntitySpecs specs )
{
    TK_DebugAssert(container != NULL);
    TK_DebugAssert(container->allocator != NULL);
    TK_DebugAssert(specs.type < TK_EntityType_MAX, "Invalid entity specs");
    
    TK_Allocator *allocator = container->allocator;
    TK_Entity *entity = specs.entity ? specs.entity : TK_FindFreeEntity(container);
    
    if ( entity == NULL )
        if ( !container->first )
        {
            container->first = allocator->alloc(allocator, sizeof(*container->first));
            if ( container->first == NULL )
                return NULL;
            
            *container->first = (TK_EntityPage){
                .next = container->first,
                .prev = container->first,
                .emptySpace = TK_ENTITY_PAGE_SIZE,
            };
            container->last = container->first;
            
            entity = &container->first->entities[0];
        }
        else
        {
            TK_EntityPage *newPage = allocator->alloc(allocator, sizeof(*newPage));
            if ( newPage == NULL )
                return NULL;
            
            *newPage = (TK_EntityPage){
                .next = container->first,
                .prev = container->last,
                .emptySpace = TK_ENTITY_PAGE_SIZE,
            };
            container->last->next = newPage;
            container->first->prev = newPage;
            
            entity = &newPage->entities[0];
        }
    
    entity->isAlive = true;
    entity->children = NULL;
    entity->next = NULL;
    entity->prev = NULL;
    entity->nickname = specs.nickname;
    entity->type = specs.type;
    entity->value = specs.value;
    entity->parent = NULL;
    
    if ( specs.children )
        for ( TK_usize i = 0; i < specs.childCount; ++i )
        {
            TK_EntitySpecs childSpecs = specs.children[i];
            childSpecs.parent = entity;
            
            TK_Entity *child = TK_CreateEntity_o(container, childSpecs);
            if ( child == NULL )
            {
                entity->isAlive = false;
                return NULL;
            }
        }
    
    if ( specs.parent )
    {
        TK_Entity *parent = specs.parent;
        
        entity->parent = parent;
        
        if ( !parent->children )
        {
            parent->children = entity;
            entity->next = entity;
            entity->prev = entity;
        }
        else
        {
            TK_Entity *lastChild = parent->children->prev;
            TK_Entity *firstChild = parent->children;
            
            entity->next = firstChild;
            entity->prev = lastChild;
            lastChild->next = entity;
            firstChild->prev = entity;
        }
    }
    
    return entity;
}



TK_EntityContainer TK_DestroyEntityContainer( TK_EntityContainer container )
{
    TK_DebugAssert(container.allocator != NULL);
    if ( container.first == NULL )
        return (TK_EntityContainer){0};
    
    TK_Allocator *allocator = container.allocator;
    TK_EntityPage *page = container.last;
    while ( page )
    {
        TK_EntityPage *next = page->next;
        TK_EntityPage *prev = page->prev;
        TK_bool last = page->next == page;
            
        allocator->free(allocator, page, sizeof(*page));
        
        if ( last )
            break;
        
        next->prev = prev;
        prev->next = next;
        page = prev;
    };
    
    return (TK_EntityContainer){0};
}



#ifdef __cplusplus
}
#endif
