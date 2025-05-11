cmake_minimum_required(VERSION 3.30.5...4.0.2)

project(nes-emulator)

include("cmake/Compilers.cmake")
include("cmake/CompilerWarnings.cmake")
include("cmake/Dependencies.cmake")
include("cmake/InterproceduralOptimization.cmake")
include("cmake/StandardProjectSettings.cmake")

add_subdirectory("apps/sdl3")
add_subdirectory("core/common")
add_subdirectory("core/nes-core")
