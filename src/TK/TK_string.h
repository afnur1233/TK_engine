#ifndef __TK_STRING_H__
#define __TK_STRING_H__
#include <TK/TK_macro.h>
#include <TK/TK_memory.h>
#include <TK/TK_stdint.h>
#include <TK/TK_typetraits.h>

typedef struct TK_String
{
    TK_byte *buf;
    TK_usize len;
    TK_usize cap;
} TK_String;

typedef struct TK_StringSlice
{
    TK_byte *buf;
    TK_usize len;
} TK_StringSlice;

#define __TK_CreateStringSliceFromCString(...)                 \
    ({                                                         \
        TK_Type_Decay(__VA_ARGS__) __argument = (__VA_ARGS__); \
        char *__string = *(char **)((void *)&__argument);      \
                                                               \
        (TK_StringSlice) {                                     \
            .buf = __string,                                   \
            .len = TK_Type_IsArray( __VA_ARGS__ )              \
                ? sizeof( __VA_ARGS__ ) - 1                    \
                : __argument ? strlen(__argument) : 0,         \
        };                                                     \
    })

#define TK_CreateStringSlice(...)                                       \
    _Generic(TK_Type_Declval( TK_Type_Unqual( __VA_ARGS__ ) ),          \
        TK_StringSlice: ( __VA_ARGS__ ),                                \
        TK_String: ({                                                   \
            TK_Type_Decay(__VA_ARGS__) __value = ( __VA_ARGS__ );       \
            *((TK_StringSlice*)((void *)&__value));                     \
        }),                                                             \
              char *: __TK_CreateStringSliceFromCString( __VA_ARGS__ ), \
        const char *: __TK_CreateStringSliceFromCString( __VA_ARGS__ )  \
    )

TKAPI TK_bool   TK_StringGrow   ( TK_Allocator *allocator, TK_String *string, TK_usize new_cap );
TKAPI TK_bool   TK_StringAppend ( TK_Allocator *allocator, TK_String *string, TK_byte *buf, TK_usize len );
TKAPI TK_String TK_StringFree   ( TK_Allocator *allocator, TK_String string );

#endif