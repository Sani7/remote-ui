find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg WebSockets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ui "ui/*")

set(PROJECT_SOURCES
    ${ui}
    helpers.cpp
    helpers.hpp
    IterableEnum.hpp
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
    ui_base.cpp
    ui_base.hpp
    web_socket_wrapper.cpp
    web_socket_wrapper.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(unisim_client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        $<$<BOOL:${EMSCRIPTEN}>:${sims}>
        )
# Define target properties for Android with Qt 6 as:
# set_property(TARGET Simulator - CVS_I10 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR } / android)
# For more information, see https: // doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(unisim_client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
# set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(unisim_client
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set_target_properties(
    unisim_client
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
        LINK_FLAGS "-Wl,--export-dynamic"
)

target_include_directories(unisim_client
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/simulators
    ${CMAKE_CURRENT_LIST_DIR}/ui
    ${PROJECT_BINARY_DIR}/
)

target_link_libraries(unisim_client PRIVATE qwt spdlog Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets magic_enum::magic_enum nlohmann_json::nlohmann_json git_tag)

include(GNUInstallDirs)
install(TARGETS unisim_client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(unisim_client)
endif()

if (NOT EMSCRIPTEN)
    file(GLOB_RECURSE sims "simulators/*.cpp")
    # Loop through each .cpp file and create a library for it
    foreach(sim ${sims})
        # Extract the base name of the file (without the directory and extension)
        get_filename_component(LIB_NAME ${sim} NAME_WE)

        # Create a library for each .cpp file
        add_library(ui_${LIB_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.hpp ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.cpp ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.ui)
        target_link_libraries(ui_${LIB_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets magic_enum::magic_enum nlohmann_json::nlohmann_json)
        target_include_directories(ui_${LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ui ${CMAKE_SOURCE_DIR}/external/qwt/src ${CMAKE_SOURCE_DIR}/external/spdlog/include ${PROJECT_BINARY_DIR}/)

        set_target_properties(
            ui_${LIB_NAME}
            PROPERTIES
                AUTOUIC ON
                AUTOMOC ON
                AUTORCC ON
        )

        add_custom_command(TARGET ui_${LIB_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ui_${LIB_NAME}>
            $<TARGET_FILE_DIR:unisim_client>)
    endforeach()
else()
    file(GLOB_RECURSE sims "simulators/*")
endif()
