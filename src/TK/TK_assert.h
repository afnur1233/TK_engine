#ifndef __TK_ASSERT_C__
#define __TK_ASSERT_C__
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __TK_CoreAssertImpl(char const *file, long long line, bool cond, char const *message, ...) {
    if (cond) {
        return;
    }
    
    printf("[DEBUG][%s:%lli] -- Assertion failed: %s\n", file, line, message);
    exit(1);
}

#define TK_CoreAssert(__condition,...)__TK_CoreAssertImpl(__FILE__,__LINE__,!!(__condition),__VA_OPT__(__VA_ARGS__,) #__condition)

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