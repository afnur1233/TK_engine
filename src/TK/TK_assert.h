#ifndef __TK_ASSERT_C__
#define __TK_ASSERT_C__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <TK/TK_stdint.h>
#include <stdlib.h>
#include <stdio.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ___TK_CoreAssertStr(...)#__VA_ARGS__
#define __TK_CoreAssertStr(...)___TK_CoreAssertStr(__VA_ARGS__)
#define TK_CoreAssert(condition, ...)                                                                             \
    ({                                                                                                            \
        TK_bool __condition = !!(condition);                                                                      \
        if (!__condition) {                                                                                       \
            const char *__message[] = { __VA_OPT__( __VA_ARGS__, ) #condition };                                  \
                                                                                                                  \
            printf("[DEBUG]["__FILE__":"__TK_CoreAssertStr(__LINE__)"] -- Assertion failed: %s\n", __message[0]); \
            exit(1);                                                                                              \
        }                                                                                                         \
    })

#ifdef NDEBUG
    #define TK_DebugAssert(...)
#else
    #define TK_DebugAssert(...)TK_CoreAssert(__VA_ARGS__)
#endif

#ifndef __cplusplus
    #define TK_StaticAssert(...)_Static_assert(__VA_ARGS__)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif