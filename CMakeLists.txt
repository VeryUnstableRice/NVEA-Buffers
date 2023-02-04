# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project (BUFFERS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
include_directories(${CMAKE_SOURCE_DIR}/src)


add_executable(BUFFERS "src/main.c" "src/Abstractiser/lexer.c" "src/Abstractiser/struct.c")
