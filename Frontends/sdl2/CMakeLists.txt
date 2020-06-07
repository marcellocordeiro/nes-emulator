project(nes-emulator-sdl2)

add_executable(nes-emulator-sdl2
  "src/main.cpp"
  "src/main_window.cpp"
  "src/system_utils.cpp"
)

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(nes-emulator-sdl2 PRIVATE SDL2::SDL2)

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator-sdl2 PRIVATE fmt::fmt)

target_link_libraries(nes-emulator-sdl2 PRIVATE nes-emulator)

set_target_properties(nes-emulator-sdl2
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
