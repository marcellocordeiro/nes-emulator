cmake_minimum_required(VERSION 3.15)

project(nes)

# Setting up the target

file(GLOB SOURCE_FILES
  "src/mappers/*.cpp"
  "src/types/*.cpp"
  "src/utility/*.cpp"
  "src/*.cpp"
)

add_library(nes STATIC ${SOURCE_FILES})

target_include_directories(nes PUBLIC "include")

# Dependencies

add_subdirectory("libs")
target_link_libraries(nes PUBLIC nes-libs)

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(nes PRIVATE SDL2::SDL2)

find_package(fmt REQUIRED)
target_link_libraries(nes PRIVATE fmt::fmt-header-only)

# Compiler settings

set_target_properties(nes
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(MSVC)
  target_compile_options(nes PRIVATE /W4 /permissive- /diagnostics:caret)
elseif(UNIX)
  target_compile_options(nes PRIVATE -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wdisabled-optimization)
endif()
