add_executable(simulators)
target_sources(simulators PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulator_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/websocket.cpp
)

target_compile_options(simulators PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_include_directories(simulators
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
    ${PROJECT_DIR}/external/websocketpp
)

target_link_libraries(simulators PRIVATE
    UI
    Timer
)