cmake_minimum_required(VERSION 3.30.5)

project(nes-emulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
configure_file("data/nes_mesen.pal" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/palette.pal" COPYONLY)

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
