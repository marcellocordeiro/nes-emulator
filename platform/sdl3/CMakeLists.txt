project(nes-emulator-sdl3)

add_executable(nes-emulator-sdl3
  "src/main.cpp"
  "src/MainWindow.cpp"
)

find_package(SDL3 CONFIG REQUIRED COMPONENTS SDL3-shared)
target_link_libraries(nes-emulator-sdl3 PRIVATE SDL3::SDL3)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(nes-emulator-sdl3 PRIVATE fmt::fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(nes-emulator-sdl3 PRIVATE spdlog::spdlog)

target_link_libraries(nes-emulator-sdl3 PRIVATE nes-core)

set_target_properties(nes-emulator-sdl3
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-emulator-sdl3 PRIVATE project-warnings)
