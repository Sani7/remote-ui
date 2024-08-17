# Third Party
include_directories(${GTEST_INCLUDE_DIR})

link_directories(${GTEST_LIB_DIR})

find_package(GTest REQUIRED)

# tests
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests ${LIBS})

target_compile_options(tests PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)
target_link_libraries(tests
    PRIVATE
    UI
    gtest
    GTest::gtest_main
)

target_include_directories(tests
    PUBLIC
    ${CMAKE_MAIN_SOURCE_DIR}/external/**
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_MAIN_SOURCE_DIR}/src/**
    ${PROJECT_BINARY_DIR}
)

include(GoogleTest)
gtest_discover_tests(tests)