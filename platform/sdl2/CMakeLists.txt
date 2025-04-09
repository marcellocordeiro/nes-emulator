project(nes-emulator-sdl2)

add_executable(nes-emulator-sdl2
  "src/main.cpp"
  "src/MainWindow.cpp"
)

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(nes-emulator-sdl2 PRIVATE SDL2::SDL2)

target_link_libraries(nes-emulator-sdl2 PRIVATE nes-core)

set_target_properties(nes-emulator-sdl2
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-emulator-sdl2 PRIVATE project-warnings)
