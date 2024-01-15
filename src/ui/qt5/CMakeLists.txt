project(nes-emulator-qt5)

add_executable(nes-emulator-qt5
  "src/main.cpp"
  "src/MainWindow.cpp"
  "src/MainWindow.h"
  "src/MainWindow.ui"
  "src/QtOpenGL.cpp"
  "src/QtOpenGL.h"
)

target_include_directories(nes-emulator-qt5 PRIVATE "src")

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
target_link_libraries(nes-emulator-qt5 PRIVATE Qt5::Widgets Qt5::OpenGL)

find_package(OpenGL REQUIRED)
target_link_libraries(nes-emulator-qt5 PRIVATE OpenGL::GL)

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator-qt5 PRIVATE fmt::fmt-header-only)

target_link_libraries(nes-emulator-qt5 PRIVATE nes-emulator-core)

set_target_properties(nes-emulator-qt5
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_link_libraries(nes-emulator-qt5 PRIVATE project-warnings)
