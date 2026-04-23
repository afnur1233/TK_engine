#ifndef __TK_TYPETRAITS_H__
#define __TK_TYPETRAITS_H__

#define TK_Type_Declval(...)       (__typeof__(__VA_ARGS__)){0}
#define TK_Type_Identity(...)      __typeof__(__VA_ARGS__)
#define TK_Type_RemovePointer(...) __typeof__(*TK_Type_Declval(__VA_ARGS__))
#define TK_Type_Decay(...)         __typeof__((0,TK_Type_Declval(__VA_ARGS__)))
#define TK_Type_Unqual(...)        __typeof_unqual__(__VA_ARGS__)

#define TK_ComptimeIf(...)                                      \
    if (                                                        \
        _Generic(TK_Type_Declval(char(*)[1 + !!(__VA_ARGS__)]), \
            char (*)[2]: 1,                                     \
            char (*)[1]: 0                                      \
        )                                                       \
    )

#define TK_Type_IsSameType(T, U)                    \
    _Generic(TK_Type_Declval(TK_Type_Identity(T)*), \
        TK_Type_Identity(U) *: true,                \
        default: false                              \
    )

#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
    #define TK_Type_IsCompatible(T, U) __builtin_types_compatible_p(T, U)
#else
    #define TK_Type_IsCompatible(T, U) TK_Type_IsSameType(TK_Type_Unqual(T), TK_Type_Unqual(U))
#endif

#define TK_Type_IsArray(...) ( !TK_Type_IsCompatible( TK_Type_Decay( __VA_ARGS__ ) *, TK_Type_Identity( __VA_ARGS__ ) * ) )

#endif