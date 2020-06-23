project(nes-emulator-glfw)

add_executable(nes-emulator-glfw
  "src/main.cpp"
  "src/main_window.cpp"
  "src/shader.cpp"
)

target_link_libraries(nes-emulator-glfw PRIVATE nes-emulator-core)

find_package(glfw3 REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE glfw)

find_package(GLEW REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE GLEW::GLEW)

find_package(OpenGL REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE OpenGL::GL)

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator-glfw PRIVATE fmt::fmt)

set_target_properties(nes-emulator-glfw
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-emulator-glfw PRIVATE project-warnings)
target_link_libraries(nes-emulator-glfw PRIVATE project-lib)
