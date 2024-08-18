add_library(Color STATIC)

target_compile_options(Color PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_sources(Color
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/color.cpp
    
)

target_include_directories(Color
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)