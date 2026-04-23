add_library(TK STATIC)
target_sources(TK PRIVATE
    TK/TK_filesystem.c
    TK/TK_memory.c
    TK/TK_scheduler.c
    TK/TK_string.c
    TK/TK_event.c
    TK/ECS/TK_ecs.c)
set_property(TARGET TK PROPERTY C_STANDARD ${C_STANDARD})
target_include_directories(TK PRIVATE
    ./
    ../deps/cglm/include
    ../deps/glfw/include
    ../deps/)
target_link_libraries(TK glfw glad)

add_executable(Sandbox)
set_property(TARGET Sandbox PROPERTY C_STANDARD ${C_STANDARD})
target_sources(Sandbox PRIVATE Sandbox/Main.c)
target_include_directories(Sandbox PRIVATE
    ./
    ../deps/cglm/include
    ../deps/glfw/include
    ../deps/)
target_link_libraries(Sandbox TK)