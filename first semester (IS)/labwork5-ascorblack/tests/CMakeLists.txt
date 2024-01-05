include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        array_3d_tests
        array_3d_test.cpp
)

target_link_libraries(
        array_3d_tests
        Array3D
        GTest::gtest_main
)

target_include_directories(array_3d_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(array_3d_tests)