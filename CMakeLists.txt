cmake_minimum_required(VERSION 3.30.5...4.0.1)

project(nes-emulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

function(copy_palette target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/data/nes_mesen.pal"
            "$<TARGET_FILE_DIR:${target}>/palette.pal"
    )
endfunction()

# include(FetchContent)

# include("cmake/AddFmt.cmake")
# include("cmake/AddSdl3.cmake")
# include("cmake/AddSpdlog.cmake")
include("cmake/CompilerWarnings.cmake")
include("cmake/StandardProjectSettings.cmake")

# add_fmt()
# add_sdl3()
# add_spdlog()

add_library(project-warnings INTERFACE)
set_project_warnings(project-warnings)

add_subdirectory("core/common")
add_subdirectory("core/nes-core")
add_subdirectory("platform/sdl3")
