add_executable(simulators)
target_sources(simulators PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/simulator_base.cpp
)

target_link_libraries(simulators PRIVATE
    UI
)