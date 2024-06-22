project(STL_BST)

set(SOURCES
        BinarySearchTreeSTL.cpp
)


add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
