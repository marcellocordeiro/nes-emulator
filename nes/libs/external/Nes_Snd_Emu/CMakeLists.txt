cmake_minimum_required(VERSION 3.15)

project(Nes_Snd_Emu)

# Setting up the target

add_library(Nes_Snd_Emu STATIC
  "src/Blip_Buffer.cpp"
  "src/Nes_Apu.cpp"
  "src/Nes_Oscs.cpp"
)

target_include_directories(Nes_Snd_Emu PUBLIC "include")

# Compiler settings

target_compile_features(Nes_Snd_Emu PRIVATE cxx_std_20)
