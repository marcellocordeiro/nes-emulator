cmake_minimum_required(VERSION 3.15)

project(Nes_Snd_Emu)

# Setting up the target

add_library(Nes_Snd_Emu STATIC)

target_sources(Nes_Snd_Emu PRIVATE
  "src/Blip_Buffer.cpp"
  "src/Nes_Apu.cpp"
  "src/Nes_Oscs.cpp"
  "src/Sound_Queue.cpp"
)

target_include_directories(Nes_Snd_Emu SYSTEM PUBLIC "include")

# Dependencies

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(Nes_Snd_Emu PRIVATE SDL2::SDL2)

# Compiler settings

target_compile_features(Nes_Snd_Emu PRIVATE cxx_std_17)
