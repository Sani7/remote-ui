add_library(Timer STATIC)

target_sources(Timer
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/timer.cpp
    
)

target_include_directories(Timer
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)