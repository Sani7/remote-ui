add_library(UI)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(json)

target_compile_options(UI PRIVATE $<$<CONFIG:Debug>:
    -Wall -Wextra -pedantic-errors -Wconversion -Wsign-conversion
    >)

target_sources(UI
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/UI_item.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_label.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_led.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_button.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_slider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_x_boxes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_combobox.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI_checkbox.cpp
)

target_include_directories(UI
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE
    ${PROJECT_DIR}/external/**
    ${CMAKE_BINARY_DIR}/**
)

target_link_libraries(UI
  PUBLIC
    Color
    nlohmann_json::nlohmann_json
)