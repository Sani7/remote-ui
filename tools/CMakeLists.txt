if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS WebSockets)

qt_add_executable(server)

include(FetchContent)

FetchContent_Declare(magic_enum URL https://github.com/Neargye/magic_enum/releases/download/v0.9.6/magic_enum-v0.9.6.tar.gz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(magic_enum)

file(GLOB_RECURSE SIM_SOURCES "simulators/*.[hc]pp")

target_sources(server PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/message_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulator_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulator_base.hpp
    ${CMAKE_CURRENT_LIST_DIR}/simulators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulators.hpp
    ${CMAKE_CURRENT_LIST_DIR}/websocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/websocket.hpp
    ${SIM_SOURCES}
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

target_link_libraries(server PRIVATE
    Qt::WebSockets
    magic_enum::magic_enum
    spdlog
    UI
)
