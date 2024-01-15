cmake_minimum_required(VERSION 3.28)

project(nes-emulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
configure_file("data/nes_mesen.pal" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/palette.pal" COPYONLY)

include("cmake/Cache.cmake")
include("cmake/CompilerWarnings.cmake")
include("cmake/StandardProjectSettings.cmake")

add_library(project-warnings INTERFACE)
set_project_warnings(project-warnings)

add_subdirectory("external")
add_subdirectory("libs")
add_subdirectory("src")
