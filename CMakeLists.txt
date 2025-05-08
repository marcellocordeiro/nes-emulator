cmake_minimum_required(VERSION 3.30.5...4.0.1)

project(nes-emulator)

include("cmake/CompilerWarnings.cmake")
include("cmake/get_cpm.cmake")
include("cmake/StandardProjectSettings.cmake")

add_library(project-warnings INTERFACE)
set_project_warnings(project-warnings)

add_subdirectory("apps/sdl3")
add_subdirectory("core/common")
add_subdirectory("core/nes-core")
