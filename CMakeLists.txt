cmake_minimum_required(VERSION 3.15)

project(nes-emulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
configure_file("data/nes_mesen.pal" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/palette.pal" COPYONLY)

include("cmake/Cache.cmake")
include("cmake/CompilerWarnings.cmake")
include("cmake/StandardProjectSettings.cmake")
include("cmake/StaticAnalysers.cmake")

add_library(project-warnings INTERFACE)
set_project_warnings(project-warnings)

add_subdirectory("Core")
add_subdirectory("Frontends")
add_subdirectory("lib")
