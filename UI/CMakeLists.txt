add_library(UI STATIC)

find_package(Qt6 REQUIRED COMPONENTS Core SerialBus)

set_target_properties(
    UI
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
)

target_compile_options(UI PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

file(GLOB_RECURSE CUSTOM_UI_SOURCES "custom/*.?pp")
file(GLOB_RECURSE UI_SOURCES "*.?pp")

target_sources(UI
  PRIVATE
    ${UI_SOURCES}
    ${CUSTOM_UI_SOURCES}
)

target_include_directories(UI
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/custom/**
    PRIVATE
    ${PROJECT_DIR}/external/**
    ${PROJECT_DIR}/src/CAN/
    ${CMAKE_BINARY_DIR}/**
)

target_link_libraries(UI
  PUBLIC
    Color
    nlohmann_json::nlohmann_json
    Qt::Core
    Qt::SerialBus
)
