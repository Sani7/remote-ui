set(COLOR_BUILD_PIC ON)
set(SPDLOG_BUILD_PIC ON)
add_subdirectory(color)
add_subdirectory(cpp-templates)
add_subdirectory(git_tag_cpp)
add_subdirectory(json)
add_subdirectory(magic_enum)
add_subdirectory(spdlog)
add_subdirectory(qwt)

if (EMSCRIPTEN)
target_compile_options(spdlog
    PRIVATE
        -pthread
)

target_link_options(spdlog
    PRIVATE
        -pthread
)
endif()