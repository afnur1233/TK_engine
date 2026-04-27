#include <TK/TK_event.h>
#include <TK/TK_assert.h>

#ifdef __cplusplus
extern "C" {
#endif



void TK_FireEvent( TK_Allocator *allocator, TK_Event *event )
{
    TK_DebugAssert( allocator != NULL );
    TK_DebugAssert( event != NULL );
    
    TK_EventConnection *connection = event->last;
    if ( !connection )
        return;
    
    for (TK_bool continue_firing = TK_True; continue_firing;) {
        TK_EventConnection *prev = connection->prev;
        
        if ( prev == event->last )
            continue_firing = TK_False;
        
        if ( connection->handlers.lockVariable )
            *connection->handlers.lockVariable = TK_True;
        
        if ( connection->handlers.callback )
            connection->handlers.callback(event, connection->handlers.callbackArgument);
        
        if ( connection->once )
            if ( connection->keep )
                TK_DisconnectEventConnection(connection);
            else
                TK_DestroyEventConnection(allocator, connection);
        
        connection = prev;
    }
}



TK_EventConnection *TK_ConnectEvent_o( TK_Allocator *allocator, TK_Event *event, TK_EventConnectOptions options )
{
    TK_DebugAssert(allocator != NULL);
    TK_DebugAssert(event != NULL);
    
    TK_EventConnection *connection;
    
    if ( options.connection )
    {
        TK_DebugAssert(options.connection != NULL);
        connection = options.connection;
        
        if ( connection->connected )
            TK_DisconnectEventConnection(connection);
    }
    else
    {
        connection = allocator->alloc(allocator, sizeof(*connection));
        if ( !connection )
            return NULL;
    }
    
    connection->connected = event;
    
    if (event->first == NULL)
    {
        event->first = connection;
        event->last = connection;
        connection->next = connection;
        connection->prev = connection;
    }
    else
    {
        event->last->next = connection;
        event->first->prev = connection;
        connection->prev = event->last;
        connection->next = event->first;
    }
    
    connection->once = options.once;
    connection->keep = options.keep;
    
    if ( options.connection )
    {
        if ( options.callback )
            connection->handlers.callback = options.callback;
        
        if ( options.callbackArgument )
            connection->handlers.callbackArgument = options.callbackArgument;
        
        if ( options.lockVariable )
            connection->handlers.lockVariable = options.lockVariable;
    }
    else
    {
        connection->handlers.callback         = options.callback;
        connection->handlers.callbackArgument = options.callbackArgument;
        connection->handlers.lockVariable     = options.lockVariable;
    }
    
    return connection;
}



TK_EventConnection *TK_DestroyEventConnection( TK_Allocator *allocator, TK_EventConnection *connection )
{
    TK_DebugAssert(allocator != NULL);
    TK_DebugAssert(connection != NULL);
    
    TK_DisconnectEventConnection(connection);
    
    return allocator->free(allocator, connection, sizeof (*connection));
}



void TK_DisconnectEventConnection( TK_EventConnection *connection )
{
    TK_DebugAssert(connection != NULL);
    
    if ( connection->connected )
        return;
    
    if ( connection->next == connection )
    {
        connection->connected->first = NULL;
        connection->connected->last = NULL;
    }
    else
    {
        connection->next->prev = connection->prev;
        connection->prev->next = connection->next;
        
        if ( connection == connection->connected->first )
            connection->connected->first = connection->next;
        
        if ( connection == connection->connected->last )
            connection->connected->last = connection->prev;
    }
    
    connection->next = NULL;
    connection->prev = NULL;
    connection->connected = NULL;
}



#ifdef __cplusplus
}
#endif