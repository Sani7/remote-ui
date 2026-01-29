# Third Party
find_package(GTest QUIET)
find_package(Qt6 REQUIRED COMPONENTS Core Test)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# tests
file(GLOB_RECURSE TEST_SOURCES "*.[hc]pp")
file(GLOB_RECURSE CAN "../server/extensions/CAN_R1/to_id.cpp")
if (GTest_FOUND)
    message(STATUS "GTest found - building target tests")
    add_executable(tests ${TEST_SOURCES} ${CAN})
    set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)

    target_compile_options(tests PRIVATE $<$<CONFIG:Debug>:
        -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
        >)

    target_link_libraries(tests
        PRIVATE
        UI
        gtest
        magic_enum::magic_enum
        GTest::gtest_main
        Qt::Core
        Qt::Test
    )

    target_include_directories(tests
        PUBLIC
        ${CMAKE_SOURCE_DIR}/external/cpp-templates
        ${CMAKE_SOURCE_DIR}/server/extensions
        ${CMAKE_CURRENT_LIST_DIR}
        ${PROJECT_BINARY_DIR}
    )

    include(GoogleTest)
    gtest_discover_tests(tests)
else()
    message(STATUS "GTest not found - not building target tests")
endif()
