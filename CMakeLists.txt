cmake_minimum_required(VERSION 3.31.6...4.0.2)

project(
  nes-emulator
  LANGUAGES CXX C
  VERSION 0.1.0
)

include(cmake/Compilers.cmake)
include(cmake/Dependencies.cmake)
include(cmake/Options.cmake)
include(cmake/ProjectOptions.cmake)
include(cmake/CompilerWarnings.cmake)

set(GIT_SHA
  "Unknown"
  CACHE STRING "SHA this build was generated from"
)

add_subdirectory(apps/sdl3)
add_subdirectory(core/common)
add_subdirectory(core/common-sys)
add_subdirectory(core/nes-core)
