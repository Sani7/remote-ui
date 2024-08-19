add_executable(server)
target_sources(server PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
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
)