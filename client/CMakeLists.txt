cmake_minimum_required(VERSION 3.5)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg WebSockets)

add_subdirectory(qwt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ui "ui/*.[hc]pp")

set(PROJECT_SOURCES
    ${ui}
    helpers.cpp
    helpers.hpp
    IterableEnum.hpp
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
    networkerror.cpp
    networkerror.hpp
    networkerror.ui
    simulator_base.cpp
    simulator_base.hpp
    web_socket_wrapper.cpp
    web_socket_wrapper.hpp
)

file(GLOB_RECURSE sims "simulators/*.cpp")

# Loop through each .cpp file and create a library for it
foreach(sim ${sims})
    # Extract the base name of the file (without the directory and extension)
    get_filename_component(LIB_NAME ${sim} NAME_WE)

    # Create a library for each .cpp file
    add_library(ui_${LIB_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.hpp ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.cpp ${CMAKE_CURRENT_SOURCE_DIR}/simulators/${LIB_NAME}.ui simulator_base.cpp)
    target_link_libraries(ui_${LIB_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets qwt magic_enum::magic_enum nlohmann_json::nlohmann_json)
    target_include_directories(ui_${LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ui ${PROJECT_BINARY_DIR}/)

    set_target_properties(
        ui_${LIB_NAME}
        PROPERTIES
            AUTOUIC ON
            AUTOMOC ON
            AUTORCC ON
    )
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Simulator-CVS_I10
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        )
# Define target properties for Android with Qt 6 as:
# set_property(TARGET Simulator - CVS_I10 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR } / android)
# For more information, see https: // doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Simulator-CVS_I10 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
# set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Simulator-CVS_I10
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(Simulator-CVS_I10
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/simulators
    ${CMAKE_CURRENT_LIST_DIR}/ui
    ${PROJECT_BINARY_DIR}/
)

target_link_libraries(client PRIVATE qwt spdlog Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets magic_enum::magic_enum nlohmann_json::nlohmann_json)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Simulator-CVS_I10)
endif()
set_target_properties(Simulator-CVS_I10 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Simulator-CVS_I10
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Simulator-CVS_I10)
endif()
