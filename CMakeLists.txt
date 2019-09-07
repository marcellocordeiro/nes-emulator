cmake_minimum_required(VERSION 3.15)

project(nes-emulator)

# Setting up the target

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

file(GLOB SOURCE_FILES
  "src/mappers/*.cpp"
  "src/types/*.cpp"
  "src/utility/*.cpp"
  "src/*.cpp"
)

add_executable(nes-emulator ${SOURCE_FILES})

target_include_directories(nes-emulator PRIVATE "include")

# Dependencies

add_subdirectory("lib")
target_link_libraries(nes-emulator PRIVATE nes-emulator-lib)

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(nes-emulator PRIVATE SDL2::SDL2)

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator PRIVATE fmt::fmt)

# Compiler settings

set_target_properties(nes-emulator
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(nes-emulator PRIVATE /W4 /permissive- /diagnostics:caret)
elseif(UNIX)
  target_compile_options(nes-emulator PRIVATE -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wdisabled-optimization)
endif()

# Copy resource files

configure_file("res/nes_mesen.pal" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/palette.pal" COPYONLY)
