cmake_minimum_required(VERSION 3.12)

project(
        labwork7
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 labwork7"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(NINJA_API_KEY "6W8QbhMh3TZ8smIPXqdr+g==0KzYuqtx8g1QdhGT")


include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8
)
FetchContent_MakeAvailable(cpr)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

add_subdirectory(lib)
add_subdirectory(bin)
