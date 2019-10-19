cmake_minimum_required(VERSION 3.14)

project(nes-libs)

# Setting up the target

add_library(nes-libs STATIC
  "src/bit_utils.cpp"
  "src/system_utils.cpp"
  "src/timer.cpp"
)

target_include_directories(nes-libs PUBLIC "include")

# Dependencies

add_subdirectory("external/Nes_Snd_Emu")
target_link_libraries(nes-libs PUBLIC Nes_Snd_Emu)

# Compiler settings

target_compile_features(nes-libs PRIVATE cxx_std_17)
