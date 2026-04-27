#ifndef __TK_MACRO_H__
#define __TK_MACRO_H__

#define TKAPI extern

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
    #define TK_UNIX
    
    #if defined(_POSIX_VERSION)
        #define TK_POSIX _POSIX_VERSION
    #endif
#endif

#if defined(__linux__)
    #define TK_LINUX
#endif

#if defined(_WIN64)
    #define TK_WINDOWS_64BIT _WIN64
    #define TK_WINDOWS TK_WIN64
#elif defined(_WIN32)
    #define TK_WINDOWS_32BIT _WIN32
    #define TK_WINDOWS TK_WIN32
#endif

#endif