#include <TK/TK_string.h>
#include <TK/TK_assert.h>



TK_bool TK_StringGrow( TK_Allocator *allocator, TK_String *string, TK_usize new_cap )
{
    TK_DebugAssert( allocator != NULL );
    TK_DebugAssert( string != NULL );
    
    if (new_cap <= string->cap)
        return true;
    
    if ( string->buf )
        string->buf = allocator->realloc(
            allocator,
            string->buf,
            ( string->cap + 1 ) * sizeof ( *string->buf ),
            ( new_cap + 1 ) * sizeof ( *string->buf )
        );
    
    else
        string->buf = allocator->alloc(
            allocator,
            ( new_cap + 1 ) * sizeof ( *string->buf )
        );
    
    if ( !string->buf )
    {
        string->cap = 0;
        string->len = 0;
        
        return false;
    }
    
    string->cap = new_cap;
    string->buf[new_cap] = '\0';
    
    return true;
}



TK_bool TK_StringAppend( TK_Allocator *allocator, TK_String *string, TK_byte *buf, TK_usize len )
{
    if ( !allocator || !string )
        return false;
    
    if ( !buf || len == 0 )
        return true;
    
    if ( !TK_StringGrow( allocator, string, string->len + len ) )
        return false;
    
    for ( TK_usize i = 0; i < len; ++i )
    {
        if ( buf[i] == '\0' )
            break;
        
        string->buf[string->len++] = buf[i];
    }
    string->buf[string->len] = '\0';
    
    return true;
}



TK_String TK_StringFree( TK_Allocator *allocator, TK_String string )
{
    if ( !allocator || !string.buf )
        return (TK_String){0};
    
    allocator->free(
        allocator,
        string.buf,
        ( string.cap + 1) * sizeof ( *string.buf )
    );
    
    return (TK_String){0};
}
