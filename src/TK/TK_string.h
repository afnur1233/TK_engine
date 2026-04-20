#ifndef __TK_STRING_H__
#define __TK_STRING_H__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <TK/TK_macro.h>
#include <TK/TK_memory.h>
#include <TK/TK_stdint.h>
#include <TK/TK_typetraits.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct TK_String      TK_String;
typedef struct TK_StringSlice TK_StringSlice;

struct TK_String
{
    TK_byte *buf;
    TK_usize len;
    TK_usize cap;
};

struct TK_StringSlice
{
    TK_byte *buf;
    TK_usize len;
};

#define TK_StringSlice_From_CString(...)                                 \
    ({                                                                   \
        char *__string = TK_REINTERPRET_CAST( __string, (__VA_ARGS__) ); \
        TK_usize len = 0;                                                \
                                                                         \
        TK_ComptimeIf ( TK_Type_IsArray( __VA_ARGS__ ) )                 \
            len = sizeof( __VA_ARGS__ ) - 1;                             \
                                                                         \
        else if ( __string )                                             \
            while ( __string[len] != '\0' )                              \
                len++;                                                   \
                                                                         \
        (TK_StringSlice) {                                               \
            .buf = __string,                                             \
            .len = len,                                                  \
        };                                                               \
    })

#define TK_StringSlice(...)                                               \
    ({                                                                    \
        __typeof__(__VA_ARGS__) __argument = (__VA_ARGS__);               \
                                                                          \
        _Generic(TK_Type_Declval(TK_Type_Unqual(__argument)),             \
            TK_String: TK_REINTERPRET_CAST( TK_StringSlice, __argument ), \
            TK_StringSlice: __argument,                                   \
                  char *: TK_StringSlice_From_CString(__argument),        \
            const char *: TK_StringSlice_From_CString(__argument)         \
        );                                                                \
    })

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TKAPI bool      TK_StringGrow   ( TK_Allocator *allocator, TK_String *string, TK_usize new_cap );
TKAPI bool      TK_StringAppend ( TK_Allocator *allocator, TK_String *string, TK_byte *buf, TK_usize len );
TKAPI TK_String TK_StringFree   ( TK_Allocator *allocator, TK_String string );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif