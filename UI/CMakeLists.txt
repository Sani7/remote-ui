set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(NOT EMSCRIPTEN)
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

file(GLOB_RECURSE UI_SOURCES "*.?pp")

target_sources(UI
  PRIVATE
    ${UI_SOURCES}
    ${CUSTOM_UI_SOURCES}
)

target_include_directories(UI
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
    ${CMAKE_BINARY_DIR}/**
)

target_link_libraries(UI
  PUBLIC
    Color
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
    Qt::Core
    Qt::SerialBus
    UI_enum
)
endif()

add_library(UI_enum INTERFACE)

target_include_directories(UI_enum
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/enum
)