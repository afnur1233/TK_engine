#ifndef __TK_ASSERT_C__
#define __TK_ASSERT_C__
#include <stdlib.h>
#include <stdio.h>

#define TK_CoreAssert(condition, ...)                                                           \
    ({                                                                                          \
        int __condition = !!(condition);                                                        \
                                                                                                \
        if ( !__condition )                                                                     \
        {                                                                                       \
            const char *__message[] = { __VA_OPT__( __VA_ARGS__, ) #condition };                \
                                                                                                \
            printf("[DEBUG][%s:%lu] Assertion failed: %s\n", __FILE__, __LINE__, __message[0]); \
            exit(1);                                                                            \
        }                                                                                       \
    })

#define TK_DebugAssert __TK_DebugAssertImpl
#ifdef NDEBUG
    #define __TK_DebugAssertImpl 
#else
    #define __TK_DebugAssertImpl TK_CoreAssert
#endif


#ifdef __cplusplus
    #define TK_StaticAssert static_assert
#else
    #define TK_StaticAssert _Static_assert
#endif

#endif