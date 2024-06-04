cmake_minimum_required(VERSION 3.25)

project(
        SearchEngineProject
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 labwork10"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

add_subdirectory(lib)
add_subdirectory(bin)

#enable_testing()
#add_subdirectory(tests)
