cmake_minimum_required(VERSION 3.25)

project(
        labwork5_ascorblack
        VERSION 1.0
        DESCRIPTION "uint17_t 3D Array"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
#add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
