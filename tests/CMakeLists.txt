include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        bst_tests
        bst_tests.cpp
)

target_link_libraries(
        bst_tests
        bst
        GTest::gtest_main
)

target_include_directories(bst_tests PUBLIC "${PROJECT_SOURCE_DIR}/lib/include")

include(GoogleTest)

gtest_discover_tests(bst_tests)