add_executable(server)

include(FetchContent)

FetchContent_Declare(magic_enum URL https://github.com/Neargye/magic_enum/releases/download/v0.9.5/magic_enum-v0.9.5.tar.gz)
FetchContent_MakeAvailable(magic_enum)

target_sources(server PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/message_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulator_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/websocket.cpp
)

target_compile_options(server PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_include_directories(server
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
    ${PROJECT_DIR}/external/websocketpp
)

target_link_libraries(server PRIVATE
    UI
    Timer
    magic_enum::magic_enum
)