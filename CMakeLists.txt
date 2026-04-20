cmake_minimum_required(VERSION 4.0.0)
project(TK VERSION "0.0.1" LANGUAGES C)

set(C_STANDARD 23)

add_library(TK STATIC 
    src/TK/TK_filesystem.c
    src/TK/TK_memory.c
    src/TK/TK_string.c)
set_property(TARGET TK PROPERTY C_STANDARD 23)
target_include_directories(TK PRIVATE ./src)

add_executable(Sandbox src/Sandbox/Main.c)
set_property(TARGET Sandbox PROPERTY C_STANDARD 23)
target_include_directories(Sandbox PRIVATE ./src)
target_link_libraries(Sandbox TK)