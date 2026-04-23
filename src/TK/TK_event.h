#ifndef __TK_EVENT_H__
#define __TK_EVENT_H__
#include <TK/TK_stdint.h>
#include <TK/TK_memory.h>

typedef struct TK_Event               TK_Event;
typedef struct TK_EventHandlers       TK_EventHandlers;
typedef struct TK_EventConnection     TK_EventConnection;
typedef void TK_EventCallback(TK_Event *event, void *);

#define TK_EVENT_IMPL()                \
    union                              \
    {                                  \
        struct                         \
        {                              \
            TK_EventConnection *first; \
            TK_EventConnection *last;  \
        };                             \
        TK_Event event;                \
    }

struct TK_Event
{
    TK_EventConnection *first;
    TK_EventConnection *last;
};

struct TK_EventHandlers
{
    TK_EventCallback *callback;
    void             *callbackArgument;
    TK_bool          *lockVariable;
};

struct TK_EventConnection
{
    TK_EventConnection *next;
    TK_EventConnection *prev;
    TK_Event           *connected;
    TK_EventHandlers    handlers;
    TK_bool             once;
    TK_bool             keep;
};

/** @brief Options for @ref TK_ConnectEvent_o.
 *  
 *  @since Added in version 1.0.
 *  @ingroup event
 */
typedef 
    struct TK_EventConnectOptions TK_EventConnectOptions;
    struct TK_EventConnectOptions
{
    /** @brief A connection to reuse.
     *  
     *  A connection to reuse, set it to NULL if you want to allocate a new one.
     *  If the connection is already connected to a event, it will be automatically disconnected.
     *  
     *  @since Added in version 1.0.
     */
    TK_EventConnection *connection;
    
    /** @brief A callback function to be called once the event is fired.
     *  
     *  @since Added in version 1.0.
     */
    TK_EventCallback *callback;
    
    /** @brief
     *  
     *  @since Added in version 1.0.
     */
    void *callbackArgument;
    
    /** @brief
     *  
     *  @since Added in version 1.0.
     */
    TK_bool *lockVariable;
    
    /** @brief If the connection should destroy itself after firing.
     *  
     *  @since Added in version 1.0.
     */
    TK_bool once;
    
    /** @brief If the connection shouldn't destroy itself when 'once' is true after firing.
     *  
     *  @since Added in version 1.0.
     */
    TK_bool keep;
};

/** @brief Convinent wrapper around @ref TK_ConnectEvent_o.
 *  
 *  @since Added in version 1.0.
 *  @ingroup event
 */
#define TK_ConnectEvent(allocator, event, ...)TK_ConnectEvent_o((allocator), (event), (TK_EventConnectOptions){__VA_ARGS__})

/** @brief Create or reuse a connection to connect to the event.
 *  
 *  @return A new connection thats connected to the given event.
 *  @param[in] allocator The allocator to allocate a new connection if one isnt provided.
 *  @param[in] event     The event to connect to.
 *  @param[in] options   Options for connecting @see @ref TK_EventConnectOptions.
 *  
 *  @since Added in version 1.0.
 *  @ingroup event
 */ 
TKAPI TK_EventConnection *TK_ConnectEvent_o( TK_Allocator *allocator, TK_Event *event, TK_EventConnectOptions options );

/** @brief Fire all connections connected to the event in reverse order.
 *  
 *  @param[in] allocator The allocator to free one time connections that dont have 'keep' as true.
 *  @param[in] event     The event to fire.
 *  
 *  @since Added in version 1.0.
 *  @ingroup event
 */ 
TKAPI void TK_FireEvent( TK_Allocator *allocator, TK_Event *event );

/** @brief Disconnect the connection from it's event and free the memory.
 *  
 *  @param[in] allocator  The allocator to free the conenctions memor<y.
 *  @param[in] connection The connection to destroy.
 *  
 *  @since Added in version 1.0.
 *  @ingroup event
 */ 
TKAPI TK_EventConnection *TK_DestroyEventConnection( TK_Allocator *allocator, TK_EventConnection *connection );

/** @brief Disconnect the connection from it's event, the connection can be reconnected with @ref TK_ConnectEvent_o.
 *  
 *  @param[in] connection The connection to disconnect.
 *  
 *  @since Added in version 1.0.
 *  @ingroup event
 */ 
TKAPI void TK_DisconnectEventConnection( TK_EventConnection *connection );

#endif