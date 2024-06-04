add_library(ArgParser modules/arg_parser/ArgParser.cpp)

project(SearchEngineSource)

set(SOURCES
        SearchEngine.cpp
        Indexer.cpp
        modules/arg_parser/ArgParser.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
