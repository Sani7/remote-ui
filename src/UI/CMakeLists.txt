add_library(UI)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(json)

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

file(GLOB_RECURSE CUSTOM_UI_SOURCES "custom/*.cpp")

target_sources(UI
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/UI_item.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_item.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_button.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_button.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_checkbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_checkbox.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_combobox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_combobox.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_dial.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_dial.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_label.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_label.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_led.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_led.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_slider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_slider.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_textbox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_textbox.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_thermo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_thermo.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_x_boxes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_x_boxes.hpp

    ${CUSTOM_UI_SOURCES}
)

target_include_directories(UI
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
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
