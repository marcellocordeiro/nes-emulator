cmake_minimum_required(VERSION 3.14)

project(nes-emulator VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

#
# Dependencies
#

find_package(SDL2 2.0 REQUIRED)
# find_package(fmt 5.3 REQUIRED)

add_subdirectory("lib")

#
# Setting up the target
#

file(GLOB SOURCE_FILES
  "src/*.cpp"
  "src/mappers/*.cpp"
  "src/types/*.cpp"
)
  
add_executable(nes-emulator ${SOURCE_FILES})
target_include_directories(nes-emulator PRIVATE "include")

target_link_libraries(nes-emulator PRIVATE nes-emulator-lib)
target_link_libraries(nes-emulator PRIVATE SDL2::SDL2)

#
# Compiler settings
#

set_target_properties(nes-emulator
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(nes-emulator
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /permissive- /diagnostics:caret>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror -Wall -Wextra -pedantic -Wuseless-cast -Wshadow -Wformat=2 -Wdisabled-optimization>
)

target_compile_definitions(nes-emulator PRIVATE $<$<CXX_COMPILER_ID:MSVC>:SDL_MAIN_HANDLED>)

# Remove for CMake 3.15 (CMP0092)
string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

#
# Post compilation
#

configure_file("res/nes_mesen.pal" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
