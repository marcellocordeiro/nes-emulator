cmake_minimum_required(VERSION 3.14)

project(Nes_Snd_Emu)

find_package(SDL2 2.0 REQUIRED)

set(SOURCE_FILES
    "src/Blip_Buffer.cpp"
    "src/Nes_Apu.cpp"
    "src/Nes_Oscs.cpp"
    "src/Sound_Queue.cpp"
)

add_library(Nes_Snd_Emu STATIC ${SOURCE_FILES})

target_include_directories(Nes_Snd_Emu PUBLIC "include")
target_compile_features(Nes_Snd_Emu PRIVATE cxx_std_17)

if(MSVC)
    target_compile_definitions(Nes_Snd_Emu PRIVATE SDL_MAIN_HANDLED)
endif()

target_link_libraries(Nes_Snd_Emu PRIVATE SDL2::SDL2)
