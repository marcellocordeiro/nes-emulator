project(nes-emulator-glfw)

add_executable(nes-emulator-glfw
  "src/main.cpp"
  "src/MainWindow.cpp"
  "src/ShaderProgram.cpp"
  "src/Renderer.cpp"
)

target_link_libraries(nes-emulator-glfw PRIVATE common)

target_link_libraries(nes-emulator-glfw PRIVATE nes-core)

find_package(glfw3 REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE glfw)

target_link_libraries(nes-emulator-glfw PRIVATE glad)

find_package(OpenGL REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE OpenGL::GL)

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE fmt::fmt-header-only)

set_target_properties(nes-emulator-glfw
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-emulator-glfw PRIVATE project-warnings)
