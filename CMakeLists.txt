cmake_minimum_required(VERSION 4.0.0)
project(TK VERSION "0.0.1" LANGUAGES C)

set(C_STANDARD 23)
add_subdirectory(./deps)
add_subdirectory(./src)