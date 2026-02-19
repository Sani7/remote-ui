find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg WebSockets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ui "ui/*")
file(GLOB_RECURSE sims "simulators/*")

set(PROJECT_SOURCES
    ${ui}
    ${sims}
    IterableEnum.hpp
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
    factory.hpp
    helpers.cpp
    helpers.hpp
    ui_base.cpp
    ui_base.hpp
    web_socket_wrapper.cpp
    web_socket_wrapper.hpp
)

qt_add_executable(unisim_client
    ${PROJECT_SOURCES}
    )

set_target_properties(
    unisim_client
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
)

target_include_directories(unisim_client
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/ui
    ${CMAKE_CURRENT_LIST_DIR}/simulators
    ${PROJECT_BINARY_DIR}/
)

target_link_libraries(unisim_client PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets spdlog magic_enum::magic_enum nlohmann_json::nlohmann_json git_tag qwt)

include(GNUInstallDirs)
install(TARGETS unisim_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
