cmake_minimum_required(VERSION 3.15)

project(nes-emulator-lib)

# Setting up the target

add_library(nes-emulator-lib STATIC)

target_sources(nes-emulator-lib PRIVATE
  "src/log.cpp"
  "src/bit_utils.cpp"
)

target_include_directories(nes-emulator-lib SYSTEM PUBLIC "include")

# Dependencies

add_subdirectory("external/Nes_Snd_Emu")
target_link_libraries(nes-emulator-lib INTERFACE Nes_Snd_Emu)

# Compiler settings

target_compile_features(nes-emulator-lib PRIVATE cxx_std_17)
