cmake_minimum_required(VERSION 3.14)

project(nes-libs)

# Setting up the target

add_library(nes-libs INTERFACE)

target_include_directories(nes-libs INTERFACE "include")

# Dependencies

add_subdirectory("external/Nes_Snd_Emu")
target_link_libraries(nes-libs INTERFACE Nes_Snd_Emu)
