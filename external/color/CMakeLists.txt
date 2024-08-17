add_library(Color STATIC)

target_sources(Color
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/color.cpp
    
)

target_include_directories(Color
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)