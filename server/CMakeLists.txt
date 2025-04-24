if (ANDROID)
    message(FATAL_ERROR "This project cannot be built on Android.")
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core WebSockets SerialBus Network)

qt_standard_project_setup()
qt_add_executable(server)

set_target_properties(
    server
    PROPERTIES
        LINK_FLAGS "-Wl,--export-dynamic"
)

file(GLOB_RECURSE CANSOURCES "CAN/*.[hc]pp")
file(GLOB_RECURSE VISASOURCES "VISA/*.[hc]pp")
file(GLOB_RECURSE GPIOSOURCES "gpio/*.[hc]pp")

target_sources(server PRIVATE
    ${CANSOURCES}
    ${VISASOURCES}
    ${GPIOSOURCES}
    main.cpp
    message_parser.cpp
    simulator_base.cpp
    simulator_base.hpp
    simulators.cpp
    simulators.hpp
    websocket.cpp
    websocket.hpp
)

target_compile_options(server PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_include_directories(server
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/simulators
    ${CMAKE_CURRENT_LIST_DIR}/CAN
    ${CMAKE_CURRENT_LIST_DIR}/VISA
    ${CMAKE_CURRENT_LIST_DIR}/gpio
    PRIVATE
)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()
if (LINUX)
    set(GPIOD_LIB "gpiodcxx")
endif()
target_link_libraries(server PRIVATE
    Qt::WebSockets
    Qt::SerialBus
    Qt::Network
    magic_enum::magic_enum
    spdlog
    UI
    cpp_templates
    ${GPIOD_LIB}
)


file(GLOB_RECURSE sims "simulators/*.cpp")

# Loop through each .cpp file and create a library for it
foreach(sim ${sims})
    # Extract the base name of the file (without the directory and extension)
    get_filename_component(LIB_NAME ${sim} NAME_WE)

    # Create a library for each .cpp file
    add_library(sim_${LIB_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.hpp ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.cpp)
    target_link_libraries(sim_${LIB_NAME} PRIVATE Qt::SerialBus magic_enum::magic_enum nlohmann_json::nlohmann_json)
    target_include_directories(sim_${LIB_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/CAN ${CMAKE_SOURCE_DIR}/VISA ${CMAKE_SOURCE_DIR}/GPIO ${CMAKE_SOURCE_DIR}/UI ${CMAKE_SOURCE_DIR}/external/color ${CMAKE_SOURCE_DIR}/external/spdlog/include)
endforeach()
