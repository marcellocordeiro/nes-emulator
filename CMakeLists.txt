cmake_minimum_required(VERSION 4.3.0)

project(
  nes-emulator
  LANGUAGES CXX C
  VERSION 0.1.0
)

include(cmake/CompilerWarnings.cmake)
include(cmake/Compilers.cmake)
include(cmake/Dependencies.cmake)
include(cmake/Options.cmake)
include(cmake/TargetOptions.cmake)
include(cmake/Testing.cmake)

set(GIT_SHA
  "Unknown"
  CACHE STRING "SHA this build was generated from"
)

add_subdirectory(apps/sdl3)
add_subdirectory(core/common)
add_subdirectory(core/common-sys)
add_subdirectory(core/nes-core)
