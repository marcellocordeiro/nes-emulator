cmake_minimum_required(VERSION 3.15)

project(nes-emulator-lib)

# Setting up the target

add_library(nes-emulator-lib STATIC)

target_sources(nes-emulator-lib PRIVATE
  "src/bit_utils.cpp"
  "src/log.cpp"
  "src/system_utils.cpp"
  "src/timer.cpp"
)

target_include_directories(nes-emulator-lib SYSTEM PUBLIC "include")

# Dependencies

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator-lib PRIVATE fmt::fmt-header-only)

add_subdirectory("external/Nes_Snd_Emu")
target_link_libraries(nes-emulator-lib INTERFACE Nes_Snd_Emu)

# Compiler settings

target_compile_features(nes-emulator-lib PRIVATE cxx_std_17)
