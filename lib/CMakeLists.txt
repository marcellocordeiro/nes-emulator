cmake_minimum_required(VERSION 3.14)

project(nes-emulator-lib)

#
# Dependencies
#

add_subdirectory(Nes_Snd_Emu)

#
# Setting up the target
#

set(SOURCE_FILES
  "src/log.cpp"
  "src/bit_utils.cpp"
)

add_library(nes-emulator-lib STATIC ${SOURCE_FILES})
target_include_directories(nes-emulator-lib SYSTEM PUBLIC "include")

target_link_libraries(nes-emulator-lib INTERFACE Nes_Snd_Emu)

#
# Compiler settings
#

target_compile_features(nes-emulator-lib PRIVATE cxx_std_17)
