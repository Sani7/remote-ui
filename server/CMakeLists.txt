if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core WebSockets SerialBus Network)

qt_standard_project_setup()
qt_add_executable(unisim_server)

set_target_properties(
    unisim_server
    PROPERTIES
        LINK_FLAGS "-Wl,--export-dynamic"
)

file(GLOB_RECURSE EXTSOURCES "extensions/*.[hc]pp")

target_sources(unisim_server PRIVATE
    ${EXTSOURCES}
    main.cpp
    message_parser.cpp
    simulator_base.cpp
    simulator_base.hpp
    simulators.cpp
    simulators.hpp
    websocket.cpp
    websocket.hpp
)

target_compile_options(unisim_server PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_include_directories(unisim_server
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/simulators
    ${CMAKE_CURRENT_LIST_DIR}/extensions
    PRIVATE
)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()
if (LINUX)
    set(GPIOD_LIB "gpiodcxx")
endif()
target_link_libraries(unisim_server PRIVATE
    Qt::WebSockets
    Qt::SerialBus
    Qt::Network
    magic_enum::magic_enum
    spdlog
    UI
    cpp_templates
    ${GPIOD_LIB}
    git_tag
)

file(GLOB_RECURSE sims "simulators/*.cpp")

# Loop through each .cpp file and create a library for it
foreach(sim ${sims})
    # Extract the base name of the file (without the directory and extension)
    get_filename_component(LIB_NAME ${sim} NAME_WE)

    # Create a library for each .cpp file
    add_library(sim_${LIB_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.hpp ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.cpp)
    target_link_libraries(sim_${LIB_NAME} PRIVATE Qt::SerialBus magic_enum::magic_enum nlohmann_json::nlohmann_json)
    target_include_directories(sim_${LIB_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/extensions ${CMAKE_SOURCE_DIR}/UI ${CMAKE_SOURCE_DIR}/external/color ${CMAKE_SOURCE_DIR}/external/cpp-templates)
    add_custom_command(TARGET sim_${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sim_${LIB_NAME}>
        $<TARGET_FILE_DIR:unisim_server>)
endforeach()
