# Third Party
find_package(GTest)
find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# tests
file(GLOB_RECURSE TEST_SOURCES "*.[hc]pp")
file(GLOB_RECURSE CAN "../server/CAN/to_id.cpp")
if (GTest_FOUND)
add_executable(tests ${TEST_SOURCES} ${CAN})

target_compile_options(tests PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_link_libraries(tests
    PRIVATE
    UI
    gtest
    GTest::gtest_main
    Qt::Core
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
endif()
