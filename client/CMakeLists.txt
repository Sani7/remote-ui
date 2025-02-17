cmake_minimum_required(VERSION 3.5)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg WebSockets)

add_subdirectory(qwt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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

        simulator_base.h
        simulator_base.cpp
        helpers.h
        helpers.cpp
        IterableEnum.hpp
        hex_spinbox.h
        hex_spinbox.cpp
        better_radiobutton.h
        better_radiobutton.cpp
        led.h
        led.cpp
        
        can_transceive.h
        can_transceive.cpp
        can_transceive.ui

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
