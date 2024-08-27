add_library(CAN)

target_sources(CAN PRIVATE
    CAN_interface.cpp
)

target_compile_options(CAN PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_include_directories(CAN
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
)

