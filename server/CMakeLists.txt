if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core WebSockets SerialBus)

qt_standard_project_setup()
qt_add_executable(server)

file(GLOB_RECURSE SOURCES "*.[hc]pp")

target_sources(server PRIVATE
    ${SOURCES}
)

target_compile_options(server PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_include_directories(server
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/simulators
    PRIVATE
)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    set(GPIOD_LIB "gpiod")
endif()
target_link_libraries(server PRIVATE
    Qt::WebSockets
    Qt::SerialBus
    magic_enum::magic_enum
    spdlog
    UI
    cpp_templates
    ${GPIOD_LIB}
)
