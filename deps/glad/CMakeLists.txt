add_library(glad STATIC)
target_sources(glad PRIVATE glad.c)
target_include_directories(glad PRIVATE ../)