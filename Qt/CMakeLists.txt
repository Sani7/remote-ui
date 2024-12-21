cmake_minimum_required(VERSION 3.5)

project(Simulator-CVS_I10 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include(FetchContent)

FetchContent_Declare(magic_enum URL https://github.com/Neargye/magic_enum/releases/download/v0.9.6/magic_enum-v0.9.6.tar.gz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(magic_enum)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(json)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg WebSockets)

add_subdirectory(qwt)

set(PROJECT_SOURCES
        simulators/sim-cvs_i10.h
        simulators/sim-cvs_i10.cpp
        simulators/sim-cvs_i10.ui

        simulators/sim-can_debugger.h
        simulators/sim-can_debugger.cpp
        simulators/sim-can_debugger.ui

        simulators/sim-scope_mux_tester.h
        simulators/sim-scope_mux_tester.cpp
        simulators/sim-scope_mux_tester.ui

        simulators/sim-test.h
        simulators/sim-test.cpp
        simulators/sim-test.ui

        simulatorbase.h
        simulatorbase.cpp
        helpers.h
        helpers.cpp
        IterableEnum.hpp
        better_radiobutton.h
        better_radiobutton.cpp
        led.h
        led.cpp

        networkerror.h
        networkerror.cpp
        networkerror.ui

        web_socket_wrapper.h
        web_socket_wrapper.cpp

        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        main.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Simulator-CVS_I10
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Simulator-CVS_I10 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Simulator-CVS_I10 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
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
    ${PROJECT_BINARY_DIR}/
)

target_link_libraries(Simulator-CVS_I10 PRIVATE qwt Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebSockets magic_enum::magic_enum nlohmann_json::nlohmann_json)

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
