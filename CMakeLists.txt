cmake_minimum_required(VERSION 3.15)

project(nes-emulator)

# Setting up the target

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(nes-emulator)

target_sources(nes-emulator PRIVATE
  "src/mappers/mapper0.cpp"
  "src/mappers/mapper1.cpp"
  "src/mappers/mapper2.cpp"
  "src/mappers/mapper4.cpp"
  "src/types/cpu.cpp"
  "src/types/ppu.cpp"
  "src/utility/file_manager.cpp"
  "src/utility/ips_patch.cpp"
  "src/apu.cpp"
  "src/cartridge.cpp"
  "src/controller.cpp"
  "src/cpu.cpp"
  "src/cpu_instructions.cpp"
  "src/debugger.cpp"
  "src/emulator.cpp"
  "src/io.cpp"
  "src/main.cpp"
  "src/mapper.cpp"
  "src/ppu.cpp"
)

target_include_directories(nes-emulator PRIVATE "include")

# Dependencies

add_subdirectory("lib")
target_link_libraries(nes-emulator PRIVATE nes-emulator-lib)

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(nes-emulator PRIVATE SDL2::SDL2)
target_compile_definitions(nes-emulator PRIVATE $<$<CXX_COMPILER_ID:MSVC>:SDL_MAIN_HANDLED>)

# Compiler settings

set_target_properties(nes-emulator
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(nes-emulator
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /permissive- /diagnostics:caret /MP>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror -Wall -Wextra -pedantic -Wuseless-cast -Wshadow -Wformat=2 -Wdisabled-optimization>
)

# Copy resource files

configure_file("res/nes_mesen.pal" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/palette.pal" COPYONLY)
